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

	void Initialize(const XMFLOAT3& Pos, Player* player,int Count);

	void BossInitialize(const XMFLOAT3& Pos, Player* player);

	void Update();

	void Draw();

	int GetCont() { return count_; }

private:

	void CheckHit();

private:
	unique_ptr<Object3d> stairs_;

	Model* stairsModel_;

	XMFLOAT3 pos_;

	Player* player_;

	int count_ = 0;
};


