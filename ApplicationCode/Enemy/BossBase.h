#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"CollisionPrimitive.h"
#include "DamageMath.h"
#include "TogemaruAct.h"

class BossBase
{
public:
	virtual void Init() = 0;
	virtual void Upda() = 0;
	virtual void Draw() = 0;

	virtual void Draw2()=0;
	virtual void Finalize() = 0;
	virtual void Attack()=0;
	virtual void SpriteDraw() = 0;
	
protected:
	std::string names;
	void RecvDamage(Vector3 pos);
	enum Name
	{
		DOGOM,
	};
	UINT DamCool=0;
	bool m_DamF=FALSE;

	Player* m_player = nullptr;
	Camera* m_Camera = nullptr;

	Vector3 m_Hummmer;
	Vector3 m_CameraPos = {0,12,20}, m_Target,m_CameraStartPos;

	int m_HP=6;
	Name m_Name;
	std::wstring str;

	int BodyDamCool = 0;
	BOOL BodyRecvDam=FALSE;
	bool AppearFlag;
	bool m_ClearF=FALSE;
	int Exp;
	bool isGetExp;
public:
	void SetCam(Camera* c) { camera = c; }
	int GetExp()const { return Exp; }
protected://UI
	Camera* camera;
	std::unique_ptr<Sprite>m_HpTex = nullptr;
	std::unique_ptr<Sprite>m_HpTex_Frame = nullptr;
	std::unique_ptr<Sprite>m_HpTex_Inner = nullptr;

	float m_hpInnerSizeX = 400.f, InnerSclingT;;
	float NowHP, BeforeHP;
	bool bravegaugeF;

	int BossMaxHP;
	float m_BodyAlpha = 1.f;

	void HPUiInit();
	void HPUiUpda();
	void HPUiDraw();
	//ボスの行動パターン
	TogemaruAct* Action = nullptr;

protected:
	//各種パラメータ
	Vector3 Pos_, Rot_, Scl_;
public:
	Vector3 GetPos()const { return Pos_; }
public:
	void SetPlayerIns(Player* player) { m_player = player; }
	void SetCamera(Camera* cam) { m_Camera = cam; }
	void SetHummerPos(Vector3 pos) { m_Hummmer = pos; }
	void ArmDamageFlash(bool&f,int&t, XMFLOAT4& c);
	std::wstring GetStr() { return str;}
	Name GetName() { return m_Name; }
	Camera* m_camera;
	void GetCSPos(Vector3 pos) { m_CameraStartPos = pos; }
	void SetCamerains(Camera* cam) { m_camera = cam; }
	virtual bool Appear() = 0;
	bool GetAppearFlag() { return AppearFlag; };

	bool GetClearF() { return m_ClearF; }

	bool GetFlash() { return FlashF; }
public:
	int32_t val = 0;
	bool FlashF;
	XMFLOAT4 color_rgb=XMFLOAT4(1,1,1,1);
	void RecvDamageFlash();
	//bool GetHp() { return m_ClearF; }
	float GetBossRot()const { return Action->GetRotY(); }

protected:
	// 防御力
	int GuardValue = 0.f;
	// 受けるダメージ値
	int SubHPValue = 0.f;
	//
	int AttackValue = 1;
public:
	// 防御力アクセサ
	void SetGuardValue(int val){ GuardValue = val; }
	float GetGuardVal()const { return GuardValue; }

	void SetSubHPValue(int val) { SubHPValue = val; }
	int GetSubHPVal()const { return SubHPValue; }
};

