#pragma once
#include"Object3d.h"
#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;

class Player;
class TreasureBox
{
public:

	void Initialize(bool empmty, const XMFLOAT3& MapPos,Player* player,int Count);

	void Update();

	void Draw();

	void Finalize();

	bool GetLock() { return lock_; }

	void SetLock(bool Lock) { this->lock_ = Lock; }

	int GetCont() { return count_; }

private:

	void CheckHit();

private:
	unique_ptr<Object3d> treasurebox_;
	unique_ptr<Object3d> ui_;
	unique_ptr<Object3d> key_;

	Model* stairsModel_;

	Model* uiModel_;

	XMFLOAT3 pos_;
	XMFLOAT3 uipos_;

	Player* player_;

	int count_ = 0;
	bool lock_ = false;
	bool f = false;
};

