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
		sleepModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_sleep.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}

	for (int i = 0; i < 9; i++) {
		title_[i] = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::title, { 0, 0 }, { 1.f, 1.f, 1.f, 1.f }, { 0.f, 0.f });
		title_[i]->SetTextureRect({ 960.f * i,0.f }, { 960.f ,128.f });
		title_[i]->SetSize({ 960.f,128.f });
		title_[i]->SetPosition(titleposition_);
	}

	wake_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::wake, { 0,0 }, { 1.f,1.f,1.f,1.f }, { 0.f,0.f });

	sleep_ = Object3d::UniquePtrCreate(sleepModel_[0]);
	sleep_->SetIsBillboardY(true);
	sleep_->SetObbScl({ 2.f,4.f,2.f });
	sleep_->SetHitRadius(0.5f);
	sleep_->SetScale({ 0.035f, 0.035f, 0.035f });
	sleep_->SetPosition({ 0.f,-2.5f,33.f });
	//鉱床
	deposit_ = new Deposit();
	deposit_->Initialize({ 0.f, 0.f, 30.f });

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

	//後でcsvから
	unsigned int EnemySize = 3;

	enemys_.resize(EnemySize);
	vec.resize(EnemySize);

	for (size_t i = 0; i < enemys_.size(); i++) {
		enemys_[i] = new NormalEnemyA();
		enemys_[i]->Init();
		enemys_[i]->SetPlayerIns(player_);
		enemys_[i]->SetOverPos(XMFLOAT3(13.f, -100.f, 49.f), XMFLOAT3(-11.f, 100.f, -5.f));
	}
	enemys_[0]->SetPos(Vector3(5, -30, 30));
	enemys_[1]->SetPos(Vector3(-5, -30, 30));
	enemys_[2]->SetPos(Vector3(0, -30, 30));
	
	map_ = make_unique<GameMap>();
	map_->Initalize(player_, cameraPos_, targetPos_, 0);

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
}

void TutorialScene::Update()
{
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::title, TRUE, 0.3f);
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

	(this->*FuncTable[phase_])();
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
	player_->TutorialUpdate(stop_, notattack_);

	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_, notlook_);

	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition));
	}

	if (deposit_ != nullptr) {
		deposit_->Update();
	}

	
	if (phase_ == Phase::Title) { return; }
	shake_->Update();
	colManager_->Update();

	if (deposit_ != nullptr) {
		if (deposit_->GetIsHit()) {
			std::unique_ptr<Ore> ore = std::make_unique<Ore>();
			ore->Initialize(deposit_->GetPos(), deposit_->OreDropVec());
			oreItems_.push_back(std::move(ore));
		}
		if (deposit_->GetHP() <= 0) {
			safe_delete(deposit_);
		}
	}
	
	if (phase_ >= Phase::Spown) {
		EnemyProcess();
	}
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
	map_->MapDraw();
	Object3d::PostDraw();
	for (auto i = 0; i < enemys_.size(); i++) {
		if (enemys_[i] != nullptr) {
			enemys_[i]->Draw();
		}
	}	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	for (size_t i = 0; i < enemys_.size(); i++)
		enemys_[i]->TexDraw();
	if (phase_ == Phase::Title) {sleep_->Draw();}
	else {player_->Draw();}
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			//ore->Draw();
		}
	}
	if (deposit_ != nullptr) {
		deposit_->Draw();
	}
	map_->BridgeDraw(notlook_);
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	//for (int i = 0; i < 9; i++) {

	//}
	titlefilter_->Draw();
	title_[titleanimeCount_]->Draw();
	wake_->Draw();
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲

	D2D1_RECT_F textDrawRange = { 600, 0, 1280, 1280 };
	//text_->Draw("meiryo", "white", L"チュートリアルシーン\n左クリックまたはLボタンでタイトルシーン\n右クリックまたはRボタンでリザルトシーン\nシェイクはEnter", textDrawRange);
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
	safe_delete(deposit_);
}

void TutorialScene::SceneChange()
{

	bool Change = player_->GetNext();
	if (Change || player_->GetHP() <= 0) {
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}

	if (/*MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
		SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::title);
		SceneManager::SceneChange(SceneManager::SceneName::Game);
	}
	else if (/*MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
		SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::title);
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
			player_->HitHammerToEnemy(vec[i]);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::attack, 0.2f);
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

	sleep_->SetModel(sleepModel_[animeCount_]);
	sleep_->Initialize();
	if (titlepos_) {
		startpos_ = player_->Get();
		startpos_.z = startpos_.z + 3.f;
		player_->SetPos(startpos_);

		titlepos_ = false;
	}
	if (action_ == false) {
		if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
			action_ = true;
		}
	}
	else {
		timer_ += 0.1f;
		size_.x += 500.f;
		size_.y += 500.f;
		titleposition_.y -= 40;
		if (timer_ >= 1) {
			phase_ = Phase::Description;
		}
	}
	titlefilter_->SetSize(size_);
	for (int i = 0; i < 9; i++) {
		title_[i]->SetPosition(titleposition_);
	}
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
	if (LeftStickX != 0 || LeftStickY != 0) {
		movetimer_ += 0.1f;
	}

	stop_ = false;
	if (movetimer_ >= 50) {
		phase_ = Phase::Spown;
	}
}

void TutorialScene::SpownPhase()
{
	fighttextwindow_->Update();
	if (!fighttextwindow_->GetCloseWindow()) {
		description_ = 0;
		phase_ = Phase::Fight;
	}
}

void TutorialScene::FightPhase()
{
	notattack_ = false;
	stop_ = false;
	if (enemys_.size() == 0) {
		phase_ = Phase::Defeat;
	}

}

void TutorialScene::DefeatPhase()
{
	notlook_ = true;
}

void TutorialScene::FreePhase()
{
}