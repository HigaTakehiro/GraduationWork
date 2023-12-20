#include "GameScene.h"

#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "NormalEnemyB.h"
#include "SoundManager.h"
#include"StageCount.h"

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
	player_->SetLevel(SceneManager::GetLevel());
	player_->SetEP(SceneManager::GetEP());
	player_->SetHP(SceneManager::GetHP());

	postEffectNo_ = PostEffect::NONE;


	//���csv����
	unsigned int EnemySize = 3;

	enemys_.resize(EnemySize);
	vec.resize(EnemySize);

	for (size_t i = 0; i < enemys_.size(); i++) {
		enemys_[i] = new NormalEnemyB();
		enemys_[i]->Init();
		enemys_[i]->SetPlayerIns(player_);

		enemys_[i]->SetOverPos(XMFLOAT3(39.f, -100.f, 5.f), XMFLOAT3(23.f, 100.f, -5.f));
	}
	enemys_[0]->SetPos2(Vector3(30, 0, -4));
	enemys_[1]->SetPos2(Vector3(25, 0, 2));
	enemys_[2]->SetPos2(Vector3(35, 0, 5));

	for (size_t i = 0; i < enemys_.size(); i++)
		enemys_[i]->SetPosDeb(enemys_[i]->GetPos2());

	int Num = StageCount::GetIns()->Up();

	map_ = make_unique<GameMap>();

	map_->Initalize(player_, cameraPos_, targetPos_, Num);

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFadeNum(1);
	schange->SetFEnd(true);
	aEffect_ = new AttackEffect();
	aEffect_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());
	aeFlag = false;
	aeCount = 0;
	SoundManager::GetIns()->StopAllBGM();
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::dungeon, TRUE, 0.4f);
}

void GameScene::Update()
{
	player_->Update();
	oreItems_.remove_if([](std::unique_ptr<Ore>& ore) {return ore == nullptr; });
	if (player_->GetHP() <= 0) {
		SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::dungeon);
	}

	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			if (ore->GetIsHit() && player_->GetIsHammerSwing() && !player_->OreCountOverMaxCount()) {
				player_->AddOreCount();
				ore = nullptr;
			}
		}
		if (ore != nullptr) {
			ore->Update();
		}
	}

	EnemyProcess();
	if (shake_->GetShakeFlag() == true) {
		cameraPos_.y += shake_->GetShakePos();
		targetPos_.y += shake_->GetShakePos();
	}
	else {
		targetPos_.y = 0;
	}
	cameraPos_.y = 12;
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	light_->Update();

	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_);
	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition, player_->GetIsHammerRelease()));
	}
	//boss_->SetHummerPos(player_->GetHammer()->GetPosition());

	shake_->Update();
	colManager_->Update();

	//�V�[���؂�ւ�
	schange->Change(0);
	SceneChange();
}

void GameScene::Draw()
{
	//�w�i�F
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(�w�i)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	background_->Draw();
	Sprite::PostDraw();
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());

	map_->MapDraw();
	Object3d::PostDraw();
	for (auto i = 0; i < enemys_.size(); i++) {
		if (enemys_[i] != nullptr) {
			enemys_[i]->Draw();
		}
	}
	//3D�I�u�W�F�N�g�`�揈��
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			ore->Draw();
		}
	}
	//boss_->Draw();
	//boss_->Draw2();

	for (size_t i = 0; i < enemys_.size(); i++)
		//enemys_[i]->TexDraw();
	player_->Draw();
	for (size_t i = 0; i < enemys_.size(); i++) {
		if (enemys_[i]->GetFlash()) {
			aEffect_->Draw(DirectXSetting::GetIns()->GetCmdList());
		}
	}
	/*for (std::unique_ptr<Grass>& grass : grasses_) {
		grass->Draw();
	}*/
	map_->BridgeDraw();

	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//�e�L�X�g�`��͈�
	//
	D2D1_RECT_F textDrawRange = { 600, 0, 1280, 1280 };
	//std::wstring hx = std::to_wstring(player_->GetPos().z);
	//text_->Draw("meiryo", "white", L"�Q�[���V�[��\n���N���b�N�܂���L�{�^���Ń^�C�g���V�[��\n�E�N���b�N�܂���R�{�^���Ń��U���g�V�[��\n�V�F�C�N��Enter"+hx, textDrawRange);
	player_->TextUIDraw();
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//�|�X�g�G�t�F�N�g�`��
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//�|�X�g�G�t�F�N�g�������Ȃ��X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	player_->SpriteDraw();
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}

void GameScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	//boss_->Finalize();
	safe_delete(player_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	colManager_->Finalize();
	map_->Finalize();
}

void GameScene::SceneChange()
{
	SceneManager::SetLevel(player_->GetLevel());
	SceneManager::SetEP(player_->GetEP());
	SceneManager::SetHP(player_->GetHP());

	bool Change = player_->GetNext();
	if (Change || player_->GetIsDead()) {
		schange->SetFStart(true);
		schange->SetFadeNum(0);
	}
	if (schange->GetEnd() == true) {
		if (StageCount::GetIns()->Now() < 3) {
			SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::dungeon);
			SceneManager::SceneChange(SceneManager::SceneName::Game);
		}
		else {
			SceneManager::SceneChange(SceneManager::SceneName::IB);
		}
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

void GameScene::EnemyProcess()
{
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	Vector3 enemyPos[3] = {};

	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetHP() <= 0)
		{
			player_->AddEP(1);
			enemys_.erase(enemys_.begin() + i);
			continue;
		}
	}
	for (auto i = 0; i < enemys_.size(); i++) {
		if (enemys_[i]->GetHP() <= 0)continue;
		enemyPos[i] = enemys_[i]->GetPos();
		if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { enemyPos[i].x, enemyPos[i].z }, 1.0f) && !player_->GetIsHammerRelease() && player_->GetIsAttack()) {
			Vector3 playerPos = player_->GetPos();
			enemys_[i]->GetDamage();
			vec[i] = playerPos - enemyPos[i];
			vec[i].normalize();
			vec[i].y = 0.0f;
			aeFlag = true;
			player_->HitHammerToEnemy(vec[i]);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerAttack, 0.2f);
		}
		if (aeFlag == true) {
			aEffect_->Update(enemyPos[i], enemys_[i]->GetFlash());
		}
	}
	if (aeFlag == true) {
		if (aeCount < 30) {
			aeCount++;
		}
		else {
			aeCount = 0;
			aeFlag = false;
		}
	}

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

	for (size_t j = 0; j < enemys_.size(); j++)
	{
		for (size_t i = 0; i < enemys_.size(); i++)
		{
			if (i == j)continue;
			if (Collision::HitCircle(XMFLOAT2(enemys_[i]->GetPos().x, enemys_[i]->GetPos().z), 1.f,
				XMFLOAT2(enemys_[j]->GetPos().x, enemys_[j]->GetPos().z), 1.f))
			{
				XMFLOAT3 pos = enemys_[j]->GetPos();

				pos.x += sin(atan2f((enemys_[j]->GetPos().x - enemys_[i]->GetPos().x), (enemys_[j]->GetPos().z - enemys_[i]->GetPos().z))) * 0.3f;
				pos.z += cos(atan2f((enemys_[j]->GetPos().x - enemys_[i]->GetPos().x), (enemys_[j]->GetPos().z - enemys_[i]->GetPos().z))) * 0.3f;

				enemys_[j]->SetPos(pos);
			}
		}
	}
	for (auto i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetHP() <= 0) { continue; }
		if (enemys_[i] != nullptr) {
			enemys_[i]->SetHammerObb(*_hummmerObb);
			enemys_[i]->Upda(camera_.get());
		}
	}
}

