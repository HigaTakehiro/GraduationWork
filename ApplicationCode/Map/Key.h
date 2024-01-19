#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"
#include<DirectXMath.h>
using namespace DirectX;

class Player;
class Key
{
public:
	~Key();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos"></param>
	void Initialize(const Vector3& pos);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Player* player,bool& lock,bool Display);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(bool Display);

	void Jump();
private:
	//オブジェクト
	Object3d* key_;
	//モデル
	Model* model[2];

	Player* player_;

	Vector3 pos_;

	float addpos_ = 0.f;

	bool display_ = false;

	XMFLOAT4 col_{ 1.f,1.f,1.f,1.f };

	//アニメーションタイマー
	int32_t animeTimer_;
	//アニメ時間
	int32_t animeTime_;
	//アニメカウント
	int32_t animeCount_;

};

