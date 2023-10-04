#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>

using namespace DirectX;

class BaseEnemy
{
protected:
	/*敵の各パラメータ*/
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
		//2D
		std::shared_ptr<Texture>Tex;
		//座標・回転・スケール
		XMFLOAT3 Pos,Rot,Scl;
	}_status;

	
public:
	/** 初期化 **/
	virtual void Init()=0;

	/** 更新 **/
	virtual void Upda(Camera*camera)=0;

	/** 描画 **/
	virtual void Draw()=0;

public:
	bool DeathJudg();

public:
	unsigned int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;
};

