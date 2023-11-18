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
		Start,
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
		bool stop = false;
	};

	struct Bridge
	{
		unique_ptr<Object3d> bridge_;
		int num;
		Direction state_;
	};

public:

	void LoadCsv(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, int StageNum);

	void CreateBridge();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initalize(Player* player,XMFLOAT3& CameraPos,XMFLOAT3& TargetPos,int StageNum);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, float OldCameraPos,bool flag=true);

	/// <summary>
	/// 描画
	/// </summary>
	void MapDraw();

	void BridgeDraw(bool flag = true);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	void CheckHitTest(Player* player);

	void CheckHitBridge(const XMFLOAT3& pos, int& Direction);

	int NextCount(const XMFLOAT3& pos, int& Direction);

	void SetStop(bool flag) { this->stopCount_ = flag; }

	XMFLOAT3 GetStartPos() { return startpos_; }

	XMFLOAT3 GetNowMapPos();

	int GetCount() { return count_; }

	void NextMap(Player* player,XMFLOAT3& CameraPos,XMFLOAT3& TargetPos,float OldCameraPos);

	void DrawingMap(int StageNum,std::stringstream& stream);

	void CreateRock();

	bool CheckRockToMap(const XMFLOAT3& RockPos);

	bool ReflectHammer(XMFLOAT3& Pos);

private:

	list<unique_ptr<Stage>> maps_;

	list<unique_ptr<Bridge>> bridge;

	unique_ptr<Stairs> stairs_;

	list<unique_ptr<Object3d>> rock_;
	//マップの番号
	int count_ = 0;
	//古い状態のマプ番号
	int oldcount_ = 0;
	//
	bool stopCount_ = false;

	bool nothit_ = false;

	int nextval_ = 0;
	//イージング用の時間
	float time_ = 0;
	//橋の角度から次の座標の位置を決める
	int direction_ = 0;
	//プレイヤーの開始位置
	XMFLOAT3 startpos_{};

	int nowstate_ = 0;
	//岩の座標
	XMFLOAT3 rockPos_{};

	XMFLOAT4 limit_ = { 13.f,11.f,7.f,16.f };
};


