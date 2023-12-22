#pragma once
#include "BaseScene.h"
#include "PostEffect.h"
#include "SafeDelete.h"
#include "MouseInput.h"
#include "Object3d.h"
#include "Vector3.h"
#include "Camera.h"
#include "MouseInput.h"
#include "Easing.h"
#include "Shapes.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "DebugText.h"
#include "MotionMath.h"
#include "SceneManager.h"
#include "TextDraw.h"
#include "JsonLoader.h"
#include "Sprite.h"
#include"GameMap.h"
#include "Ore.h"
#include"BossBase.h"
#include"BaseEnemy.h"
#include"Shake.h"
#include"IntermediateBase.h"
#include "SceneChange.h"
#include "Player.h"

class IBScene :public BaseScene
{
public: //�����o�֐�
	/// <summary>
	/// �������֐�
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

private: //�����o�֐�
	/// <summary>
	/// �V�[���؂�ւ�����
	/// </summary>
	void SceneChange();

	/// <summary>
	/// �J�����������ݒ�
	/// </summary>
	void CameraSetting();
	/// <summary>
	/// �A�j���[�V��������
	/// </summary>
	void Animation();

	/// <summary>
	/// UI�X�V����
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// �X�L����ʍX�V����
	/// </summary>
	void SkillUIUpdate();
private: //�����o�ϐ�
	//�|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect_;
	//�|�X�g�G�t�F�N�g�ԍ�
	PostEffect::PostEffectNo postEffectNo_;
	//���C�g
	std::unique_ptr<LightGroup> light_;
	//�J����
	std::unique_ptr<Camera> camera_;
	//�e�L�X�g�`��
	TextDraw* text_;
	//���n��
	std::unique_ptr<GameMap> map_;
	//�w�i�摜
	std::unique_ptr<Sprite> background_;
	//�J�������W
	Vector3 cameraPos_;
	//�����_
	Vector3 targetPos_;

	Shake* shake_;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};

	IntermediateBase* ib_;

	int baseNo = 0;

	//�v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_;
	//�v���C���[���f��
	Model* playerModel_[4];
	//�v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> fire_;
	//�v���C���[���f��
	Model* fireModel_;
	std::unique_ptr<Sprite> susumu_;
	std::unique_ptr<Sprite> skillSprite_;
	std::unique_ptr<Sprite> skillB_;
	std::unique_ptr<Sprite> arrow;

	//�A�j���[�V�����J�E���g
	int32_t animeCount_;
	//�A�j���[�V�����X�s�[�h
	int32_t animeSpeed_;
	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�O�t���[���A�j���[�V�����J�E���g
	int32_t preAnimeCount_;
	int32_t hp_ = 0;
	float count = 0;
	float count2 = 2;
	float soundCount;
	int32_t baseCount = 0;
	SceneChangeEffect* schange;
	Player* playerUI_;
	bool skillFlag = false;
	int32_t skillCount = 0;
	int32_t skillCount2 = 0;

	//�X�L����ʃX�v���C�g
	//�E�B���h�EUI�X�v���C�g
	std::unique_ptr<Sprite> window_[3];
	//�X�L����ʃv���C���[�X�v���C�g
	std::unique_ptr<Sprite> skillPlayer_[4];
	//�X�L���p�l���X�v���C�g
	std::unique_ptr<Sprite> skillPanel_[13];
	//�X�L����ʃJ�[�\���X�v���C�g
	std::unique_ptr<Sprite> skillCursor_;
};

