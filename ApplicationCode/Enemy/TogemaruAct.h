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
	//飛ばす針用
	static constexpr UINT spearSize = 8;

	std::array<Vector3, spearSize>SpearPos_ = {};
	std::array<float, spearSize>SpearAngle_ = {};
	//針のアルファ値
	std::array<float,spearSize>spearsAlpha ={};
	std::array<float, spearSize>spearsRot= {};
	//棘の発射範囲
	float ShotRange = 0.f;
	//発射フラグ
	bool isShot = FALSE;
	//
	void ResetParam_Spear();

private:
	//当たり判定周り
	void CollideDeposit();

	void CollideDeposit2();
	//
	bool depositCollideF = FALSE;
	//
	int32_t depositDelTime = 0;

	bool depositCollideF2 = FALSE;
	//
	int32_t depositDelTime2 = 0;
	//
	Vector3 DepositReproduction();
public:
	//鉱石座標
	static Vector3 depositPos;
	//
	static bool depositDelF;
	//鉱石座標
	static Vector3 depositPos2;
	//
	static bool depositDelF2;
public:
	//アニメーション名前
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
	float GetSpearsRot(int index) { return spearsRot[index]; }

private:

	//通常パラメータ
	Vector3 Pos_,Rot_,Scl_= { 0.040f, 0.065f, 0.050f };
	//戦闘開始
	bool beginBattle = FALSE;
	//プレイヤーインスタンス
	Player* Player_ = nullptr;
	//攻撃インターバル
	uint32_t actionCount = 1;
	//
	int Hp;
	//行動リスト
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
	/* 関数テーブル */
	typedef struct
	{
		//状態番号
		int state=0;
		//各状態の関数
		void(TogemaruAct::*func)();
	}AppearState;

	//番号用
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

	//関数ポインタ
	static void (TogemaruAct::*ActionList[])();

//移動
private:
	float Walk();//通常歩き
	float Follow();//追従

	//移動スピード
	float movSpeed = 0.1f;
	//向いた方に移動する
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot ={};

	//回転用のアングル
	float angle_ = 0.f;
	//
	float oldRotY_;
	//
	float rotEaseT = 0.f;
	//追跡するか
	bool isFollow = FALSE;
	//
	int32_t animationWaitTime = 0;
	//突進用変数
	float rushEaseT=0.f;
	//突進終わって針出すまでの待ち時間
	int32_t waitShotCount = 0;
	Vector3 RushStartPos = {};

	bool RoleF;
	bool damf = false;
	int32_t damcool = 0;
//ダメージくらったとき
private:
	bool isSpearCrush=FALSE;
	//破壊した棘の数
	int32_t crushSpearNum = 0;
	//棘の再生時間
	int32_t reproductionTime = 0;
	//RunAway()条件
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
		bool DeathSmallF;
		int ActionRandVal=0;
//シェイク
private:
	bool shakeF = FALSE;
	float shakeXVal=0.f, shakeYVal=0.f;
	float shakeT = 0.f;

	float dshakeXVal = 0.f, dshakeYVal = 0.f;
	float dshakeT = 0.f;
	int guardp;
	Vector3 DepositReproduction2();
public:
	//行動遷移
	void Transition();
	//画面振動(鉱石にぶつかった時)
	static Vector3 cameraPos;//カメラ座標
	static Vector3 oldCameraPos;
	static Vector3 DefaultPos;
	static bool TogemaruDeathF;
	void ViewShake();
	void SetGuardPoint(int gv) { guardp = gv; }
	bool GetRole()const { return RoleF; }
	void SetHp(int hp) { Hp = hp; }
public:
	// ゲッター //
	Vector3 GetPos()const { return Pos_; }
	Vector3 GetRot()const { return Rot_; }
	Vector3 GetScl()const { return Scl_; }

	Vector3 GetSpearPos(int index)const { return SpearPos_[index]; }

	//鉱石座標
	Vector3 GetDepositPos()const { return depositPos; }
	//鉱石座標
	Vector3 GetDepositPos2()const { return depositPos2; }

	int32_t GetCrushSpearNum()const { return crushSpearNum; }

	float GetSpearAlpha(int index)const { return spearsAlpha[index]; }
	
	//
	bool GetDepositDelF()const { return depositDelF; }

	bool GetDepositDelF2()const { return depositDelF2; }

	// セッター //
	void SetPlayerIns(Player* player) { Player_ = player; }

	void SetDepositPos(Vector3 pos) { depositPos = pos; }

	void SetDepositPos2(Vector3 pos) { depositPos2 = pos; }

	float GetRotY()const { return Rot_.y; }
};

