#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Grass
{
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Grass();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3& pos,int I);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(const Vector3& playerPos);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private: //�����o�֐�
	/// <summary>
	/// �A���t�@�l�ݒ�
	/// </summary>
	/// <param name="playerPos">�v���C���[���W</param>
	void AlphaTest(const Vector3& playerPos);

private: //�����o�ϐ�
	//�I�u�W�F�N�g
	Object3d* grass_;
	//���f��
	Model* model_[3];

	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�A�j������
	int32_t animeTime_;
	//�A�j���J�E���g
	int32_t animeCount_;
};

