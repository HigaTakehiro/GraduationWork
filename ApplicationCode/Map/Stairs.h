#pragma once
#include"Object3d.h"
#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;

class Stairs
{
public:
	void Initialize(const XMFLOAT3& Pos);

	void Update();

	void Draw();

private:
	unique_ptr<Object3d> stairs_;

	XMFLOAT3 pos_;
};

