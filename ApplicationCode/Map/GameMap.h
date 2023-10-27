#pragma once
#include"Object3d.h"
#include"Player.h"
#include<memory.h>
#include<DirectXMath.h>
#include <fstream>
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
	/// 初期化
	/// </summary>
	void Initalize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(int OldCount);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	void CheckHitTest(Player* player);
	
	void CheckHitBridge(const XMFLOAT3& pos,int& Direction);

	int NextCount (const XMFLOAT3& pos, int& Direction);

	void NoHitCheck(const XMFLOAT3& pos);

	void SetStop(bool flag) { this->stopCount_ = flag; }

	int GetNextVal() { return nextval_; }

	XMFLOAT3 GetNowMapPos();

	/// <summary>
	/// 拠点セーブ
	/// </summary>
	void MapSave(XMFLOAT3 pos);
private:

	list<unique_ptr<Stage>> maps_;

	list<unique_ptr<Bridge>> bridge;

	list<unique_ptr<Object3d>> bridge_;

	Stage* sta[3][3];

	XMFLOAT3 pos_[3][3];

	int count_ = 0;

	bool stopCount_ = false;

	bool nothit_ = false;

	int nextval_ = 0;

	int bridgeDirection = 0;
};

