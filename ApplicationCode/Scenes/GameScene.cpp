#include "GameScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"

void GameScene::Initialize()
{
	ShowCursor(true);
	//�|�X�g�G�t�F�N�g������
	//��ʑ傫���ݒ�
	const Vector3 LB = { -1.0f, -1.0f, 0.0f };
	const Vector3 LT = { -1.0f, +1.0f, 0.0f };
	const Vector3 RB = { +1.0f, -1.0f, 0.0f };
	const Vector3 RT = { +1.0f, +1.0f, 0.0f };
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize(LT, LB, RT, RB);

	//�J����������
	CameraSetting();
	oldcamerapos_ = cameraPos_.z;
	//���C�g������
	light_ = LightGroup::UniquePtrCreate();
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}
	//light->SetCircleShadowActive(0, true);
	Object3d::SetLight(light_.get());

	//3d�I�u�W�F�N�g������
	player_ = new Player;
	player_->Initialize();

	postEffectNo_ = PostEffect::NONE;

	boss_.reset(new Dogom());
	boss_->Init();
	boss_->SetPlayerIns(player_);

	for (auto i = 0; i < enemys_.size(); i++) {
		enemys_[i] = new NormalEnemyA();
		enemys_[i]->Init();

		enemys_[i]->SetPlayerIns(player_);

		aEffect_[i] = new AttackEffect();
		aEffect_[i]->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());
		eeFlag[i] = false;
	}
	enemys_[0]->SetPos(Vector3(10, -30, 10));
	enemys_[2]->SetPos(Vector3(-15, -30, -5));
	enemys_[2]->SetPos(Vector3(0, -30, -5));
	map_ = make_unique<GameMap>();
	map_->Initalize();
	mapBase_ = make_unique < IntermediateBase>();
	mapBase_->Initialize();

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());
	count_ = map_->NextCount(player_->GetPos(), direction);
	oldcount_ = count_;

	ore_ = std::make_unique<Ore>();
	ore_->Initialize({ -5, 2, -5 }, { 1, 0, 0 });

	for (int32_t i = 0; i < 3; i++) {
		std::unique_ptr<Ore> newOre = std::make_unique<Ore>();
		newOre->Initialize({ -5 + ((float)i * 5), 2, -10 }, { 0, 0, 0 });
		oreItems_.push_back(std::move(newOre));
	}
}

void GameScene::Update()
{
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			if (ore->GetIsHit() && player_->GetOreCountRate() < 1.0f && player_->GetIsHammerSwing()) {
				player_->AddOreCount();
				ore = nullptr;
			}
		}
		if (ore != nullptr) {
			ore->Update();
		}
	}

	if (ore_ != nullptr) {
		if (ore_->GetIsHit() && player_->GetOreCountRate() < 1.0f && player_->GetIsHammerSwing()) {
			player_->AddOreCount();
			ore_ = nullptr;
		}
	}

	if (ore_ != nullptr) {
		ore_->Update();
	}

	player_->Update();
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	Vector3 enemyPos[3] = { enemys_[0]->GetPos(),enemys_[1]->GetPos() ,enemys_[2]->GetPos() };
	Vector3 vec[3];
	for (auto i = 0; i < enemys_.size(); i++) {
		if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { enemyPos[i].x, enemyPos[i].z }, 1.0f) && !player_->GetIsHammerRelease() && player_->GetIsAttack()) {
			Vector3 playerPos = player_->GetPos();
			enemys_[i]->GetDamage();
			vec[i] = playerPos - enemyPos[i];
			vec[i].normalize();
			vec[i].y = 0.0f;
			eeFlag[i] = true;
			attackCount = 0;
			player_->HitHammerToEnemy(vec[i]);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::attack, 0.2f);
		}
		if (attackCount > 10) {
			eeFlag[i] = false;
		}
		if (eeFlag[i] == true) {
			attackCount++;
		}
		aEffect_[i]->Update(enemys_[i]->GetPos());
	}
	//�f�o�b�O�J�����ړ�����
	if (KeyInput::GetIns()->HoldKey(DIK_W)) {
		cameraPos_.z += 1.0f;
		targetPos_.z += 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_S)) {
		cameraPos_.z -= 1.0f;
		targetPos_.z -= 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_A)) {
		cameraPos_.x += 1.0f;
		targetPos_.x += 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_D)) {
		cameraPos_.x -= 1.0f;
		targetPos_.x -= 1.0f;
	}
	//HP�f�o�b�O����
	if (KeyInput::GetIns()->TriggerKey(DIK_O)) {
		player_->SubHP(1);
	}
	if (KeyInput::GetIns()->TriggerKey(DIK_R)) {
		player_->SetHP(3);
	}

	if (shake_->GetShakeFlag() == true) {
		cameraPos_.y += shake_->GetShakePos();
		targetPos_.y += shake_->GetShakePos();
	}
	else {
		cameraPos_.y = 12;
		targetPos_.y = 0;

	}

	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	light_->Update();

	//�v���C���[��OBB�ݒ�
	XMFLOAT3 trans = { player_->GetHammer()->GetMatWorld().r[3].m128_f32[0],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[1],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[2]
	};
	OBB l_obb;
	l_obb.SetParam_Pos(trans);
	l_obb.SetParam_Rot(player_->GetHammer()->GetMatRot());
	l_obb.SetParam_Scl({ 1.0f,2.10f,10.0f });

	_hummmerObb = &l_obb;

	for (auto i = 0; i < enemys_.size(); i++)
	{
		enemys_[i]->SetHammerObb(*_hummmerObb);
		enemys_[i]->Upda(camera_.get());

	}
	boss_->Upda();
	NextMap();
	map_->CheckHitTest(player_);
	map_->Update();
	//if(���ԋ��_�ɂ�����Z�[�u){
	map_->MapSave(1);
	//}
	mapBase_->Update();
	//if (player_->GetHP() <= 0) {
		if (KeyInput::GetIns()->HoldKey(DIK_X)) {
			player_->SetPos({ 0,0,0 });
		}
	//}
	boss_->SetHummerPos(player_->GetHammer()->GetPosition());
	shake_->Update();
	colManager_->Update();
	//�V�[���؂�ւ�
	SceneChange();
}

void GameScene::Draw()
{
	//�w�i�F
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(�w�i)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	map_->Draw(oldcount_);
	//mapBase_->Draw();
	Object3d::PostDraw();

	for (auto i = 0; i < enemys_.size(); i++) {
		enemys_[i]->Draw();
		if (eeFlag[i] == true) {
			aEffect_[i]->Draw(DirectXSetting::GetIns()->GetCmdList());
		}
	}
	boss_->Draw2();
	//3D�I�u�W�F�N�g�`�揈��
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	//map_->Draw(oldcount_);
	if (ore_ != nullptr) {
		ore_->Draw();
	}
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			ore->Draw();
		}
	}boss_->Draw();
	player_->Draw();

	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//�e�L�X�g�`��͈�

	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };

	std::wstring hp = std::to_wstring(player_->GetHP());
	text_->Draw("meiryo", "white", L"�Q�[���V�[��\n���N���b�N�܂���L�{�^���Ń^�C�g���V�[��\n�E�N���b�N�܂���R�{�^���Ń��U���g�V�[��\n�V�F�C�N��Enter\nHP : " + hp, textDrawRange);

	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//�|�X�g�G�t�F�N�g�`��
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//�|�X�g�G�t�F�N�g�������Ȃ��X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}


void GameScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	boss_->Finalize();
	safe_delete(player_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	colManager_->Finalize();
	map_->Finalize();
}

void GameScene::SceneChange()
{
	if (/*MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
		SceneManager::SceneChange(SceneManager::SceneName::Title);
	}
	else if (/*MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
		SceneManager::SceneChange(SceneManager::SceneName::Result);
	}
}

void GameScene::CameraSetting()
{
	std::string line;
	Vector3 pos{};
	Vector3 target{};
	std::stringstream stream;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("CameraSetting.csv");

	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("#") == 0) {
			continue;
		}
		if (word.find("pos") == 0) {
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
		}
		if (word.find("target") == 0) {
			line_stream >> target.x;
			line_stream >> target.y;
			line_stream >> target.z;
		}

		cameraPos_ = pos;
		targetPos_ = target;

		camera_ = std::make_unique<Camera>();
		camera_->SetEye(cameraPos_);
		camera_->SetTarget(targetPos_);
	}
}

void GameScene::NextMap()
{
	//�ړ����ł͂Ȃ�
	if (player_->GetNotNext()) { return; }
	//�v���C���[���}�b�v�̒[�ɗ�����
	count_ = map_->NextCount(player_->GetPos(), direction);
	player_->SetStop(true);
	float NextTarget = 0;
	XMFLOAT3 NextPos_ = map_->GetNowMapPos();
	XMFLOAT3 PlayerPos = player_->GetPos();
	XMFLOAT3 NEXTPLAYERPOS{};
	NextTarget = oldcamerapos_ + NextPos_.z - 2.f;


	if (direction == 0) { player_->SetStop(false); return; }
	if (direction == 2) {
		NEXTPLAYERPOS.x = NextPos_.x - 5;
		NEXTPLAYERPOS.z = PlayerPos.z;
	}
	else if (direction == 1) {
		NEXTPLAYERPOS.x = NextPos_.x + 7;
		NEXTPLAYERPOS.z = PlayerPos.z;
	}
	else if (direction == 4) {
		NEXTPLAYERPOS.z = NextPos_.z + 9;
		NEXTPLAYERPOS.x = PlayerPos.x;
	}
	else if (direction == 3) {
		NEXTPLAYERPOS.z = NextPos_.z - 4;
		NEXTPLAYERPOS.x = PlayerPos.x;
	}


	time_ += 0.01f;
	cameraPos_.x = Easing::easeIn(time_, 0.7, cameraPos_.x, NextPos_.x);
	targetPos_.x = Easing::easeIn(time_, 0.7, targetPos_.x, NextPos_.x);
	cameraPos_.z = Easing::easeIn(time_, 0.7, cameraPos_.z, NextTarget);
	targetPos_.z = Easing::easeIn(time_, 0.7, targetPos_.z, NextPos_.z);
	PlayerPos.x = Easing::easeIn(time_, 0.3, PlayerPos.x, NEXTPLAYERPOS.x);
	PlayerPos.z = Easing::easeIn(time_, 0.3, PlayerPos.z, NEXTPLAYERPOS.z);

	player_->SetPos(PlayerPos);
	if (time_ >= 0.7) {
		oldcount_ = count_; time_ = 0; map_->SetStop(false); player_->SetStop(false);
	}
}
