#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "SceneManager.h"
#include "PostEffect.h"


class ClearScene :
    public BaseScene
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
	//���n��
	std::unique_ptr<GameMap> map_;
	//�w�i�摜
	unique_ptr<Object3d> idol_;
	//����
	unique_ptr<Object3d>ground_;
	//���f��
	Model* idolModel_[4];

	Model* housemodel_;

	unique_ptr<Object3d> dome;

	unique_ptr<Object3d>house_[6];

	//�J�������W
	Vector3 cameraPos_;
	//�����_
	Vector3 targetPos_;

	Shake* shake_;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};

	IntermediateBase* ib_;
	SceneChangeEffect* schange;

	int baseNo = 0;

	//�v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_;
	//�v���C���[���f��
	Model* playerModel_[4];
	XMFLOAT3 idolPos_ = { 0.f,0.f,0.f };
	//�A�j���[�V�����J�E���g
	int32_t animeCount_ = 0;
	//�A�j���[�V�����X�s�[�h
	int32_t animeSpeed_ = 8;
	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�O�t���[���A�j���[�V�����J�E���g
	int32_t preAnimeCount_;
	//�A�j���[�V�����J�E���g
	int32_t idolanimeCount_ = 0;
	//�A�j���[�V�����X�s�[�h
	int32_t idolanimeSpeed_ = 8;
	//�A�j���[�V�����^�C�}�[
	int32_t idolanimeTimer_;
	//�O�t���[���A�j���[�V�����J�E���g
	int32_t idolpreAnimeCount_;
};

