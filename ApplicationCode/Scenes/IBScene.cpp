#include "IBScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"

void IBScene::Initialize()
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
	for (int32_t i = 0; i < 4; i++) {
		playerModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_rest.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f }, true);
	}
	fireModel_ = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "fire.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * 0, 0.0f }, { 128.0f, 128.0f }, true);
	player_ = Object3d::UniquePtrCreate(playerModel_[0]);
	player_->SetIsBillboardY(true);
	player_->SetScale({ 0.15f, 0.15f, 0.15f });
	fire_ = Object3d::UniquePtrCreate(fireModel_);
	fire_->SetIsBillboardY(true);
	fire_->SetScale({ 0.1f, 0.1f, 0.1f });

	skillB_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillButton, { 1000, 50 }, { 1,1,1,1 }, { 0.0f, 0.0f });
	susumu_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::susumuButton, { 1000, 150 }, { 1,1,1,1 }, { 0.0f, 0.0f });
	arrow = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::Arrow, { 900, 50 }, { 1,1,1,1 }, { 0.0f, 0.0f });

	postEffectNo_ = PostEffect::NONE;

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	ib_ = new IntermediateBase();
	ib_->Initialize();
	ib_->LoadFloor();

	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFEnd(true);
	schange->SetFadeNum(1);

	baseNo = ib_->GetBaseNo();
	animeTimer_ = 0;
	preAnimeCount_ = 999;
	animeSpeed_ = 8;
	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
}

void IBScene::Update()
{
	Animation();
	player_->SetPosition({ -8.0f,2.5f, 8.0f });
	player_->Update();
	fire_->SetPosition({ 0.0f,2.5f, 8.0f });
	fire_->Update();
;	//デバッグカメラ移動処理
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
	//HPデバッグ処理

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



	if (hp_ != 0) {
		if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
			baseNo++;
		}
	}

	ib_->Update();
	ib_->FloorSave(baseNo);

	skillB_->SetSize({ 256, 64 });
	//skillSprite_->SetSize({ 1280, 720 });
	susumu_->SetSize({ 256, 64 });
	if (count2 < 6) {
		count2++;
	}
	if (count2 == 6) {
		if (count < 4) {
			count++;
		}
		else {
			count = 0;
		}
		count2 = 0;
	}
	schange->Change(0);

	arrow->SetTextureRect({ 0 + 64 * count,0 }, { 64,64 });
	arrow->SetSize({ 64,64 });
	//shake_->Update();
	//colManager_->Update();
	//シーン切り替え
	SceneChange();
}

void IBScene::Draw()
{
	//背景色
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(背景)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	background_->Draw();
	Sprite::PostDraw();
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	ib_->Draw();
	Object3d::PostDraw();
	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	player_->Draw();
	fire_->Draw();
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());

	skillB_->Draw();
	susumu_->Draw();
	arrow->Draw();

	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲

	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hx = std::to_wstring(schange->GetEnd());
	text_->Draw("meiryo", "white", L"中間拠点シーン\n左クリックまたはLボタンで次の階層へ\n" + hx, textDrawRange);
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//ポストエフェクトをかけないスプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}


void IBScene::Finalize()
{
	safe_delete(text_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	//colManager_->Finalize();
}

void IBScene::SceneChange()
{
	if (KeyInput::GetIns()->TriggerKey(DIK_UPARROW)) {
		arrow->SetPosition({ 900,50 });
	}
	else if (KeyInput::GetIns()->TriggerKey(DIK_DOWNARROW)) {
		arrow->SetPosition({ 900,150 });
	}
	float leftStick = PadInput::GetIns()->leftStickY();
	if (leftStick > 0) {
		arrow->SetPosition({ 900,150 });
	}
	if (leftStick < 0) {
		arrow->SetPosition({ 900,50 });
	}
	if (arrow->GetPosition().y == 150) {
		if (KeyInput::GetIns()->TriggerKey(DIK_RETURN) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
			//schange->SetFEnd(false);
			schange->SetFStart(true);
			schange->SetFadeNum(0);
		}
		if (schange->GetEnd() == true) {
			schange->SetFStart(false);
			schange->SetFadeNum(0);
			SceneManager::SceneChange(SceneManager::SceneName::Boss);
		}
	}
	else if (arrow->GetPosition().y == 50) {
		if (KeyInput::GetIns()->TriggerKey(DIK_RETURN) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
				schange->SetFEnd(false);
				schange->SetFStart(true);
				schange->SetFadeNum(0);
		}
		if (schange->GetEnd() == true) {
			schange->SetFStart(false);
			schange->SetFadeNum(0);
			SceneManager::SceneChange(SceneManager::SceneName::SKILL);
		}
	}

}

void IBScene::CameraSetting()
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

void IBScene::Animation()
{
	//タイマーカウント
	if (++animeTimer_ >= animeSpeed_) {
		if (++animeCount_ >= 4) {
			animeCount_ = 0;
		}
		animeTimer_ = 0;
	}

	if (preAnimeCount_ == animeCount_) return;
	player_->SetModel(playerModel_[animeCount_]);
	player_->Initialize();

	preAnimeCount_ = animeCount_;
}

