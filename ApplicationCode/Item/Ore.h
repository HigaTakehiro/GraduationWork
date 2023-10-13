#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Ore
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Ore() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Ore();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="vec">方向ベクトル</param>
	void Initialize(Vector3 pos, Vector3 vec);
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
	/// <summary>
	/// 当たり判定取得
	/// </summary>
	/// <returns></returns>
	bool GetIsHit() { return ore_->GetIsHit(); }

private: //メンバ変数
	// 鉱石オブジェクト
	std::unique_ptr<Object3d> ore_;
	// 鉱石モデル
	Model* oreModel_;
	// 座標
	Vector3 pos_;
	// 方向ベクトル
	Vector3 vec_;
	// 速度
	float speed_;
};

