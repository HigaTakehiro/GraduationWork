#include "GameScene.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "Helper.h"
#include "NormalEnemyB.h"
#include "SoundManager.h"
#include"StageCount.h"

#pragma warning(disable:4996)

void GameScene::Initialize()
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

	int Num = StageCount::GetIns()->Up();

	map_ = make_unique<GameMap>();
	map_->Initalize(player_, cameraPos_, targetPos_, Num);

	skillManager_->SetPlayer(player_);

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFadeNum(1);
	schange->SetFEnd(true);
	aEffect_ = new AttackEffect();
	aEffect_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());
	aeCount = 0;
	SoundManager::GetIns()->StopAllBGM();
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::dungeon, TRUE, 0.4f);

	invincibleParticle_ = ParticleManager::UniquePtrCreate(DirectXSetting::GetIns()->GetDev(), camera_.get());

}

void GameScene::Update()
{
	dome->Update();
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
			ore->Update(player_->GetPos());
		}
	}

	for (int32_t i = 0; i < map_->GetDepositsSize(); i++) {
		std::unique_ptr<Deposit>& deposit = map_->GetDeposit(i);
		if (deposit != nullptr) {
			Helper::ColKnock(player_->GetPos(), deposit->GetPos(), player_, Collision::GetLength(player_->GetPos(), deposit->GetPos()) < 2.f, 1.5f);
		}
	}

	EnemyProcess();
	//当たったらシェイク
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
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	light_->Update();
	for (int i = 0; i < map_->GetDepositsSize(); i++) {
		unique_ptr<Deposit>& Dep = map_->GetDeposit(i);
		if (Dep != nullptr && Dep->GetHP() > 0) {
			if (Dep->GetIsHit(player_->GetIsHammerSwing())) {
				unique_ptr<Ore> ore = make_unique<Ore>();
				ore->Initialize(Dep->GetPos(), Dep->OreDropVec());
				oreItems_.push_back(std::move(ore));
			}
			Dep->Update(player_->GetPos());
		}
	}
	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_);
	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition, player_->GetIsHammerRelease()));
	}
	else {
		player_->ResetOreCount();
	}
	//boss_->SetHummerPos(player_->GetHammer()->GetPosition());

	shake_->Update();
	colManager_->Update();
	skillManager_->Update();

	if (player_ != nullptr) {
		ParticleCreate();
	}
	invincibleParticle_->Update();
	activeSkillPanel01_->SetIsActive(skillManager_->GetIsActiveCheck("HyperMode"));
	activeSkillPanel02_->SetIsActive(skillManager_->GetIsActiveCheck("FallHammer"));
	activeSkillPanel01_->Update({ 0.f, 0.f });
	activeSkillPanel02_->Update({ 0.f, 0.f });

	//シーン切り替え
	schange->Change(0);
	SceneChange();
}

void GameScene::Draw()
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
	for (int i = 0; i < map_->GetDepositsSize(); i++) {
		unique_ptr<Deposit>& Dep = map_->GetDeposit(i);
		if (Dep != nullptr) {
			if (map_->GetCount() != Dep ->GetMapNum() ) { continue; }
			Dep->Draw();
		}
	}
	Object3d::PostDraw();
	for (size_t i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		if (Enemy == nullptr ) { continue; }
		if (map_->GetCount() != Enemy->GetCount()) { continue; }
		
		Enemy->Draw();
		//if (Enemy->GetHP() > 0 && Enemy->GetFlash() == true) {
		//	aEffect_->Draw(DirectXSetting::GetIns()->GetCmdList());
		//}
	}

	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			ore->Draw();
		}
	}
	//boss_->Draw();
	//boss_->Draw2();

	for (size_t i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		if (Enemy == nullptr) { continue; }
		if (map_->GetCount() != Enemy->GetCount()) { continue; }
		Enemy->TexDraw();
	}
	map_->BridgeDraw();

	player_->Draw();
	invincibleParticle_->Draw(DirectXSetting::GetIns()->GetCmdList());
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲
	//
	D2D1_RECT_F textDrawRange = { 600, 0, 1280, 1280 };
	//std::wstring hx = std::to_wstring(cameraPos_.y);
	//text_->Draw("meiryo", "white", L"ゲームシーン\n" + hx, textDrawRange);
	if (schange->GetFStart() == false && schange->GetFEnd() == false) {
		player_->TextUIDraw();
	}
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//ポストエフェクトをかけないスプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	player_->SpriteDraw();
	activeSkillPanel01_->SpriteDraw();
	activeSkillPanel02_->SpriteDraw();
	schange->Draw();
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}

void GameScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	safe_delete(player_);
	invincibleParticle_->Finalize();
	//boss_->Finalize();
	safe_delete(player_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	colManager_->Finalize();
	map_->Finalize();
	skillManager_->Finalize();
}

void GameScene::SceneChange()
{
	SceneManager::SetLevel(player_->GetLevel());
	SceneManager::SetEP(player_->GetEP());
	SceneManager::SetHP(player_->GetHP());
	SceneManager::SetMaxHP(player_->GetMaxHP());
	SceneManager::SetATK(player_->GetATK());
	SceneManager::SetDEF(player_->GetDef());
	SceneManager::SetSPD(player_->GetSPD());
	SceneManager::SetSkillPoint(player_->GetSkillPoint());
	SceneManager::SetLevelUpEP(player_->GetLevelUpEP());

	bool Change = player_->GetNext();
	if (Change || player_->GetIsDead()) {
		schange->SetFStart(true);
		schange->SetFadeNum(0);
	}
	if (schange->GetEnd() == true) {
		FILE* fp;
		int i;
		fp = fopen("Engine/Resources/GameData/save.csv", "r");
		fscanf(fp, "%d", &i);
		fclose(fp);
		if (StageCount::GetIns()->Now() == 4||
			StageCount::GetIns()->Now() == 9||
			StageCount::GetIns()->Now() == 16) {
			if (i == 1||i==3||i==5) {
				fp = fopen("Engine/Resources/GameData/save.csv", "r+");
				i = i + 1;
				fprintf(fp, "%d", i);
				fclose(fp);
			}
			SceneManager::SceneChange(SceneManager::SceneName::IB);
		}
		else {
			SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::dungeon);
			SceneManager::SceneChange(SceneManager::SceneName::Game);
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
#pragma region 兎
	for (size_t i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		if (Enemy == nullptr) { continue; }
		if (Enemy->GetHP() <= 0) {
			player_->AddEP(1);
			continue;
		}
	}

	for (auto i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		if (Enemy == nullptr || Enemy->GetHP() <= 0) { continue; }
		XMFLOAT3 EnemyPos;
		int Num = Enemy->GetType();
		if (Enemy->GetType() == 0) { EnemyPos = Enemy->GetPos(); }
		else if (Enemy->GetType() == 1) { EnemyPos = Enemy->GetPos(); }

		//bool fallham = player_->GetIsFall() && Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 3.5f, { Enemy->GetPos().x, Enemy->GetPos().z + 3.f }, 1.0f);
		if ((Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { Enemy->GetPos().x, Enemy->GetPos().z + 3.f }, 1.0f)  && player_->GetIsAttack()))
		{
			if(!PadInput::GetIns()->PushButton(PadInput::Button_B)&& player_->GetIsHammerRelease())
			player_->SetIsHammerReflect(true);
			Enemy->SetDamF(true);

			//player_->SetIsFall(false);
		} else
		{
			Enemy->SetDamF(false);
		}
		if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { EnemyPos.x, EnemyPos.z+3.f }, 1.0f) && player_->GetIsAttack()) {
			Vector3 playerPos = player_->GetPos();
			Enemy->GetDamage(true);
			Vector3 Vec{};
			Vector3 eposcorr = { EnemyPos.x,EnemyPos.y, EnemyPos.z + 3.f };
			Vec = playerPos - eposcorr;
			Vec.normalize();
			Vec.y = 0.0f;
			player_->HitHammerToEnemy(Vec / 2.f);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerAttack, 0.2f);
		}
		Enemy->damage();
		//if (Enemy->GetHP() > 0 && Enemy->GetFlash() == true) {
		//	aEffect_->Update(Enemy->GetPos());
		//}
		if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { EnemyPos.x, EnemyPos.z }, 1.0f) && player_->GetIsHammerRelease()) {
			ShakeCount++;
			if (ShakeCount == 1) {
				shake_->SetIwaFlag(true);
				shake_->SetShakeFlag(true);
			}
		}
	}
#pragma endregion

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
#pragma region 兎
	for (size_t j = 0; j < map_->GetEnemySize(); j++) {
		unique_ptr<BaseEnemy>& Ene1 = map_->GetEnemy(j);
		for (size_t i = 0; i < map_->GetEnemySize(); i++) {
			unique_ptr<BaseEnemy>& Ene2 = map_->GetEnemy(i);
			if (i == j || Ene1 == nullptr || Ene2 == nullptr)continue;
			XMFLOAT3 Pos1;
			XMFLOAT3 Pos2;
			if (Ene1->GetType() == 0) { Pos1 = Ene1->GetPos(); }
			else if (Ene1->GetType() == 1) { Pos1 = Ene1->GetPos2(); }
			if (Ene2->GetType() == 0) { Pos2 = Ene2->GetPos(); }
			else if (Ene2->GetType() == 1) { Pos2 = Ene2->GetPos2(); }


			if (Collision::HitCircle(XMFLOAT2(Pos2.x,Pos2.z), 1.f,
				XMFLOAT2(Pos1.x,Pos1.z), 1.f))
			{
				XMFLOAT3 pos;
				if (Ene1->GetType() == 0) { pos = Ene1->GetPos(); }
				else if (Ene1->GetType() == 1) { pos = Ene1->GetPos2(); }
				pos.x += sin(atan2f((Ene1->GetPos().x - Ene2->GetPos().x), (Ene1->GetPos().z - Ene2->GetPos().z))) * 0.3f;
				pos.z += cos(atan2f((Ene1->GetPos().x - Ene2->GetPos().x), (Ene1->GetPos().z - Ene2->GetPos().z))) * 0.3f;

				
			}
		}
	}
	for (auto i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Ene = map_->GetEnemy(i);
		if (Ene == nullptr ) { continue; }
		if (map_->GetCount() != Ene->GetCount()) { continue; }
		if (Ene->GetHP() <= 0) { continue; }
		if (Ene->GetType() == 1) { Ene->SetPos(Ene->GetPos2()); }
		Ene->Upda(camera_.get());
	}
#pragma endregion
}

void GameScene::ParticleCreate()
{
	if (player_ != nullptr) {
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
}

