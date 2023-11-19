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

	//�J����������
	cameraPos_ = { 0, 8, 30 };
	targetPos_ = { 0, 0, 0 };

	camera_ = std::make_unique<Camera>();
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);

	//���C�g������
	light_ = LightGroup::UniquePtrCreate();
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}
	//light->SetCircleShadowActive(0, true);
	Object3d::SetLight(light_.get());
	scange = new SceneChangeEffect();
	scange->Initialize();
	//3d�I�u�W�F�N�g������

	//SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::game, true, 0.1f);
	sceneFlag = false;
}

void TitleScene::Update()
{
	camera_->SetEye(cameraPos_);
	light_->Update();
	scange->Change(0);
	//�V�[���؂�ւ�
	SceneChange();
}

void TitleScene::Draw()
{
	//�w�i�F
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(�w�i)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());

	Sprite::PostDraw();

	//3D�I�u�W�F�N�g�`�揈��
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Object3d::PostDraw();

	//�X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	scange->Draw();
	Sprite::PostDraw();

	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//�e�L�X�g�`��͈�
	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hx = std::to_wstring(scange->GetEnd());
	text_->Draw("meiryo", "white", L"�^�C�g���V�[��\n���N���b�N�܂���L�{�^���Ń��U���g�V�[��\n�E�N���b�N�܂���R�{�^���ŃQ�[���V�[��" + hx, textDrawRange);
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);
	//�|�X�g�G�t�F�N�g��`���Ȃ��X�v���C�g�`��
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
		sceneFlag = false;
		scange->SetStart(true);
	}
	else if (MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
		sceneFlag = true;
		scange->SetStart(true);
	}
	if (scange->GetEnd() == true) {
		if (sceneFlag == true) {
			SceneManager::SceneChange(SceneManager::SceneName::Game);
		}
		else {
			SceneManager::SceneChange(SceneManager::SceneName::IB);
		}
	}

}
