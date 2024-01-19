#include "TutorialScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"
#include "Helper.h"
#include"StageCount.h"
#include "HPUpSkill.h"
#include "SPDUpSkill.h"
#include "HammerReturnSkill.h"

#pragma warning(disable:4996)

void (TutorialScene::* TutorialScene::FuncTable[])() {
	&TutorialScene::TitlePhase,
		& TutorialScene::DescriptionPhase,
		& TutorialScene::MovePhase,
		& TutorialScene::SpownPhase,
		& TutorialScene::FightPhase,
		& TutorialScene::DefeatPhase,
		& TutorialScene::FreePhase,
};


void TutorialScene::Initialize()
{
	ShowCursor(true);
	//ポストエフェクト初期化
	//画面大きさ設定
	const Vector3 LB = { -1.0f, -1.0f, 0.0f };
	const Vector3 LT = { -1.0f, +1.0f, 0.0f };
	const Vector3 RB = { +1.0f, -1.0f, 0.0f };
	const Vector3 RT = { +1.0f, +1.0f, 0.0f };

	//プレイヤー初期化
	for (int32_t i = 0; i < 4; i++) {
		sleepModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_sleep.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}

	//sky = ModelManager::GetIns()->GetModel("skydome");
	dome=Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("skydome"));
	dome->Initialize();

	for (int i = 0; i < 9; i++) {
		title_[i] = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::title, { 0, 0 }, { 1.f, 1.f, 1.f, 1.f }, { 0.f, 0.f });
		title_[i]->SetTextureRect({ 960.f * i,0.f }, { 960.f ,128.f });
		title_[i]->SetSize({ 960.f,128.f });
		title_[i]->SetPosition(titleposition_);
	}

	wake_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::wake, { 0,0 }, { 1.f,1.f,1.f,1.f }, { 0.f,0.f });
	wake_->SetAlpha(1.5f);
	wake_->SetPosition(wakePos_);

	nextui_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::asist_, { 0,0 }, { 1.f,1.f,1.f,1.f }, { 0.f,0.f });
	nextui_->SetAlpha(1.5f);
	nextui_->SetPosition(asistPos_);

	sleep_ = Object3d::UniquePtrCreate(sleepModel_[0]);
	sleep_->SetIsBillboardY(true);
	sleep_->SetObbScl({ 2.f,4.f,2.f });
	sleep_->SetHitRadius(0.5f);
	sleep_->SetScale({ 0.035f, 0.035f, 0.035f });
	sleep_->SetPosition(sleepPos_);


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

	postEffectNo_ = PostEffect::NONE;

	startenemypos_[0] = { 5, 12.5, 18 };
	startenemypos_[1] = { -5, 12.5, 18 };

	map_ = make_unique<GameMap>();
	map_->Initalize(player_, cameraPos_, targetPos_, 0);

	for (size_t i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		Enemy->SetPos(startenemypos_[i]);
	}

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	textWindow_ = new MessageWindow();
	textWindow_->Initialize("TutorialMessage.csv");

	fighttextwindow_ = new MessageWindow();
	fighttextwindow_->Initialize("TutorialMessage2.csv");

	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });

	titlefilter_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::filter, { WinApp::window_width / 2, WinApp::window_height / 2 + 150.f }, { 0.f, 0.f, 0.f, 1.0f }, { 0.5f, 0.71f });
	titlefilter_->SetSize(size_);
	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFEnd(true);
	schange->SetFadeNum(1);
	phase_ = Phase::Title;
	oldpushCount_ = pushCount_;

	skillManager_->SetPlayer(player_);

	SoundManager::GetIns()->StopAllBGM();
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::title, TRUE, 0.3f);

}

void TutorialScene::Update()
{
	dome->Update();

	int32_t Max = player_->GetMaxHP();
	player_->SetHP(Max);
	oreItems_.remove_if([](std::unique_ptr<Ore>& ore) {return ore == nullptr; });
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

	(this->*FuncTable[phase_])();
	if (map_->GetHit() == true) {
		ShakeCount++;
		if (ShakeCount < 30) {
			shake_->SetIwaFlag(true);
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
		//cameraPos_.x += shake_->GetShakePos();
		//targetPos_.x += shake_->GetShakePos();
	}
	else {
		cameraPos_.y = 12;
		//cameraPos_.x = 0;
		targetPos_.y = 0;
		//targetPos_.x = 0;
	}
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

	cameraPos_.y = 12;
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	player_->TutorialUpdate(stop_, notattack_);
	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_, notlook_);

	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition, player_->GetIsHammerRelease()));
	}

	schange->Change(0);

	if (phase_ == Phase::Title) { return; }
	shake_->Update();
	colManager_->Update();

	if (phase_ >= Phase::Spown) {
		EnemyProcess();
	}
	FILE* fp;
	fp = fopen("Engine/Resources/GameData/floor.csv", "w");
	fprintf(fp, "floor %d", 0);
	fclose(fp);
	static bool isAddSkill = false;
	if (KeyInput::GetIns()->TriggerKey(DIK_T) && !isAddSkill) {
		HPUpSkill* hpUp = new HPUpSkill("hpUp_Test", 5);
		skillManager_->AddPlayerPassiveSkill(hpUp);
		HammerReturnSkill* hammerReturn = new HammerReturnSkill("HammerReturn");
		skillManager_->AddPlayerPassiveSkill(hammerReturn);
		isAddSkill = true;
	}

	skillManager_->Update();

	SceneChange();

}

void TutorialScene::Draw()
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
			Dep->Draw();
		}
	}

	Object3d::PostDraw();
	/*for (auto i = 0; i < enemys_.size(); i++) {
		if (enemys_[i] != nullptr) {
			enemys_[i]->TutorialDraw(25.f);
		}
	}	*/

	for (size_t i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		if (Enemy == nullptr) { continue; }
		Enemy->TutorialDraw(25.f);
	}
	
	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			ore->Draw();
		}
	}
	/*for (size_t i = 0; i < enemys_.size(); i++)
		enemys_[i]->TutorialTexDraw();*/

	for (size_t i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		if (Enemy == nullptr) { continue; }
		Enemy->TutorialTexDraw();
	}
	map_->BridgeDraw(notlook_);

	if (phase_ == Phase::Title) { sleep_->Draw(); }
	else { player_->Draw(); }

	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	titlefilter_->Draw();
	title_[titleanimeCount_]->Draw();
	wake_->Draw();
	if (phase_ == Phase::Description || phase_ == Phase::Spown) {
		nextui_->Draw();
	}

	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲

	D2D1_RECT_F textDrawRange = { 600, 0, 1280, 1280 };
	//text_->Draw("meiryo", "white", L"チュートリアルシーン\n左クリックまたはLボタンでタイトルシーン\n右クリックまたはRボタンでリザルトシーン\nシェイクはEnter", textDrawRange);
	std::wstring MoveTimer = std::to_wstring((int32_t)movetimer_);
	if (phase_ == Phase::Move) {
		movetextui_->Draw("bestTen", "white", L"Lスティックで動いてみよう\n10/" + MoveTimer, textDrawRange);
	}

	if (phase_ == Phase::Fight) {
		fighttextui_->Draw("bestTen", "white", L"敵を全て倒そう\nBボタンでハンマーを振り回し体当たり\n", textDrawRange);
	}

	if (phase_ == Phase::Defeat) {
		fighttextui_->Draw("bestTen", "white", L"洞窟を進んで階段へ向かおう\n", textDrawRange);
	}

	if (phase_ != Phase::Title) {
		player_->TextUIDraw();
		textWindow_->TextMessageDraw();
		fighttextwindow_->TextMessageDraw();
	}
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);
	if (phase_ != Phase::Title) {
		//ポストエフェクトをかけないスプライト描画処理(UI等)
		Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
		player_->SpriteDraw();
		textWindow_->SpriteDraw();
		fighttextwindow_->SpriteDraw();
		Sprite::PostDraw();
	}
	DirectXSetting::GetIns()->PostDraw();
}

void TutorialScene::Finalize()
{
	safe_delete(textWindow_);
	safe_delete(movetextui_);
	safe_delete(fighttextui_);
	safe_delete(fighttextwindow_);
	for (int i = 0; i < 4; i++) {
		safe_delete(sleepModel_[i]);
	}
	player_->Finalize();
	safe_delete(player_);
	map_->Finalize();
	wake_.release();
	nextui_.release();
	titlefilter_.release();
	skillManager_->Finalize();
}

void TutorialScene::SceneChange()
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
	if (Change || player_->GetHP() <= 0) {
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}

	//これいつか消すように
	if (PadInput::GetIns()->TriggerButton(PadInput::Button_X)) {
		SceneManager::SceneChange(SceneManager::SceneName::Boss2);
	}

	if (PadInput::GetIns()->PushButton(PadInput::Button_A) && PadInput::GetIns()->PushButton(PadInput::Button_RB) && PadInput::GetIns()->PushButton(PadInput::Button_LB)) {
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}
}

void TutorialScene::CameraSetting()
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

void TutorialScene::EnemyProcess()
{
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	Vector3 enemyPos[3] = {};

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
		if (Enemy == nullptr||Enemy->GetHP()<=0) { continue; }
		enemyPos[i] = Enemy->GetPos();
		if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { enemyPos[i].x, enemyPos[i].z }, 1.0f) && !player_->GetIsHammerRelease() && player_->GetIsAttack()) {
			Vector3 playerPos = player_->GetPos();
			Enemy->GetDamage();
			Vector3 vec{};
			vec = playerPos - enemyPos[i];
			vec.normalize();
			vec.y = 0.0f;
			player_->HitHammerToEnemy(vec / 2.f);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerAttack, 0.2f);
		}
	}

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

	for (size_t j = 0; j < map_->GetEnemySize(); j++) {
		unique_ptr<BaseEnemy>& Ene1 = map_->GetEnemy(j);
		for (size_t i = 0; i < map_->GetEnemySize(); i++) {
			unique_ptr<BaseEnemy>& Ene2 = map_->GetEnemy(i);
			if (i == j||Ene1==nullptr||Ene2==nullptr)continue;
			if (Collision::HitCircle(XMFLOAT2(Ene2->GetPos().x, Ene2->GetPos().z), 1.f,
				XMFLOAT2(Ene1->GetPos().x, Ene1->GetPos().z), 1.f))
			{
				XMFLOAT3 pos = Ene1->GetPos();

				pos.x += sin(atan2f((Ene1->GetPos().x - Ene2->GetPos().x), (Ene1->GetPos().z - Ene2->GetPos().z))) * 0.3f;
				pos.z += cos(atan2f((Ene1->GetPos().x - Ene2->GetPos().x), (Ene1->GetPos().z - Ene2->GetPos().z))) * 0.3f;

				Ene1->SetPos(pos);
			}
		}
	}

	for (auto i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Ene = map_->GetEnemy(i);
		if (Ene == nullptr) { continue; }
		if (Ene->GetHP() <= 0 ) { continue; }
		Ene->SetHammerObb(*_hummmerObb);
		Ene->TutorialUpda(camera_.get(), notjump_);
	}

}

void TutorialScene::SleepShale()
{
	if (!action_) {
		if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
			pushCount_ += 1;
			shaketimer_ = 0;
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::playerDamage, 0.2f);
			if (pushCount_ >= 5) {
				action_ = true;
			}
		}
	}

	if (pushCount_ > oldpushCount_) {
		shaketimer_ += 1;
		if (shaketimer_ % 2 == 0) {
			startpos_.x += shakeval_;
		}
		else {
			startpos_.x -= shakeval_;
			shakeval_ -= 0.02f;
		}
		if (shaketimer_ >= 10) {
			shakeval_ = 0.5f;
			oldpushCount_ = pushCount_;
		}
	}

}


void TutorialScene::TitlePhase()
{
	//タイマーカウント
	if (++animeTimer_ >= animeSpeed_) {
		if (++animeCount_ >= 4) {
			animeCount_ = 0;
		}
		animeTimer_ = 0;
	}

	//タイマーカウント
	if (++titleanimeTimer_ >= titleanimeSpeed_) {
		if (++titleanimeCount_ >= 9) {
			titleanimeCount_ = 0;
		}
		titleanimeTimer_ = 0;
	}

	if (titlepreAnimeCount_ != titleanimeCount_) {

		titlepreAnimeCount_ = titleanimeCount_;
	}

	SleepShale();
	sleep_->SetModel(sleepModel_[animeCount_]);
	sleep_->Initialize();
	if (titlepos_) {
		startpos_ = player_->Get();
		startpos_.z = startpos_.z + 6.f;
		player_->SetPos(startpos_);
		titlepos_ = false;
	}

	if (action_ && shaketimer_ >= 10) {
		timer_ += 0.1f;
		size_.x += 500.f;
		size_.y += 500.f;
		titleposition_.y -= 40;
		wakePos_.y += 100;
		if (timer_ >= 1) {
			phase_ = Phase::Description;
		}
	}
	titlefilter_->SetSize(size_);
	for (int i = 0; i < 9; i++) {
		title_[i]->SetPosition(titleposition_);
	}
	wake_->SetPosition(wakePos_);
	sleep_->SetPosition(startpos_);
	sleep_->Update();
	if (preAnimeCount_ == animeCount_) return;
	preAnimeCount_ = animeCount_;
}

void TutorialScene::DescriptionPhase()
{
	textWindow_->Update();
	if (!textWindow_->GetCloseWindow()) {
		description_ = 0;
		phase_ = Phase::Move;
	}
}

void TutorialScene::MovePhase()
{
	float LeftStickX = PadInput::GetIns()->leftStickX();
	float LeftStickY = PadInput::GetIns()->leftStickY();

	if (KeyInput::GetIns()->PushKey(DIK_RIGHT) ||
		KeyInput::GetIns()->PushKey(DIK_LEFT) ||
		KeyInput::GetIns()->PushKey(DIK_UP) ||
		KeyInput::GetIns()->PushKey(DIK_DOWN)) {
		movetimer_ += 0.1f;
	}

	if (LeftStickX != 0 || LeftStickY != 0) {
		movetimer_ += 0.1f;
	}

	stop_ = false;
	if (movetimer_ >= 10) {
		phase_ = Phase::Spown;
	}
}

void TutorialScene::SpownPhase()
{
	notjump_ = true;
	notattack_ = true;
	stop_ = true;

	fighttextwindow_->Update();

	/*if (startenemypos_[0].y >= -2.5f) {
		startenemypos_[0].y -= 1.f;
		enemys_[0]->SetPos(startenemypos_[0]);
	}
	if (startenemypos_[1].y >= -2.5f) {
		startenemypos_[1].y -= 1.f;
		enemys_[1]->SetPos(startenemypos_[1]);
	}*/

	for (size_t i = 0; i < map_->GetEnemySize(); i++) {
		unique_ptr<BaseEnemy>& Enemy = map_->GetEnemy(i);
		if (Enemy == nullptr) { continue; }
		if (startenemypos_[i].y >= -2.5f) {
			startenemypos_[i].y -= 1.f;
			Enemy->SetPos(startenemypos_[i]);
		}
	}

	if (!fighttextwindow_->GetCloseWindow()) {
		description_ = 0;
		phase_ = Phase::Fight;
	}
}

void TutorialScene::FightPhase()
{
	notjump_ = false;
	notattack_ = false;
	stop_ = false;

	if (map_->EnemyAllKill()) {
		phase_ = Phase::Defeat;
	}	
	/*if (enemys_.size() == 0) {
		phase_ = Phase::Defeat;
	}*/
}

void TutorialScene::DefeatPhase()
{
	notlook_ = true;
}

void TutorialScene::FreePhase()
{
}