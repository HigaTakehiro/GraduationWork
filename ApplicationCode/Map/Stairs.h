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
	~Stairs();
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
	unique_ptr<Object3d> ui_;

	Model* stairsModel_;

	Model* uiModel_;

	XMFLOAT3 pos_;
	XMFLOAT3 uipos_;

	Player* player_;

	int count_ = 0;
	bool f = false;
};


