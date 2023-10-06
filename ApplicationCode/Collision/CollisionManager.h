#pragma once
#include "BaseCollision.h"
#include <list>

class Object3d;

class CollisionManager
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~CollisionManager();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// オブジェクト追加
	/// </summary>
	/// <param name="obj">追加オブジェクト</param>
	void AddObj(Object3d& obj);
	/// <summary>
	/// コリジョンチェック
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	void CollisionCheck(Object3d* obj1, Object3d* obj2);
	/// <summary>
	/// 当たり判定計算
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	void HitTest(Object3d* obj1, Object3d* obj2);
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private: //メンバ変数
	//オブジェクトリスト
	std::list<Object3d*> objList_;
	//コリジョンチェック
	BaseCollision* collision_[1][1];
};

