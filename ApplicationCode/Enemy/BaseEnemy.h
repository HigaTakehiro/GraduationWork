#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"CollisionPrimitive.h"
#include "Player.h"
//#include
using namespace DirectX;

class BaseEnemy
{
protected:

	/*敵の各パラメータ*/
	//*************************************

	struct Status
	{
		//体力
		unsigned int HP;
		//攻撃値
		unsigned int DamageValue;
		//被ダメージ後の仰け反り時間
		float KnockTime;
		//移動スピード
		float MoveSpeed;
		//索敵半径
		float SearchRange;
		//2D
		std::shared_ptr<Texture>Tex;
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
	//**************************************

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

	std::shared_ptr<Player>_player;
	OBB _playerOBB;
private:

	//攻撃受けたか？
	bool RecvDamage;
public:
	/** 初期化 **/
	virtual void Init() = 0;

	/** 更新 **/
	virtual void Upda(Camera* camera) = 0;

	/** 描画 **/
	virtual void Draw() = 0;

	/** アニメーション **/
	virtual void TextureAnimation() = 0;
public:
	bool DeathJudg();

	void CollideHummmer();
	//プレイヤーのインスタンス引き継ぎ　あとで直す部分
	void SetPlayerIns(Player* player) { _player.reset(player); }
	void SetHammerObb(OBB obb) { _playerOBB=obb; }
public:
	unsigned int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;
};

