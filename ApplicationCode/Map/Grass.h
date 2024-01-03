#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Grass
{
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Grass();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& pos,int I);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const Vector3& playerPos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private: //メンバ関数
	/// <summary>
	/// アルファ値設定
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	void AlphaTest(const Vector3& playerPos);

private: //メンバ変数
	//オブジェクト
	Object3d* grass_;
	//モデル
	Model* model_[3];

	//アニメーションタイマー
	int32_t animeTimer_;
	//アニメ時間
	int32_t animeTime_;
	//アニメカウント
	int32_t animeCount_;
};

