#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Deposit
{
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Deposit();

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
	/// 鉱石ドロップ方向
	/// </summary>
	/// <returns></returns>
	Vector3 OreDropVec();

	/// <summary>
	/// 鉱床ヒット判定取得
	/// </summary>
	/// <returns>ヒット判定</returns>
	bool GetIsHit();

	/// <summary>
	/// HP取得
	/// </summary>
	/// <returns>HP</returns>
	int32_t GetHP() { return hp_; }

private: //メンバ変数
	//鉱床オブジェクト
	std::unique_ptr<Object3d> deposit_;
	//鉱床モデル
	Model* model_;
	//HP
	int32_t hp_;
	//無敵時間
	int32_t hitCoolTime_;
	//無敵時間タイマー
	int32_t hitCoolTimer_;
	
};

