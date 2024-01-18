#pragma once
#include"Player.h"
#include <array>
#include <intsafe.h>
#include<DirectXMath.h>
#include "BaseCollision.h"
#include"Vector3.h"
#include"Spline.h"
using namespace DirectX;
class TogemaruAct
{
private:
	//��΂��j�p
	static constexpr UINT spearSize = 8;

	std::array<Vector3, spearSize>SpearPos_ = {};
	std::array<float, spearSize>SpearAngle_ = {};
	//�j�̃A���t�@�l
	std::array<float,spearSize>spearsAlpha ={};
	//���̔��˔͈�
	float ShotRange = 0.f;
	//���˃t���O
	bool isShot = FALSE;
	//
	void ResetParam_Spear();

private:
	//�����蔻�����
	void CollideDeposit();
	//
	bool depositCollideF = FALSE;
	//
	int32_t depositDelTime = 0;
	//
	Vector3 DepositReproduction();
public:
	//�z�΍��W
	static Vector3 depositPos;
	//
	static bool depositDelF;
public:
	//�A�j���[�V�������O
	enum AnimeName
	{
		IdlE,
		WALK_FRONT,
		WALK_RIGHT,
		WALK_LEFT,
		WALK_BACK,
		ROLE,
		CRUSH_FRONT,
		CRUSH_RIGHT,
		CRUSH_LEFT,
		CRUSH_BACK
	}anime_name_;

	void CrushAnimation();
	void WalkAnimation();
	AnimeName GetName()const { return anime_name_; }
private:
	//�ʏ�p�����[�^
	Vector3 Pos_,Rot_,Scl_={};
	//�퓬�J�n
	bool beginBattle = FALSE;
	//�v���C���[�C���X�^���X
	Player* Player_ = nullptr;
	//�U���C���^�[�o��
	uint32_t actionCount = 1;
	//�s�����X�g
	enum Act
	{
		MOVE,
		ATTACK_RUSH,
		ATTACK_SHOTSPEAR,
		RUNAWAY,
		DEATH
	}act_;


	bool Appear();
	//-----------------
	/* �֐��e�[�u�� */
	typedef struct
	{
		//��Ԕԍ�
		int state=0;
		//�e��Ԃ̊֐�
		void(TogemaruAct::*func)();
	}AppearState;

	//�ԍ��p
	enum StateName
	{
		P1=0,
		P2=1,
		P3=2,
		P_Num=3
	};

	void StateExecute(int state);
	float CameraEaseT, PosEaseT;
	void Phase1();
	void Phase2();
	void Phase3();
	bool beforeBattle = FALSE;
	static AppearState StateArray[StateName::P_Num];

	StateName cameraStateIndex;
	void Camerawork();
	//-----------------

	void Move();
	void Attack_Rush();
	void Attack_ShotSpear();
	void RunAway();
	void Death();

	//�֐��|�C���^
	static void (TogemaruAct::*ActionList[])();

//�ړ�
private:
	float Walk();//�ʏ����
	float Follow();//�Ǐ]

	//�ړ��X�s�[�h
	float movSpeed = 0.1f;
	//���������Ɉړ�����
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot ={};

	//��]�p�̃A���O��
	float angle_ = 0.f;
	//
	float oldRotY_;
	//
	float rotEaseT = 0.f;
	//�ǐՂ��邩
	bool isFollow = FALSE;
	//
	int32_t animationWaitTime = 0;
	//�ːi�p�ϐ�
	float rushEaseT=0.f;
	//�ːi�I����Đj�o���܂ł̑҂�����
	int32_t waitShotCount = 0;
	Vector3 RushStartPos = {};

	bool RoleF;
//�_���[�W��������Ƃ�
private:
	bool isSpearCrush=FALSE;
	//�j�󂵂����̐�
	int32_t crushSpearNum = 0;
	//���̍Đ�����
	int32_t reproductionTime = 0;
	//RunAway()����
	bool CrushSpear();

	bool CollideSpear();

	void RushGround360();
	Spline* spline;
	std::vector<XMFLOAT3>SplinePosList;
	float splineT;
	XMFLOAT3 BefoSplinePos;
	XMFLOAT3 SplineAfterPos;
	float RandM_P = 1;
		float CurreRandVal[6] = {};

		int ActionRandVal=0;
//�V�F�C�N
private:
	bool shakeF = FALSE;
	float shakeXVal=0.f, shakeYVal=0.f;
	float shakeT = 0.f;
public:
	//�s���J��
	void Transition();
	//��ʐU��(�z�΂ɂԂ�������)
	static Vector3 cameraPos;//�J�������W
	static Vector3 oldCameraPos;
	static Vector3 DefaultPos;
	void ViewShake();

	bool GetRole()const { return RoleF; }
public:
	// �Q�b�^�[ //
	Vector3 GetPos()const { return Pos_; }
	Vector3 GetRot()const { return Rot_; }
	Vector3 GetScl()const { return Scl_; }

	Vector3 GetSpearPos(int index)const { return SpearPos_[index]; }

	//�z�΍��W
	Vector3 GetDepositPos()const { return depositPos; }

	int32_t GetCrushSpearNum()const { return crushSpearNum; }

	float GetSpearAlpha(int index)const { return spearsAlpha[index]; }
	
	//
	bool GetDepositDelF()const { return depositDelF; }

	// �Z�b�^�[ //
	void SetPlayerIns(Player* player) { Player_ = player; }

	void SetDepositPos(Vector3 pos) { depositPos = pos; }

	float GetRotY()const { return Rot_.y; }
};

