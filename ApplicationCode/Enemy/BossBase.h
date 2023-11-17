#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"CollisionPrimitive.h"
#include "Player.h"
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
	int m_HP=110;
	Name m_Name;
	std::wstring str;

	int BodyDamCool = 0;
	BOOL BodyRecvDam=FALSE;
public:
	void SetPlayerIns(Player* player) { m_player = player; }
	void SetCamera(Camera* cam) { m_Camera = cam; }
	void SetHummerPos(Vector3 pos) { m_Hummmer = pos; }

	std::wstring GetStr() { return str;}
	Name GetName() { return m_Name; }

};

