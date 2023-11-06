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
	/// �z�Ύ擾����
	/// </summary>
	void AddOreCount() { oreCount_++; }

	/// <summary>
	/// �����ݒ�
	/// </summary>
	/// <param name="vec"></param>
	void HitHammerToEnemy(Vector3 vec);

	void SetPos(Vector3 pos) { this->pos_ = pos; }

	void SetStop(bool stop) { this->stop_ = stop; }

	bool GetStop() { return stop_; }

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

	/// <summary>
	/// �n���}�[��]�U���t���O���擾
	/// </summary>
	/// <returns></returns>
	bool GetIsHammerSwing() { return isHammerSwing_; }

	bool GetNotNext() { return notnext_; }

	/// <summary>
	/// �z�Ύ擾�����擾
	/// </summary>
	/// <returns>�z�Ύ擾����</returns>
	float GetOreCountRate() { return (float)oreCount_ / (float)maxOreCount_; }

	/// <summary>
	/// HP���擾
	/// </summary>
	/// <returns>HP</returns>
	int32_t GetHP() { return hp_; }

	/// <summary>
	/// HP���Z�b�g
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHP(int32_t hp) { hp_ = hp; }

	/// <summary>
	/// HP�����Z
	/// </summary>
	/// <param name="addHP">���Z����HP</param>
	void AddHP(int32_t addHP) { hp_ += addHP; }

	/// <summary>
	/// HP�����Z
	/// </summary>
	/// <param name="subHP">���Z����HP</param>
	void SubHP(int32_t subHP) { hp_ -= subHP; }

	void SetNextFlor(bool flag) { nextflor_ = flag; }

	bool GetNextFlor() { return nextflor_; }

	bool GetNext() { return next_; }


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
	//�z�Ύ擾��
	int32_t oreCount_;
	//�z�΍ő�擾��
	int32_t maxOreCount_;
	//�U����
	int32_t attackPoint_;
	//�z�Ύ擾���Ƃɑ傫������T�C�Y
	Vector3 hammerSizeUp_;
	//�z�Ύ擾���Ƃ̉�]���x�W��
	float hammerRotCoeff_;

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
	//������]���x
	float initRotSpeed_;
	//�����U����
	int32_t initAtkPoint_;
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
	//�ʏ펞
	Model* playerModel_[4];
	//�ړ����f��
	//�O
	Model* frontMoveModel_[4];
	//��
	Model* backMoveModel_[4];
	//�E
	Model* rightMoveModel_[4];
	//��
	Model* leftMoveModel_[4];

	//�n���}�[�I�u�W�F�N�g
	std::unique_ptr<Object3d> hammer_;
	//�n���}�[���f��
	Model* hammerModel_;
	//�n���}�[�����t���O
	bool isHammerRelease_;
	//�U���t���O
	bool isAttack_;
	//�n���}�[��]�t���O
	bool isHammerSwing_;
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
	//�A�j���[�V�����J�E���g
	int32_t animeCount_;

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
	//�����W��
	float repulsionPower_;
	//������~�߂�
	bool stop_ = false;
	//�U�����ɃX�e�[�W�ړ����Ȃ��l��
	bool notnext_ = false;
	//�K�i�̃`�F�b�N
	bool nextflor_ = false;

	bool next_ = false;

public:
	Object3d* GetHammer() { return hammer_.get(); }
	DirectX::XMFLOAT3 GetPos() { return player_->GetPosition(); }
	bool GetisHit() { return player_->GetIsHit(); }
	bool getisHammerActive() {
		if (isHammerRelease_ || isHammerSwing_)return true; return false;
	}
};

