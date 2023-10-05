#pragma once
#include "Object3d.h"
#include "Vector3.h"

class Player
{
public: //�����o�֐�
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
	/// �I������
	/// </summary>
	void Finalize();

private: //�����o�֐�

	/// <summary>
	/// �v���C���[�X�e�[�^�X�̐ݒ�
	/// </summary>
	void PlayerStatusSetting();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �n���}�[����
	/// </summary>
	void HammerThrow();

private: //�����o�ϐ�
	//���W
	Vector3 pos_;
	//�傫��
	Vector3 scale_;
	//��]�p
	Vector3 rot_;

	//�ړ����x
	float moveSpeed_;
	//��]���x
	float rotSpeed_;
	//��]���Z�b�g�^�C�}�[
	float rotResetTimer_;
	//��]���Z�b�g����
	float rotResetTime_;
	//������]�p
	Vector3 initRot_;

	//�v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_;
	//�v���C���[���f��
	Model* playerModel_[5];

	//�n���}�[�I�u�W�F�N�g
	std::unique_ptr<Object3d> hammer_;
	//�n���}�[���f��
	Model* hammerModel_;
	//�n���}�[�����t���O
	bool isHammerRelease_;
	//�n���}�[�������p�x
	float hammerThrowRot_;


public:
	Object3d* GetHammer() { return hammer_.get(); }
	DirectX::XMFLOAT3 GetPos(){ return player_->GetPosition(); }
};

