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
	&TutorialScene::DescriptionPhase,
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

	unsigned int EnemySize = 3;

	enemys_.resize(EnemySize);
	vec.resize(EnemySize);

	for (size_t i = 0; i < enemys_.size(); i++) {
		enemys_[i] = new NormalEnemyA();
		enemys_[i]->Init();
		enemys_[i]->SetPlayerIns(player_);
	}
	enemys_[0]->SetPos(Vector3(10, -30, 10));
	enemys_[2]->SetPos(Vector3(-15, -30, -5));
	enemys_[2]->SetPos(Vector3(0, -30, -5));

	map_ = make_unique<GameMap>();
	map_->Initalize(player_, cameraPos_, targetPos_, 0);

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });

	titlefilter_=Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::filter, { WinApp::window_width/2, WinApp::window_height/2 }, { 0.f, 0.f, 0.f, 1.0f }, { 0.5f, 0.5f });

	Vector3 StartPos = player_->GetPos();
	StartPos.z = StartPos.z + 15.f;
	player_->SetPos(StartPos);
	Vector3 startPos = player_->GetPos();

}

void TutorialScene::Update()
{
	(this->*FuncTable[phase_])();
	if (shake_->GetShakeFlag() == true) {
		cameraPos_.y += shake_->GetShakePos();
		targetPos_.y += shake_->GetShakePos();
	} else {
		cameraPos_.y = 12;
		targetPos_.y = 0;
	}
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_,true);

	player_->Update();
	if (phase_ == Phase::Title) { return; }
	shake_->Update();
	colManager_->Update();
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
	}
	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	/*for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			ore->Draw();
		}
	}*/
	//boss_->Draw();
	//boss_->Draw2();
	player_->Draw();
	map_->BridgeDraw(notlook_);
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	titlefilter_->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲

	D2D1_RECT_F textDrawRange = { 600, 0, 1280, 1280 };
	text_->Draw("meiryo", "white", L"チュートリアルシーン\n左クリックまたはLボタンでタイトルシーン\n右クリックまたはRボタンでリザルトシーン\nシェイクはEnter", textDrawRange);

	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);
	if (phase_ != Phase::Title) {
		//ポストエフェクトをかけないスプライト描画処理(UI等)
		Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
		player_->SpriteDraw();
		Sprite::PostDraw();
	}
	DirectXSetting::GetIns()->PostDraw();
}

void TutorialScene::Finalize()
{
}

void TutorialScene::SceneChange()
{
	bool Change = player_->GetNext();
	if (Change) {
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}
	if (/*MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
		SceneManager::SceneChange(SceneManager::SceneName::Title);
	}
	else if (/*MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
		SceneManager::SceneChange(SceneManager::SceneName::Result);
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

void TutorialScene::TitlePhase()
{
	

	if (action_ == false) {
		if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
			action_ = true;
		}
	}
	else {
		timer_ += 0.1f;
		if (timer_ >= 1) {
			phase_ = Phase::Description;
		}
	}
}

void TutorialScene::DescriptionPhase()
{
	if (PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
		description_ += 1;
	}


	if (description_ == 5) {
		description_ = 0;
		phase_ = Phase::Move;
	}
}

void TutorialScene::MovePhase()
{
}

void TutorialScene::SpownPhase()
{
	if (PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
		description_ += 1;
	}


	if (description_ == 5) {
		description_ = 0;
		phase_ = Phase::Move;
	}
}

void TutorialScene::FightPhase()
{
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	Vector3 enemyPos[3] = {};
	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetHP() <= 0)
		{
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
}

void TutorialScene::DefeatPhase()
{
}

void TutorialScene::FreePhase()
{
}
