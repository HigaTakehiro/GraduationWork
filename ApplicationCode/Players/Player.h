#pragma once
#include "Object3d.h"
#include "Vector3.h"

class Player
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private: //メンバ変数
	//座標
	Vector3 pos_;
	//大きさ
	Vector3 scale_;
	//回転角
	float rot_;

	//オブジェクト
	std::unique_ptr<Object3d> player_;
	//モデル
	Model* model_[5];
};

