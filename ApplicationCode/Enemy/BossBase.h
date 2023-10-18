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

	virtual void Attack();

protected:
	enum Name
	{
		DOGOM,
	};

	int m_HP=100;
	Name m_Name;
public:
	Name GetName() { return m_Name; }
};

