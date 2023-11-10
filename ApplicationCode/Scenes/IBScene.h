#pragma once
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
#include "Player.h"
#include"GameMap.h"
#include "Ore.h"
#include"BossBase.h"
#include"BaseEnemy.h"
#include"Shake.h"
#include"IntermediateBase.h"
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

protected: //�ÓI�����o�֐�

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

	int baseNo;

	//�v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_;
	//�v���C���[���f��
	Model* playerModel_[4];

	//�A�j���[�V�����J�E���g
	int32_t animeCount_;
	//�A�j���[�V�����X�s�[�h
	int32_t animeSpeed_;
	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�O�t���[���A�j���[�V�����J�E���g
	int32_t preAnimeCount_;
};

