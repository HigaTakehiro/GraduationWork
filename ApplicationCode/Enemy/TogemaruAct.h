#pragma once
#include"Player.h"
#include <array>
#include <intsafe.h>
#include<DirectXMath.h>
#include "BaseCollision.h"
#include"Vector3.h"
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
	//棘の発射範囲
	float ShotRange = 0.f;
	//発射フラグ
	bool isShot = FALSE;
	//
	void ResetParam_Spear();

private:
	//当たり判定周り
	void CollideDeposit();
	//
	bool depositCollideF = FALSE;
	//
	int32_t depositDelTime = 0;
	//
	Vector3 DepositReproduction();
public:
	//鉱石座標
	static Vector3 depositPos;
	//
	static bool depositDelF;
public:
	//アニメーション名前
	enum AnimeName
	{
		IdlE,
		WALK,
		ROLE,
		CRUSH
	}anime_name_;

	AnimeName GetName()const { return anime_name_; }
private:
	//通常パラメータ
	Vector3 Pos_,Rot_,Scl_={};
	//戦闘開始
	bool beginBattle = FALSE;
	//プレイヤーインスタンス
	Player* Player_ = nullptr;
	//攻撃インターバル
	uint32_t actionCount = 1;
	//行動リスト
	enum Act
	{
		MOVE,
		ATTACK_RUSH,
		ATTACK_SHOTSPEAR,
		RUNAWAY,
		DEATH
	}act_;

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
public:
	//行動遷移
	void Transition();
public:
	// ゲッター //
	Vector3 GetPos()const { return Pos_; }
	Vector3 GetRot()const { return Rot_; }
	Vector3 GetScl()const { return Scl_; }

	Vector3 GetSpearPos(int index)const { return SpearPos_[index]; }

	//鉱石座標
	Vector3 GetDepositPos()const { return depositPos; }

	int32_t GetCrushSpearNum()const { return crushSpearNum; }

	float GetSpearAlpha(int index)const { return spearsAlpha[index]; }

	
	//
	bool GetDepositDelF()const { return depositDelF; }

	// セッター //
	void SetPlayerIns(Player* player) { Player_ = player; }

	void SetDepositPos(Vector3 pos) { depositPos = pos; }

};

