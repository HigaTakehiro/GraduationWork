#pragma once
#include"Object3d.h"
#include"Player.h"
#include"Stairs.h"
#include"Grass.h"
#include "Deposit.h"
#include "Ore.h"
#include"BaseEnemy.h"
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

	struct Grassland
	{
		unique_ptr<Grass> grass_;
		int num;
	};


public:

	void LoadCsv(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, int StageNum);

	void CreateBridge();

	void CreateGrass(const XMFLOAT3& MapPos,int Count);

	void CreateDeposits(const XMFLOAT3& MapPos, int MapNum);

	void CreateEnemy(Player* player,const XMFLOAT3& MapPos, int Enemy);

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

	Deposit* GetDePosit();
	bool DepositIsHit(bool flag) { return deposit_->GetIsHit(flag); }

	int GetCount() { return count_; }

	void NextMap(Player* player,XMFLOAT3& CameraPos,XMFLOAT3& TargetPos,float OldCameraPos);

	void DrawingMap(int StageNum,std::stringstream& stream);

	bool EnemyAllKill();

	void CreateRock();

	bool CheckRockToMap(const XMFLOAT3& RockPos);

	bool ReflectHammer(XMFLOAT3& Pos, bool isHammerRelease);

	/// <summary>
	/// 鉱脈リストを取得
	/// </summary>
	/// <returns>鉱脈</returns>
	unique_ptr<Deposit>& GetDeposit(int32_t number) { return deposits_[number]; }
	/// <summary>
	/// 鉱脈リストサイズを取得
	/// </summary>
	/// <returns>鉱脈リストサイズ</returns>
	int32_t GetDepositsSize() { return deposits_.size(); }

	/// <summary>
	/// 敵リストを取得
	/// </summary>
	/// <returns>鉱脈</returns>
	unique_ptr<BaseEnemy>& GetEnemy(int32_t number) { return enemys_[number]; }

	/// <summary>
	/// 敵リストサイズを取得
	/// </summary>
	/// <returns>鉱脈リストサイズ</returns>
	int32_t GetEnemySize() { return enemys_.size(); }

private:

	list<unique_ptr<Stage>> maps_;

	list<unique_ptr<Bridge>> bridge;

	unique_ptr<Stairs> stairs_;

	list<unique_ptr<Object3d>> rock_;
	
	list<unique_ptr<Grassland>> grass_;

	//鉱石アイテム
	std::list<std::unique_ptr<Ore>> oreItems_;

	Deposit* deposit_;
	std::vector<std::unique_ptr<Deposit>> deposits_;

	vector<unique_ptr<BaseEnemy>> enemys_;

	//マップの番号
	int count_ = 0;
	//古い状態のマプ番号
	int oldcount_ = 0;
	
	int enemyscount_ = 0;

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

	XMFLOAT4 limit_ = { 10.f,10.f,8.8f,12.f };
};


