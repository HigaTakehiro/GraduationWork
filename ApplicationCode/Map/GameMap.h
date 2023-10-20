#pragma once
#include"Object3d.h"
#include"Player.h"
#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;
class GameMap
{
private:
	enum Map {
		None = 0,
		Normal,
		Forest,
		Enemy,
		Boss,
	};

	enum Direction {
		Vertical=0,
		Beside,
	};

	struct Stage
	{
		unique_ptr<Object3d> stage_;
		XMFLOAT3 stagePos_;
		int num;
		Map state_;
	};

	struct Bridge
	{
		unique_ptr<Object3d> bridge_;
		int num;
		Direction state_;
	};

public:
	
	void LoadCsv();
	
	void CreateBridge();

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initalize();

	/// <summary>
	/// XVˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(int OldCount);

	/// <summary>
	/// I—¹ˆ—
	/// </summary>
	void Finalize();

	void CheckNowNumber(const XMFLOAT3& pos);

	void CheckHitTest(Player* player);

	void CheckBridge();
	
	int CheckHitBridge(const XMFLOAT3& pos);

	int GetCount(const XMFLOAT3& pos);

	void SetStop(bool flag) { this->stopCount_ = flag; }

	int GetNextVal() { return nextval_; }

	XMFLOAT3 GetNowMapPos();
	
private:

	list<unique_ptr<Stage>> maps_;

	list<unique_ptr<Bridge>> bridge;

	list<unique_ptr<Object3d>> bridge_;

	Stage* sta[3][3];

	XMFLOAT3 pos_[3][3];

	int count_ = 0;

	bool stopCount_ = false;

	int nextval_ = 0;

	int bridgeDirection = 0;
};

