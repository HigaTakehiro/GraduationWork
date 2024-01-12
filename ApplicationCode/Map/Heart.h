#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Player;
class Heart
{
public:
	~Heart();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos"></param>
	void Initialize(const Vector3& pos);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Player* player);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	void HitPlayer();
private:
	//オブジェクト
	Object3d* heart_;
	//モデル
	Model* model;

	Player* player_;

	bool display_ = false;

	//アニメーションタイマー
	int32_t animeTimer_;
	//アニメ時間
	int32_t animeTime_;
	//アニメカウント
	int32_t animeCount_;

};

