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
	void Initialize(Vector3 pos);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(const Vector3& playerPos);

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
	bool GetIsHit(bool isHammerSwing);

	/// <summary>
	/// HP�擾
	/// </summary>
	/// <returns>HP</returns>
	int32_t GetHP() { return hp_; }

	/// <summary>
	/// ���W�擾
	/// </summary>
	/// <returns>���W</returns>
	Vector3 GetPos() { return deposit_->GetPosition(); }

	/// <summary>
	/// �}�b�v�̔ԍ����Z�b�g
	/// </summary>
	/// <returns></returns>
	void SetMapNum(int Num) { this->mapnum_=Num; }

	/// <summary>
	/// �}�b�v�̔ԍ����擾
	/// </summary>
	/// <returns></returns>
	int GetMapNum() { return mapnum_; }

private: //�����o�֐�
	/// <summary>
	/// �A���t�@�l�ݒ�
	/// </summary>
	/// <param name="playerPos"></param>
	void AlphaTest(const Vector3& playerPos);

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
	//�}�b�v�ԍ�
	int mapnum_;
};

