#pragma once
#include "Vector3.h"
#include<DirectXMath.h>
#include"Player.h"
using namespace DirectX;
class MunniAction
{
private:
	Vector3 Pos_,Rot_,Scl_;
	Player* m_Player_ = nullptr;

	/* MOVE */
	float movSpeed = 1.f;
	float oldRotY,rotEaseT=0.f;

	int32_t BeginWalkCount = 0;
	int32_t randAddRotVal=90;
	int32_t idleCount = 0;

	bool isOverMoving=FALSE;
	
	float Walk();
	float Follow();

	/* FOLLOW*/
	bool isFollow = FALSE;
public:
	void Attack();
	void Move();
	void Death();

private:
	//
	

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

};

