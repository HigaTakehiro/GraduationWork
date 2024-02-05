#pragma once
#include <Vector3.h>
#include"Player.h"
#include<array>
class LastBossAct
{
private:
	//通常パラメータ
	Vector3 Pos_ = {0,-2.5f,-8.f}, Rot_, Scl_ = { 0.040f, 0.065f, 0.050f };
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
		ATTACK_Hole,
		ATTACK_Flame,
		ATTACK_largeSpell,
		Barrier,
		DEATH
	}act_;


	bool Appear();
	//-----------------
	/* 関数テーブル */
	typedef struct
	{
		//状態番号
		int state = 0;
		//各状態の関数
		void(LastBossAct::* func)();
	}AppearState;

	//番号用
	enum StateName
	{
		P1 = 0,
		P2 = 1,
		P3 = 2,
		P_Num = 3
	};

	void StateExecute(int state);
	float CameraEaseT, PosEaseT;
	void Phase1();
	void Phase2();
	void Phase3();
	bool beforeBattle = false;
	static AppearState StateArray[StateName::P_Num];

	StateName cameraStateIndex;
	void Camerawork();
	//-----------------

	void Move();
	float Follow();
	void Attack_Hole();
	void Attack_Flame();
	
	void RunAway();
	void Death();
	void Act_Barrier();
public:
	void Attack_Spell();

public:
	void Transision();
	static void (LastBossAct::* ActionList[])();

private://バリア
	bool BarrierF = false;

	static constexpr int32_t barrierSize = 3;
	std::array<Vector3, barrierSize>BarrierPos;
	std::array<float, barrierSize>BarrierAngle;
	std::array<float, barrierSize>BarrierAlpha={1,1,1};
	std::array<BOOL, barrierSize>BarrierDamF;
	std::array<int, barrierSize>BarrierHp={5,5,5};
	std::array<int, barrierSize>BarrierDamCool;
	bool AllBarrierDestroyF;
	int RefBarrierTime;
private://ぶらっくほーる
	static constexpr int32_t holeSize = 2;
	std::array<std::unique_ptr<Object3d>, holeSize>HoleTex;
	std::array<DirectX::XMFLOAT3, holeSize>HoleSize;
	std::array<DirectX::XMFLOAT3, holeSize>HolePos;
	std::array<float, holeSize>HoleRange;
	float HoleSclingEaseT = 0.f;
	int32_t ShotWaitTime;

	bool HoleSmalling;
	int HoleActivCount;

private:// 火の玉ストレート
	static constexpr int32_t flameSize = 4;

	enum FlamePhase
	{
		INITFLAME,
		SHOTFLAME,
		ENDFLAME
	}flameP;
	std::array<std::unique_ptr<Object3d>, flameSize>FlameTex;
	std::array<DirectX::XMFLOAT3, flameSize>FlameVec;
	std::array<DirectX::XMFLOAT3, flameSize>FlamePos;
	std::array<DirectX::XMFLOAT3, flameSize>FlameInitPos;
	std::array<DirectX::XMFLOAT3, flameSize>FlameScl;
	std::array<DirectX::XMFLOAT4, flameSize>FlameColor;
	int actionval;
	std::string ss;
private://詠唱
	std::unique_ptr<Object3d>SpellRangeTex;
	Vector3 RangeScale;
	int SpellCount = 0;
	bool SpellCancel;
	DirectX::XMVECTOR move = { 0,0,0.1f,0 };
	DirectX::XMMATRIX matRot;
	Vector3 MeteoPos,MeteoScl;
	int beforeHp;
public:
	Vector3 GetPos() { return Pos_; }
	Vector3 GetScl() { return Scl_; }

	Vector3 GetHoleSize(int ind) { return HoleSize[ind]; }
	Vector3 GetHolePos(int ind) { return HolePos[ind]; }

	Vector3 GetFlamePos(int ind) { return FlamePos[ind]; }
	DirectX::XMFLOAT4 GetFlameColor(int ind) { return FlameColor[ind]; }
	Vector3 GetFlameScl(int ind) { return FlameScl[ind]; }

	Vector3 GetBarrierPos(int ind) { return BarrierPos[ind]; }
	float GetBarrierAlpha(int ind) { return BarrierAlpha[ind]; }
	int guardp;
	void SetGuardPoint(int gv) { guardp = gv; }
	Vector3 GetRangeScl() { return RangeScale; }
	Vector3 GetMeteoPos() { return MeteoPos; }
	bool meteof;
	BOOL damff;
	int damcool;
	int hp;
	bool hammeronHole[2];

	void SetDamF(BOOL& F) { F=damff; }
	BOOL GetDamF() { return damff; }
	void SetDamCool(int val) { damcool = val; }
	int GetHp() { return Hp; }
	void SetHp(int val) { Hp = val; }
	void SetPlayerIns(Player* player) { Player_ = player; }

public:
	enum AnimeName
	{
		WIDLE,
		WMOVE,
		WCHARGE,
		WDEATH
	}_state = WMOVE;

	AnimeName GetName() { return _state; }
};

