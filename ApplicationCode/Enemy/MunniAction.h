#pragma once
#include "Vector3.h"
#include<DirectXMath.h>
#include"Player.h"
#include"Object3d.h"
#include<memory>
using namespace DirectX;
class MunniAction
{
public:
	MunniAction();
	static MunniAction* GetIns();
private:
	// 座標
	Vector3 Pos_,Rot_,Scl_;
	Vector3 InitPos;
	// プレイヤー用インススタンス
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
	float shakeCount = 0.f;
	void IdleShake(bool shakestop);

	/* IMPACTTEX */
	std::unique_ptr<Object3d>ImpactTex = nullptr;
	std::unique_ptr<Object3d>ImpactAreaTex = nullptr;
	Vector3 impTexScl = { 0,0,0 };
	float impTexAlpha = 1.f;
	float impAreaalpha = 0.f;
	bool isTexScling = FALSE;
	void ImpTexInit();
	void ImpTexUpda();

	/* TRANSITION */
	void Attack();
	void Move();
	void Death();

	/* COLLISION */
	void Collide();

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

	void SetInitPos(Vector3 pos) { Pos_ = pos; InitPos = pos; }

public:
	void ImpTexDraw();

};

