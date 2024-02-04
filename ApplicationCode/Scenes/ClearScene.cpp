#include "ClearScene.h"

void ClearScene::Initialize()
{
	//ポストエフェクト初期化
//画面大きさ設定
	const Vector3 LB = { -1.0f, -1.0f, 0.0f };
	const Vector3 LT = { -1.0f, +1.0f, 0.0f };
	const Vector3 RB = { +1.0f, -1.0f, 0.0f };
	const Vector3 RT = { +1.0f, +1.0f, 0.0f };
	if (postEffect_ == nullptr) {
		postEffect_ = std::make_unique<PostEffect>();
	}
	postEffect_->Initialize(LT, LB, RT, RB);
	//カメラ初期化
	CameraSetting();
	//ライト初期化
	if (light_ == nullptr) {
		light_ = LightGroup::UniquePtrCreate();
	}
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}
	Object3d::SetLight(light_.get());
	//プレイヤー初期化
	for (int32_t i = 0; i < 4; i++) {
		idolModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_idle.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 2.0f }, { 128.0f, 128.0f });
	}

	dome = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("skydome"));
	dome->Initialize();
	dome->SetRotation({ 0.0f,90.f,0.0f });

	idol_ = Object3d::UniquePtrCreate(idolModel_[0]);
	idol_->SetIsBillboardY(true);
	idol_->SetObbScl({ 2.f,4.f,2.f });
	idol_->SetHitRadius(0.5f);
	idol_->SetScale({ 0.035f, 0.035f, 0.035f });
	idol_->SetPosition(idolPos_);

	ground_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
	ground_->SetPosition({ 0.f,-2.f,0.f });

	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFEnd(true);
	schange->SetFadeNum(1);
}

void ClearScene::Update()
{
	schange->Change(0);
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	Animation();
	dome->Update();
	ground_->Update();
}

void ClearScene::Draw()
{
	//背景色
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };
	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	dome->Draw();
	ground_->Draw();
	idol_->Draw();
	Object3d::PostDraw();
	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());
	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);
	DirectXSetting::GetIns()->PostDraw();
}

void ClearScene::Finalize()
{
}

void ClearScene::SceneChange()
{
}

void ClearScene::CameraSetting()
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

void ClearScene::Animation()
{
	//タイマーカウント
	if (++animeTimer_ >= animeSpeed_) {
		if (++animeCount_ >= 4) {
			animeCount_ = 0;
		}
		animeTimer_ = 0;
	}


	if (preAnimeCount_ == animeCount_)return;
	idol_->SetModel(idolModel_[animeCount_]);
	idol_->Initialize();
	idol_->Update();
	
	preAnimeCount_ = animeCount_;
}
