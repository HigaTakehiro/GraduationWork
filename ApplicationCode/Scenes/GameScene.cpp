#include "GameScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include"NormalEnemyA.h"
void GameScene::Initialize()
{
	ShowCursor(true);
	//�|�X�g�G�t�F�N�g������
	//��ʑ傫���ݒ�
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

	//3d�I�u�W�F�N�g������
	ground_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
	ground_->SetScale({ 0.2f, 0.2f, 0.2f });
	player_ = new Player;
	player_->Initialize();

	postEffectNo_ = PostEffect::NONE;

	ene = new NormalEnemyA();
	ene->Init();
}

void GameScene::Update()
{
	ground_->Update();
	player_->Update();

	if (KeyInput::GetIns()->HoldKey(DIK_W)) {
		cameraPos_.z += 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_S)) {
		cameraPos_.z -= 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_A)) {
		cameraPos_.x += 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_D)) {
		cameraPos_.x -= 1.0f;
	}

	camera_->SetEye(cameraPos_);
	light_->Update();
	ene->Upda(camera_.get());
	//�V�[���؂�ւ�
	SceneChange();
}

void GameScene::Draw()
{
	//�w�i�F
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(�w�i)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();

	//3D�I�u�W�F�N�g�`�揈��
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	ground_->Draw();
	player_->Draw();
	Object3d::PostDraw();

	ene->Draw();
	//�X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//�e�L�X�g�`��͈�
	D2D1_RECT_F textDrawRange = { 0, 0, 500, 500 };
	text_->Draw("meiryo", "white", L"�Q�[���V�[��\n���N���b�N�Ń^�C�g���V�[��\n�E�N���b�N�Ń��U���g�V�[��", textDrawRange);
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//�|�X�g�G�t�F�N�g�`��
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//�|�X�g�G�t�F�N�g�������Ȃ��X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}


void GameScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	safe_delete(player_);
}

void GameScene::SceneChange()
{
	if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK)) {
		SceneManager::SceneChange(SceneManager::SceneName::Title);
	}
	else if (MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK)) {
		//SceneManager::SceneChange(SceneManager::SceneName::Result);
	}
}