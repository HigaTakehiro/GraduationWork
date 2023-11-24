#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Deposit
{
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Deposit();

	/// <summary>
	/// ������
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
	/// �z�΃h���b�v����
	/// </summary>
	/// <returns></returns>
	Vector3 OreDropVec();

	/// <summary>
	/// �z���q�b�g����擾
	/// </summary>
	/// <returns>�q�b�g����</returns>
	bool GetIsHit();

	/// <summary>
	/// HP�擾
	/// </summary>
	/// <returns>HP</returns>
	int32_t GetHP() { return hp_; }

private: //�����o�ϐ�
	//�z���I�u�W�F�N�g
	std::unique_ptr<Object3d> deposit_;
	//�z�����f��
	Model* model_;
	//HP
	int32_t hp_;
	//���G����
	int32_t hitCoolTime_;
	//���G���ԃ^�C�}�[
	int32_t hitCoolTimer_;
	
};

