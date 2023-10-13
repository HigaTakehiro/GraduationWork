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

	/// <summary>
	/// ��]�p�擾
	/// </summary>
	/// <returns>��]�p</returns>
	Vector3 GetRot() { return rot_; }

	/// <summary>
	/// �n���}�[�̃p���[���グ��
	/// </summary>
	void AddHammerPower() { hammerPower_++; }

	/// <summary>
	/// �����ݒ�
	/// </summary>
	/// <param name="vec"></param>
	void HitHammerToEnemy(Vector3 vec);

	/// <summary>
	/// �n���}�[�����t���O���擾
	/// </summary>
	/// <returns>�n���}�[�����t���O</returns>
	bool GetIsHammerRelease() { return isHammerRelease_; }

	/// <summary>
	/// �U���t���O���擾
	/// </summary>
	/// <returns>�U���t���O</returns>
	bool GetIsAttack() { return isAttack_; }

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

	/// <summary>
	/// �n���}�[����
	/// </summary>
	void HammerGet();

	/// <summary>
	/// �n���}�[�߂菈��
	/// </summary>
	void HammerReturn();

	/// <summary>
	/// �n���}�[��������
	/// </summary>
	void HammerPowerUp();

	/// <summary>
	/// ��������
	/// </summary>
	void Repulsion();

private: //�����o�ϐ�
	//���W
	Vector3 pos_;
	//�傫��
	Vector3 scale_;
	//��]�p
	Vector3 rot_;
	//HP
	int32_t hp_;
	//�n���}�[�p���[
	int32_t hammerPower_;

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
	//�ō��ړ����x
	float maxMoveSpeed_;
	//�����x
	Vector3 acc_;
	//�n���}�[�U���������x
	float hammerAcc_;

	//���n���}�[��������
	int32_t hammerTime = 60;
	int32_t hammerTimer = 0;

	//�v���C���[�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_;
	//�v���C���[���f��
	Model* playerModel_[4];

	//�n���}�[�I�u�W�F�N�g
	std::unique_ptr<Object3d> hammer_;
	//�n���}�[���f��
	Model* hammerModel_;
	//�n���}�[�����t���O
	bool isHammerRelease_;
	//�U���t���O
	bool isAttack_;
	//�n���}�[���W
	Vector3 hammerPos_;
	//�n���}�[�T�C�Y
	Vector3 hammerSize_;
	//�n���}�[�������x
	float throwSpeed_;
	//�n���}�[�����x�N�g��
	Vector3 hammerVec_;
	//�n���}�[�����l
	Vector3 initHammerPos_;
	Vector3 initHammerScale_;
	Vector3 initHammerRot_;

	//���I�u�W�F�N�g
	std::unique_ptr<Object3d> arrow_;
	//��󃂃f��
	Model* arrowModel_;

	//�n���}�[�������p�x
	float hammerThrowRot_;

	//�����U���x�N�g��
	Vector3 repulsionVec_;
	//�����U�����x
	float repulsionSpeed_;

public:
	Object3d* GetHammer() { return hammer_.get(); }
	DirectX::XMFLOAT3 GetPos(){ return player_->GetPosition(); }
};

