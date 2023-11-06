#pragma once
#include"Object3d.h"
#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;

class Player;
class Stairs
{
public:

	void LoadCsv();

	void Initialize(const XMFLOAT3& Pos, Player* player);

	void Update();

	void Draw();

private:

	void CheckHit();

private:
	unique_ptr<Object3d> stairs_;

	XMFLOAT3 pos_;

	Player* player_;

};


