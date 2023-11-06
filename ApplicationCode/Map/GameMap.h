#pragma once
#include"Object3d.h"
#include"Player.h"
#include"Stairs.h"

#include<memory.h>
#include<DirectXMath.h>
using namespace std;
using namespace DirectX;

class Player;
class GameMap
{
private:
	enum Map {
		None = 0,
		Normal,
		Forest,
		Enemy,
		Kaidan,
		Boss,
	};

	enum Direction {
		Vertical = 0,
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

	void LoadCsv(Player* player);

	void CreateBridge();

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initalize(Player* player);

	/// <summary>
	/// XVˆ—
	/// </summary>
	void Update(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, float OldCameraPos);

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

	/// <summary>
	/// I—¹ˆ—
	/// </summary>
	void Finalize();

	void CheckHitTest(Player* player);

	void CheckHitBridge(const XMFLOAT3& pos, int& Direction);

	int NextCount(const XMFLOAT3& pos, int& Direction);

	void SetStop(bool flag) { this->stopCount_ = flag; }

	XMFLOAT3 GetNowMapPos();

	void NextMap(Player* player,XMFLOAT3& CameraPos,XMFLOAT3& TargetPos,float OldCameraPos);

private:

	list<unique_ptr<Stage>> maps_;

	list<unique_ptr<Bridge>> bridge;

	unique_ptr<Stairs> stairs_;

	int count_ = 0;

	int oldcount_ = 0;

	bool stopCount_ = false;

	bool nothit_ = false;

	int nextval_ = 0;

	int bridgeDirection = 0;

	float time_ = 0;

	int direction_ = 0;
};


