#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"
#include<DirectXMath.h>
using namespace DirectX;

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
	void Update(Player* player, bool& Display);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(bool Display);

	void Jump();

	void Spown();

	void HitPlayer(bool& Display);
private:
	//オブジェクト
	Object3d* heart_;
	//モデル
	Model* model_[2];

	Player* player_;

	bool display_ = false;

	//アニメーションタイマー
	int32_t animeTimer_;
	//アニメ時間
	int32_t animeTime_;
	//アニメカウント
	int32_t animeCount_;

	Vector3 pos_;

	bool spown_ = true;

	XMFLOAT4 col_{ 1.f,1.f,1.f,1.f };
};

