#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"CollisionPrimitive.h"
#include "MunniAction.h"
#include "Player.h"
//#include
using namespace DirectX;

class BaseEnemy
{
public:
	

protected:
	
	int type_ = 0;

	enum Type {
		TypeA,
		TypeB,
	}_type;

	/*敵の各パラメータ*/
	//*************************************
	struct Status
	{
		//体力
		int HP = 3;
		//攻撃値
		unsigned int DamageValue;
		//被ダメージ後の仰け反り時間
		float KnockTime;
		//移動スピード
		float MoveSpeed;
		//索敵半径
		float SearchRange;
		//2D
		std::shared_ptr<Texture>Tex, TexBack, TexRight, TexLeft;
		size_t TexSize;
		/*
		std::shared_ptr<Object3d>Tex;
		std::array<Model*, 5>TexModel;
		size_t TexSize;
		 */
		 //座標・回転・スケール
		XMFLOAT3 Pos, Rot, Scl;
		OBB Obb;
	}_status;

	/* OBJ */
	struct StateObj
	{
		/*モデル*/
		std::unique_ptr<Object3d>obj_ = {};
		/* 画像枚数 */
		int TexSize_ = 4;
		/* モデル画像 */
		std::vector<Model*>Model_ = {};
		/* 体力 */
		int Hp_ = 10;
		//攻撃値*/
		unsigned int DamageValue_ = 0;
		/* 被ダメージ後の仰け反り時間 */
		float KnockTime_ = 0.f;
		/* 移動スピード */
		float MoveSpeed_ = 0.f;
		/* 索敵半径 */
		float SearchRange_ = 0.f;
		/* 各パラメータ */
		Vector3 Pos_ = {}, Rot_ = {}, Scl = {};
		/* 色 */
		XMFLOAT4 Color_ = { 1,1,1,1 };
		/*  */
		bool DamCool = FALSE;
	}state_obj_;
	//**************************************
	int guardp;
	/*行動*/
	//**************************************
	enum BaseAction
	{
		IDLE,
		WALK,
		FOLLOW,
		KNOCK,
		ATTACK,
		DEATH
	};
	BaseAction _action = IDLE;

	static void(BaseEnemy::* stateTable[])();

	void Idle(),
		Walk(),
		Follow(),
		Knock(),
		Attack(),
		Death();

	void MoveDirection();
	//**************************************

	//アニメーション関連
	int AnimTim;
	unsigned int AnimationCount;
	unsigned int AnimationInterval;
	bool RecvDamage;
	std::shared_ptr<Player>_player;
	OBB _playerOBB;

	bool _isFlash;
	bool _isAttack;
	bool PlayerRecv;
	float t = 0.0f;
	int FlashCount;
	float back_t;
	XMFLOAT4 _color;

	int m_MaxHp = 0;
	std::unique_ptr<Object3d>m_HpTex;
	std::unique_ptr<Object3d>m_ShadowTex;

	XMFLOAT3 OverPosMax, OverPosMin;

	bool FlashF; float val = 1;
	std::string Tag_;
	//XMFLOAT4 color;

	int count_;
public:
	void SetFlash(bool f) { FlashF = f; }
	bool GetFlash() { return FlashF; }
	bool GetRecv() { return _isFlash; }
	void DamageFlash();

	void PlayerHitBody(float dis,bool& f);
	void DestryAct(float&alpha,int hp);
	float aTime;
	int32_t RefTime;

protected:
	void TexInit();
	void TexUpda();
	BOOL edamf;
	int edamcool;
	bool damfprot;
public:
	void damage();
	void damage2();
	void SetDamF(bool f) { damfprot = f; }
	virtual void TexDraw()=0;

	void TutorialTexDraw();

	void SetOverPos(XMFLOAT3 max, XMFLOAT3 min) { OverPosMax = max; OverPosMin = min; }
	//攻撃受けたか？

public:
	/** 初期化 **/
	virtual void Init() = 0;

	/** 更新 **/
	virtual void Upda(Camera* camera) = 0;

	/** 描画 **/
	virtual void Draw() = 0;

	/** アニメーション **/
	virtual void TextureAnimation() = 0;

	virtual void AttackAction() = 0;

	virtual void TutorialUpda(Camera* camera, bool flag) = 0;

	virtual void TutorialDraw(float Mindis) = 0;


public:
	bool DeathJudg();
	bool death() { if (_status.HP <= 0)return true; return false; }
	void CollideHummmer();

	void RecvFlashColor();
	//プレイヤーのインスタンス引き継ぎ　あとで直す部分
	void SetPlayerIns(Player* player) { _player.reset(player); }
	void SetHammerObb(OBB obb) { _playerOBB = obb; }

	void SetCount(int Count) { this->count_ = Count; }
	int GetCount() { return count_; }

	void GetDamage(bool f);
	void CollideHammerDeb();
private:
	void RotforPlayer();
public:
	int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;

	XMFLOAT3 GetPos2() const;
	XMFLOAT3 GetRot2() const;
	XMFLOAT3 GetScl2() const;

	int GetType()const;

public:
	void SetPos2(Vector3 pos) { state_obj_.Pos_ = pos; }
	void SetPos(Vector3 pos) { _status.Pos = pos; }

	virtual void SetPosDeb(Vector3 pos) = 0;

protected:
	// 防御力
	float GuardValue = 0.f;
	// 受けるダメージ値
	int SubHPValue = 0.f;
public:
	// 防御力アクセサ
	void SetGuardValue(float val) { GuardValue = val; }
	float GetGuardVal()const { return GuardValue; }

	void SetSubHPValue(int val) { SubHPValue = val; }
	int GetSubHPVal()const { return SubHPValue; }

	

};

