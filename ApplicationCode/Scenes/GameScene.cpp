#include "GameScene.h"

#include "ExternalFileLoader.h"
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

	postEffectNo_ = PostEffect::NONE;


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
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲
	//
	D2D1_RECT_F textDrawRange = { 600, 0, 1280, 1280 };
	//std::wstring hx = std::to_wstring(cameraPos_.y);
	//text_->Draw("meiryo", "white", L"ゲームシーン\n" + hx, textDrawRange);
	player_->TextUIDraw();
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//ポストエフェクトをかけないスプライト描画処理(UI等)
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

	bool Change = player_->GetNext();
	if (Change || player_->GetIsDead()) {
		schange->SetFStart(true);
		schange->SetFadeNum(0);
	}
	if (schange->GetEnd() == true) {

		if (StageCount::GetIns()->Now() == 4 ||
			StageCount::GetIns()->Now() == 9 ||
			StageCount::GetIns()->Now() == 16) {
			SceneManager::SceneChange(SceneManager::SceneName::IB);
		}
		else {
			SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::dungeon);
			SceneManager::SceneChange(SceneManager::SceneName::Game);
		}

	}
	//これいつか消すように
	if (PadInput::GetIns()->TriggerButton(PadInput::Button_X)) {
		SceneManager::SceneChange(SceneManager::SceneName::Boss);
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
		else if (Enemy->GetType() == 1) { EnemyPos = Enemy->GetPos2(); }
		if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { EnemyPos.x, EnemyPos.z }, 1.0f) && !player_->GetIsHammerRelease() && player_->GetIsAttack()) {
			Vector3 playerPos = player_->GetPos();
			Enemy->GetDamage();
			Vector3 Vec{};
			Vec = playerPos - EnemyPos;
			Vec.normalize();
			Vec.y = 0.0f;
			player_->HitHammerToEnemy(Vec / 2.f);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerAttack, 0.2f);
		}
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

