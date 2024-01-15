#pragma once
#include"Object3d.h"
#include"Key.h"
#include"Heart.h"
#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;

class Player;
class TreasureBox
{
public:

	void Initialize(int num, const XMFLOAT3& MapPos,Player* player,int Count);

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
	unique_ptr<Key> key_;
	unique_ptr<Heart> heart_;

	Model* stairsModel_[4];

	Model* uiModel_;

	XMFLOAT3 pos_;
	XMFLOAT3 uipos_;

	Player* player_;

	int count_ = 0;
	bool lock_ = false;
	bool f = false;
	bool display_ = false;
	bool boxdisplay_ = true;

	//アニメーションタイマー
	int32_t animeTimer_;
	//アニメ時間
	int32_t animeTime_;
	//アニメカウント
	int32_t animeCount_;
};

