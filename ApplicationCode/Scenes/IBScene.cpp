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

	//3d�I�u�W�F�N�g������
	player_ = new Player;
	player_->Initialize();

	postEffectNo_ = PostEffect::NONE;

	map_ = make_unique<GameMap>();

	map_->Initalize(player_, cameraPos_, targetPos_, 0);


	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	ib_ = new IntermediateBase();
	ib_->Initialize();



	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
}

void IBScene::Update()
{

	player_->Update();
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	Vector3 enemyPos[3] = {};
	//�f�o�b�O�J�����ړ�����
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
	//HP�f�o�b�O����
	if (KeyInput::GetIns()->TriggerKey(DIK_O)) {
		player_->SubHP(1);
	}
	if (KeyInput::GetIns()->TriggerKey(DIK_R)) {
		player_->SetHP(3);
	}

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

	//�v���C���[��OBB�ݒ�
	XMFLOAT3 trans = { player_->GetHammer()->GetMatWorld().r[3].m128_f32[0],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[1],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[2]
	};
	OBB l_obb;
	l_obb.SetParam_Pos(trans);
	l_obb.SetParam_Rot(player_->GetHammer()->GetMatRot());
	l_obb.SetParam_Scl({ 1.0f,2.10f,10.0f });

	_hummmerObb = &l_obb;

	ib_->Update();
	ib_->FloorSave(1);

	//ib_->LoadFloor();
	shake_->Update();
	//colManager_->Update();
	//�V�[���؂�ւ�
	SceneChange();
}

void IBScene::Draw()
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
	player_->Draw();
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//�X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//�e�L�X�g�`��͈�

	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hp = std::to_wstring(player_->GetHP());
	text_->Draw("meiryo", "white", L"�Q�[���V�[��\n���N���b�N�܂���L�{�^���Ŏ��̊K�w��\nHP : " + hp, textDrawRange);
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//�|�X�g�G�t�F�N�g�`��
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//�|�X�g�G�t�F�N�g�������Ȃ��X�v���C�g�`�揈��(UI��)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}


void IBScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	safe_delete(player_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	//colManager_->Finalize();
	map_->Finalize();
}

void IBScene::SceneChange()
{
	bool Change = player_->GetNext();
	if (Change) {
		SceneManager::SceneChange(SceneManager::SceneName::Title);
	}
	if (MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
		SceneManager::SceneChange(SceneManager::SceneName::Boss);
	}
	else if (/*MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
		SceneManager::SceneChange(SceneManager::SceneName::Result);
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
