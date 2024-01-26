#pragma once
#include "Object3d.h"
#include "Model.h"
#include "ParticleManager.h"
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
	void Initialize(Vector3 pos,bool isDetroy=FALSE,Camera*camera=nullptr);

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

private:
	//
	bool isDestroy = FALSE;
	//�j��t���O
	bool destroyF_ = FALSE;
	//�j��G�t�F�N�g
	std::unique_ptr<ParticleManager>desParticle_;
	//�p�[�e�B�N���p
	int32_t particleCreateCool_;//���錫���������ł����Ȃ��ƎQ�Ƃ������������s���ĕ��G������
	Camera* camera_ = nullptr;
	Vector3 parPos_ = {};
	bool BossDestroy;//�z�΂̐F
	DirectX::XMFLOAT4 color_={1,1,1,1};
public:

	void SetDestroyBoss(bool f) { BossDestroy = f; }
	void SetCameraIns(Camera* ins) { camera_ = ins; }
	void SetDestroyF(bool f) { /* if(particleCreateCool_ > 60)*/ { destroyF_ = f;/* particleCreateCool_ = 0;*/ } }
	void SetParPos(Vector3 pos) { parPos_ = pos; }
	float GetDepositAlpha()const { return color_.w; }
	void DestroyEffect();
	void ParticleDraw();
};

