#include "TitleScene.h"
#include"TutorialScene.h"
#include <random>
#include "SoundManager.h"
#include "PadInput.h"

void TitleScene::Initialize()
{
	const Vector3 LB = { -1.0f, -1.0f, 0.0f };
	const Vector3 LT = { -1.0f, +1.0f, 0.0f };
	const Vector3 RB = { +1.0f, -1.0f, 0.0f };
	const Vector3 RT = { +1.0f, +1.0f, 0.0f };
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize(LT, LB, RT, RB);

	//カメラ初期化
	cameraPos_ = { 0, 8, 30 };
	targetPos_ = { 0, 0, 0 };

	camera_ = std::make_unique<Camera>();
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);

	//ライト初期化
	light_ = LightGroup::UniquePtrCreate();
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}
	//light->SetCircleShadowActive(0, true);
	Object3d::SetLight(light_.get());
	schange = new SceneChangeEffect();
	schange->Initialize();
	//3dオブジェクト初期化

	//SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::game, true, 0.1f);
}

void TitleScene::Update()
{
	camera_->SetEye(cameraPos_);
	light_->Update();
	schange->Change(0);
	//シーン切り替え
	SceneChange();
}

void TitleScene::Draw()
{
	//背景色
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(背景)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());

	Sprite::PostDraw();

	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Object3d::PostDraw();

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	schange->Draw();
	Sprite::PostDraw();

	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲
	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hx = std::to_wstring(schange->GetEnd());
	text_->Draw("meiryo", "white", L"タイトルシーン\n左クリックまたはLボタンでリザルトシーン\n右クリックまたはRボタンでゲームシーン" + hx, textDrawRange);
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);
	//ポストエフェクトを描けないスプライト描画
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());

	Sprite::PostDraw();

	DirectXSetting::GetIns()->PostDraw();
}


void TitleScene::Finalize()
{
	safe_delete(text_);
	//colManager_->Finalize();
}

void TitleScene::SceneChange()
{
	if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
		schange->SetFadeNum(0);
		schange->SetFStart(true);
	}
	if (schange->GetEnd() == true) {
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}

}
