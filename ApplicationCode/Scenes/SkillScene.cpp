#include "SkillScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"

void SkillScene::Initialize()
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
	CameraSetting();
	oldcamerapos_ = cameraPos_.z;
	//���C�g������
	light_ = LightGroup::UniquePtrCreate();
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}
	//light->SetCircleShadowActive(0, true);
	Object3d::SetLight(light_.get());


	skillSprite_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skill, { 0, 0 }, { 1,1,1,1 }, { 0.0f, 0.0f });

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

void SkillScene::Update()
{
	
	schange->Change(0);
	//shake_->Update();
	//colManager_->Update();
	//�V�[���؂�ւ�
	SceneChange();
}

void SkillScene::Draw()
{
	//�w�i�F
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(�w�i)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	background_->Draw();
	Sprite::PostDraw();
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	ib_->Draw();
	Object3d::PostDraw();
	//3D�I�u�W�F�N�g�`�揈��
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
		skillSprite_->Draw();
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//�e�L�X�g�`��͈�

	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hx = std::to_wstring(schange->GetEnd());
	text_->Draw("meiryo", "white", L"���ԋ��_�V�[��\n���N���b�N�܂���L�{�^���Ŏ��̊K�w��\n" + hx, textDrawRange);
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//�|�X�g�G�t�F�N�g�`��
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//�|�X�g�G�t�F�N�g�������Ȃ��X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}


void SkillScene::Finalize()
{
	safe_delete(text_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	//colManager_->Finalize();
}

void SkillScene::SceneChange()
{
	if (KeyInput::GetIns()->TriggerKey(DIK_RETURN) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
		schange->SetFEnd(false);
		schange->SetFStart(true);
		schange->SetFadeNum(0);
	}
	if (schange->GetEnd() == true) {
		schange->SetFStart(false);
		schange->SetFadeNum(0);
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}
}

void SkillScene::CameraSetting()
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

void SkillScene::Animation()
{
	//�^�C�}�[�J�E���g
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

