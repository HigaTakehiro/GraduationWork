#pragma once
#include "Vector3.h"
#include<DirectXMath.h>
#include"Player.h"
using namespace DirectX;
class MunniAction
{
public:
	static MunniAction* GetIns();
private:
	Vector3 Pos_,Rot_,Scl_;
	Player* m_Player_ = nullptr;

	/* MOVE */
	float movSpeed = 0.2f;
	float oldRotY,rotEaseT=0.f;

	int32_t BeginWalkCount = 0;
	int32_t randAddRotVal=90;
	int32_t idleCount = 0;

	bool isOverMoving=FALSE;
	
	float Walk();
	float Follow();

	/* JUMP */
	float m_JumpInter_ = 0.f;
	float m_JumpPower = 1.f;
	void Jump();

	/* FOLLOW*/
	bool isFollow = FALSE;
	bool isSearch = FALSE;
	uint32_t FollowCount = 0;

	/* ATTACK */
	uint32_t atckTimer_=0;
	uint32_t atckCoolTime=0;
	bool isAtck = false;
	bool canAtck = true;

	/* TRANSITION */
	void Attack();
	void Move();
	void Death();

	enum List
	{
		MOVE,
		ATTACK,
		DEATH,
	}list_;
public:
	void AttackTransition();
private:
	//
	static void (MunniAction::*ActionList[])();

public:
	void SetParametors(Vector3 paramsPos, Vector3 paramsRot, Vector3 paramsScl)
	{
		Pos_ = paramsPos;
		Rot_ = paramsRot;
		Scl_ = paramsScl;
	}
	void SetPlayer(Player* player) { m_Player_ = player; }

	Vector3 GetPos() const{ return Pos_; }
	Vector3 GetRor()const { return Rot_; }
	Vector3 GetScl()const { return Scl_; }

	void SetInitPos(Vector3 pos) { Pos_ = pos; }

};

