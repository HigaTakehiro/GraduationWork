#include "BossScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
//#include "Normal//enemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"
#include"StageCount.h"

#pragma warning(disable:4996)

void BossScene::Initialize()
{
	ShowCursor(true);
	//ポストエフェクト初期化
	//画面大きさ設定
	const Vector3 LB = { -1.0f, -1.0f, 0.0f };
	const Vector3 LT = { -1.0f, +1.0f, 0.0f };
	const Vector3 RB = { +1.0f, -1.0f, 0.0f };
	const Vector3 RT = { +1.0f, +1.0f, 0.0f };
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize(LT, LB, RT, RB);

	//カメラ初期化
	CameraSetting();
	oldcamerapos_ = cameraPos_.z;
	//ライト初期化
	light_ = LightGroup::UniquePtrCreate();
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}

	dome = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("skydome"));
	dome->Initialize();
	dome->SetRotation({ 0.0f,90.f,0.0f });
	dome->SetPosition({ 30.f,0.f,30.f });

	//light->SetCircleShadowActive(0, true);
	Object3d::SetLight(light_.get());

	//3dオブジェクト初期化
	player_ = new Player;
	player_->Initialize();
	player_->SetLevel(SceneManager::GetLevel());
	player_->SetEP(SceneManager::GetEP());
	player_->SetHP(SceneManager::GetHP());
	player_->SetMaxHP(SceneManager::GetMaxHP());
	player_->SetATK(SceneManager::GetATK());
	player_->SetDEF(SceneManager::GetDEF());
	player_->SetSPD(SceneManager::GetSPD());
	player_->SetSkillPoint(SceneManager::GetSkillPoint());
	player_->SetLevelUpEP(SceneManager::GetLevelUpEP());

	postEffectNo_ = PostEffect::NONE;

	boss_.reset(new Dogom());
	boss_->Init();
	boss_->SetPlayerIns(player_);

	skillManager_->SetPlayer(player_);
	//if (player_->GetHP() > 0) {
	int Num = StageCount::GetIns()->Up();
	//}
	map_ = make_unique<GameMap>();
	map_->Initalize(player_, cameraPos_, targetPos_, 6);

	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFEnd(true);
	schange->SetFadeNum(1);

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
	boss_->GetCSPos(cameraPos_);

	m_Stairs.reset(new Stairs());
	m_Stairs->BossInitialize(Vector3(0, -0.f, 0), player_);

	SoundManager::GetIns()->StopAllBGM();
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::firstBoss, TRUE, 0.4f);

	invincibleParticle_ = ParticleManager::UniquePtrCreate(DirectXSetting::GetIns()->GetDev(), camera_.get());
	activeSkillPanel01_ = std::make_unique<SkillPanel>();
	activeSkillPanel01_->Initialize(L"Empty", { 287.f, 32.f }, SkillPanel::Empty);
	activeSkillPanel02_ = std::make_unique<SkillPanel>();
	activeSkillPanel02_->Initialize(L"Empty", { 352.f, 32.f }, SkillPanel::Empty);
	if (skillManager_->GetActiveSkillName01() != "None") {
		if (skillManager_->GetActiveSkillName01() == "HyperMode") {
			activeSkillPanel01_->Initialize(L"HyperMode", { 287.f, 32.f }, SkillPanel::HyperMode);
		}
	}
	if (skillManager_->GetActiveSkillName02() != "None") {
		if (skillManager_->GetActiveSkillName02() == "FallHammer") {
			activeSkillPanel02_->Initialize(L"FallHammer", { 352.f, 32.f }, SkillPanel::FallHammer);
		}
	}
}

void BossScene::Update()
{
	dome->Update();
	if (!boss_.get()) return;
	player_->Update();
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	//HPデバッグ処理
	if (KeyInput::GetIns()->TriggerKey(DIK_O)) {
		player_->SubHP(1);
	}
	if (KeyInput::GetIns()->TriggerKey(DIK_R)) {
		player_->SetHP(3);
	}

	if (map_->GetHit() == true) {
		ShakeCount++;
		if (ShakeCount == 1) {
			shake_->SetIwaFlag(true);
			shake_->SetShakeFlag(true);
		}
	}
	else {
		ShakeCount = 0;
	}
	if (shake_->GetShakeFlag() == true) {
		if (cameraPos_.y < 13 || cameraPos_.y > 12) {
			cameraPos_.y += shake_->GetShakePos();
			targetPos_.y += shake_->GetShakePos();
		}
	}
	else {
		cameraPos_.y = 12;
		targetPos_.y = 0;
	}
	//if (boss_->GetAppearFlag() == FALSE) {
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	//}//
		//boss_->SetCamera(camera_.get());
	light_->Update();
	//プレイヤーのOBB設定
	XMFLOAT3 trans = { player_->GetHammer()->GetMatWorld().r[3].m128_f32[0],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[1],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[2]
	};
	OBB l_obb;
	l_obb.SetParam_Pos(trans);
	l_obb.SetParam_Rot(player_->GetHammer()->GetMatRot());
	l_obb.SetParam_Scl({ 1.0f,2.10f,10.0f });
	_hummmerObb = &l_obb;
	boss_->Upda();
	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_);
	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition, player_->GetIsHammerRelease()));
	}
	else {
		player_->ResetOreCount();
	}
	boss_->SetHummerPos(player_->GetHammer()->GetPosition());
	shake_->Update();
	colManager_->Update();

	if (boss_->GetClearF()) {
		m_Stairs->Update();
		if (player_->GetNextFlor()) {
			if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
				touchFlor = TRUE;
			}
		}
	}
	//衝突時一旦破棄
	schange->Change(0);
	skillManager_->Update();
	activeSkillPanel01_->SetIsActive(skillManager_->GetIsActiveCheck("HyperMode"));
	activeSkillPanel02_->SetIsActive(skillManager_->GetIsActiveCheck("FallHammer"));
	activeSkillPanel01_->Update({ 0.f, 0.f });
	activeSkillPanel02_->Update({ 0.f, 0.f });
	invincibleParticle_->Update();

	//シーン切り替えmmm
	SceneChange();
	if (touchFlor)
	{
		if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
			if (StageCount::GetIns()->Now() <= 18) {
				SceneManager::SetLevel(player_->GetLevel());
				SceneManager::SetEP(player_->GetEP());
				SceneManager::SetHP(player_->GetHP());
				SceneManager::SetMaxHP(player_->GetMaxHP());
				SceneManager::SetATK(player_->GetATK());
				SceneManager::SetDEF(player_->GetDef());
				SceneManager::SetSPD(player_->GetSPD());
				SceneManager::SetSkillPoint(player_->GetSkillPoint());
				SceneManager::SetLevelUpEP(player_->GetLevelUpEP());
				SceneManager::SceneChange(SceneManager::SceneName::IB);
			}
			else {
				SceneManager::SceneChange(SceneManager::SceneName::Tutorial);
			}
		}
	}


}

void BossScene::Draw()
{
	//背景色
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };
	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());
	//スプライト描画処理(背景)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	background_->Draw();
	Sprite::PostDraw();
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	dome->Draw();
	map_->MapDraw();
	if (boss_->GetClearF())
		m_Stairs->Draw();
	Object3d::PostDraw();
	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	boss_->Draw();
	map_->BridgeDraw();
	player_->Draw();
	boss_->Draw2();
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());
	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());
	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲
	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hp = boss_->GetStr();
	//text_->Draw("meiryo", "white", L"ボスシーン\n左クリックまたはLボタンでタイトルシーン\n右クリックまたはRボタンでリザルトシーン\nシェイクはEnter\nHP : " + hp, textDrawRange);
	if (schange->GetFStart() == false && schange->GetFEnd() == false) {
		player_->TextUIDraw();
	}
	invincibleParticle_->Draw(DirectXSetting::GetIns()->GetCmdList());
	DirectXSetting::GetIns()->endDrawWithDirect2D();
	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);
	//ポストエフェクトをかけないスプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	player_->SpriteDraw();
	boss_->SpriteDraw();
	activeSkillPanel01_->SpriteDraw();
	activeSkillPanel02_->SpriteDraw();
	schange->Draw();
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}

void BossScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	safe_delete(player_);
	//boss_->Finalize();
	safe_delete(player_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	colManager_->Finalize();
	map_->Finalize();
	skillManager_->Finalize();
}

void BossScene::SceneChange()
{
	if (!player_->GetIsDead())return;


	bool Change = player_->GetNext();
	if (Change || player_->GetIsDead()) {
		SceneManager::SetLevel(player_->GetLevel());
		SceneManager::SetEP(player_->GetEP());
		SceneManager::SetHP(player_->GetHP());
		SceneManager::SetMaxHP(player_->GetMaxHP());
		SceneManager::SetATK(player_->GetATK());
		SceneManager::SetDEF(player_->GetDef());
		SceneManager::SetSPD(player_->GetSPD());
		SceneManager::SetSkillPoint(player_->GetSkillPoint());
		schange->SetFStart(true);
		schange->SetFadeNum(0);
		FILE* fp;
		int i;
		fp = fopen("Engine/Resources/GameData/save.csv", "w");
		fprintf(fp, "%d", 0);
		fclose(fp);
		fp = fopen("Engine/Resources/GameData/save.csv", "r");
		fscanf(fp, "%d", &i);
		fclose(fp);
		fp = fopen("Engine/Resources/GameData/save.csv", "r+");
		i = i + 1;
		fprintf(fp, "%d", i);
		fclose(fp);
		SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::firstBoss);
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}
}


void BossScene::CameraSetting()
{
	//if (boss_->GetAppearFlag())return;
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

void BossScene::ParticleCreate()
{
	//無敵状態パーティクル
	if (player_->GetIsInvincible()) {
		int32_t life = 30;
		Vector3 pos = player_->GetPos();
		pos.y -= 0.5f;

		Vector3 vel = { 0.f, 0.f, 0.f };
		float rnd_vel = 0.2f;
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.f;

		Vector3 acc = { 0.f, 0.f, 0.f };
		float rnd_acc = 0.015f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc * rnd_acc / 2.0f;

		invincibleParticle_->Add(life, pos, vel, acc, 1.f, 0.f, { 1.5f, 1.5f, 1.5f }, { 1.f, 1.f, 1.f }, 0.5f, 0.0f);
		invincibleParticle_->LoadTexture("Flash");
	}
}
