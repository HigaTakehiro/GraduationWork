#include "SecBossScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
//#include "Normal//enemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "Helper.h"
#include "SoundManager.h"
#include "StageCount.h"
#include "Togemaru.h"
#pragma warning(disable:4996)
void SecBossScene::Initialize()
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

	boss_.reset(new Togemaru());
	boss_->Init();
	boss_->SetPlayerIns(player_);

	int Num = StageCount::GetIns()->Up();
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

	Deposit_.reset(new Deposit());
	Deposit_->Initialize(Vector3(0, -2.5f, -8.f), true, camera_.get());


	Deposit_2.reset(new Deposit());
	Deposit_2->Initialize(Vector3(5, -2.5f, -8.f), true, camera_.get());


	SoundManager::GetIns()->StopAllBGM();
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::firstBoss, TRUE, 0.4f);
	cameraPos_.y = 12;
	targetPos_.y = 0;

	skillManager_->SetPlayer(player_);

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

void SecBossScene::Update()
{
	dome->Update();

	if (!boss_.get()) return;
	//SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::firstBoss,TRUE,0.4f);

	player_->Update();
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	//Vector3 //enemyPos[3] = {};


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


	TogemaruAct::DefaultPos = cameraPos_;
	if (!TogemaruAct::depositDelF && !TogemaruAct::depositDelF2) {
		TogemaruAct::oldCameraPos = cameraPos_;
	}
	cameraPos_.x += TogemaruAct::cameraPos.x;
	cameraPos_.y += TogemaruAct::cameraPos.y;
	//cameraPos_.z = 0;

	//cameraPos_ = { boss_->GetPos().x,boss_->GetPos().y + 2.5f,boss_->GetPos().z+8.f };
	//targetPos_ = boss_->GetPos();
	//if (boss_->GetAppearFlag() == FALSE) {
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);

	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_);
	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition, player_->GetIsHammerRelease()));
	}

	boss_->SetHummerPos(player_->GetHammer()->GetPosition());


	//shake_->Update();
	colManager_->Update();
	skillManager_->Update();
	//boss_->SetHummerPos(player_->GetHammer()->GetPosition());
	activeSkillPanel01_->SetIsActive(skillManager_->GetIsActiveCheck("HyperMode"));
	activeSkillPanel02_->SetIsActive(skillManager_->GetIsActiveCheck("FallHammer"));
	activeSkillPanel01_->Update({ 0.f, 0.f });
	activeSkillPanel02_->Update({ 0.f, 0.f });

	if (boss_->GetClearF()) {
		m_Stairs->Update();
		if (player_->GetNextFlor()) {
			if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
				touchFlor = TRUE;
			}
		}
	}

	//衝突時一旦破棄
	if (TogemaruAct::depositDelF && !m_DepositCreate) {
		m_DepositCreate = TRUE;
		Deposit_->SetDestroyF(true);//エフェクト生成用
	}
	if (TogemaruAct::depositDelF2 && !m_DepositCreate2) {
		m_DepositCreate2 = TRUE;
		Deposit_2->SetDestroyF(true);//エフェクト生成用
	}
	oreItems_.remove_if([](std::unique_ptr<Ore>& ore) {return ore == nullptr; });

	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			if (Collision::HitCircle({ ore->Getpos().x,ore->Getpos().z + 3.f }, 0.1f, { player_->GetPos().x,player_->GetPos().z }, 1.f)
				&& player_->GetIsHammerSwing() && !player_->OreCountOverMaxCount()) {
				player_->AddOreCount();
				ore = nullptr;
			}
		}
		if (ore != nullptr) {
			ore->Update(player_->GetPos());
		}
	}


	if (TogemaruAct::TogemaruDeathF)
	{
		Deposit_->SetDestroyBoss(true);
		Deposit_->SetParPos(boss_->GetPos());
		Deposit_->SetDestroyF(true);//エフェクト生成用
	}
	if (m_DepositCreate)
	{
		//完全に透明になったら破棄
		if (Deposit_->GetDepositAlpha() <= 0.f) {
			Deposit_.reset(nullptr);
		}
		//一定時間たっｔら鉱石復活
		if (!TogemaruAct::depositDelF) {
			Deposit_.reset(new Deposit());
			Deposit_->Initialize(TogemaruAct::depositPos, true, camera_.get());
			m_DepositCreate = FALSE;
		}
	}

	if (m_DepositCreate2)
	{
		//完全に透明になったら破棄
		if (Deposit_2->GetDepositAlpha() <= 0.f) {
			Deposit_2.reset(nullptr);
		}
		//一定時間たっｔら鉱石復活
		if (!TogemaruAct::depositDelF2) {
			Deposit_2.reset(new Deposit());
			Deposit_2->Initialize(TogemaruAct::depositPos2, true, camera_.get());
			m_DepositCreate2 = FALSE;
		}
	}


	if (!TogemaruAct::depositDelF && Deposit_ != nullptr && Deposit_->GetHP() > 0) {
		if (Deposit_->GetIsHit(player_->GetIsHammerSwing())) {
			unique_ptr<Ore> ore = make_unique<Ore>();
			ore->Initialize(Deposit_->GetPos(), Deposit_->OreDropVec());
			oreItems_.push_back(std::move(ore));
		}
		Deposit_->Update(player_->GetPos());
	}
	if (!TogemaruAct::depositDelF2&& Deposit_2 != nullptr &&  Deposit_2->GetHP() > 0) {
		if (Deposit_2->GetIsHit(player_->GetIsHammerSwing())) {
			unique_ptr<Ore> ore = make_unique<Ore>();
			ore->Initialize(Deposit_2->GetPos(), Deposit_2->OreDropVec());
			oreItems_.push_back(std::move(ore));
		}
		Deposit_2->Update(player_->GetPos());
	}
	Vector3 dpos = { Deposit_->GetPos().x,Deposit_->GetPos().y,Deposit_->GetPos().z + 3.f };
	Vector3 dpos2 = { Deposit_2->GetPos().x,Deposit_->GetPos().y,Deposit_2->GetPos().z + 3.f };
	Helper::ColKnock(player_->GetPos(), dpos, player_, !TogemaruAct::depositDelF&& Collision::GetLength(player_->GetPos(), dpos) < 3.5f, 1.5f);
	Helper::ColKnock(player_->GetPos(), dpos2, player_, !TogemaruAct::depositDelF2 && Collision::GetLength(player_->GetPos(), dpos2) < 3.5f, 1.5f);

	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition, player_->GetIsHammerRelease()));
	}
	else {
		player_->ResetOreCount();
	}
	if (Deposit_ != nullptr) {
		Deposit_->Update(player_->Get());
	}
	if (Deposit_2 != nullptr) {
		Deposit_2->Update(player_->Get());
	}
	schange->Change(0);
	ParticleCreate();
	invincibleParticle_->Update();

	//シーン切り替えmmm
	SceneChange();
	if (touchFlor)
	{
		if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
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
	}
}

void SecBossScene::Draw()
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
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			ore->Draw();
		}
	}
	boss_->Draw();
	map_->BridgeDraw();
	player_->Draw();
	invincibleParticle_->Draw(DirectXSetting::GetIns()->GetCmdList());

	if (player_->GetPos().z <= 12.f) {
		if (!TogemaruAct::depositDelF) {
			Deposit_->Draw();
		}
		if (!TogemaruAct::depositDelF2) {
			Deposit_2->Draw();
		}
	}
	boss_->Draw2();
	Deposit_->ParticleDraw();
	Deposit_2->ParticleDraw();
	Object3d::PostDraw();
	//shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲

	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hp = boss_->GetStr();
	//text_->Draw("meiryo", "white", L"ボスシーン\n左クリックまたはLボタンでタイトルシーン\n右クリックまたはRボタンでリザルトシーン\nシェイクはEnter\nHP : " + hp, textDrawRange);
	player_->TextUIDraw();

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
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}

void SecBossScene::Finalize()
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

void SecBossScene::SceneChange()
{
	if (!player_->GetIsDead())return;
	SceneManager::SetLevel(player_->GetLevel());
	SceneManager::SetEP(player_->GetEP());
	SceneManager::SetHP(player_->GetHP());
	SceneManager::SetMaxHP(player_->GetMaxHP());
	SceneManager::SetATK(player_->GetATK());
	SceneManager::SetDEF(player_->GetDef());
	SceneManager::SetSPD(player_->GetSPD());
	SceneManager::SetSkillPoint(player_->GetSkillPoint());
	SceneManager::SetLevelUpEP(player_->GetLevelUpEP());

	schange->SetFStart(true);
	schange->SetFadeNum(0);
	FILE* fp;
	int i;
	fp = fopen("Engine/Resources/GameData/save.csv", "r");
	fscanf(fp, "%d", &i);
	fclose(fp);
	if (i == 4) {
		fp = fopen("Engine/Resources/GameData/save.csv", "r+");
		i = i + 1;
		fprintf(fp, "%d", i);
		fclose(fp);
	}
	SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::firstBoss);
	SceneManager::SceneChange(SceneManager::SceneName::IB);

	//if (schange->GetEnd() == true) {
	//	SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::firstBoss);
	//	SceneManager::SceneChange(SceneManager::SceneName::IB);
	//}
	//if (/*MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
	//	SceneManager::SceneChange(SceneManager::SceneName::Title);
	//}
	//else if (/*MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
	//	SceneManager::SceneChange(SceneManager::SceneName::Result);
	//}
}


void SecBossScene::CameraSetting()
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

void SecBossScene::ParticleCreate()
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
