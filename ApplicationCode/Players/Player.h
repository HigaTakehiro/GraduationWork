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

private: //メンバ関数

	/// <summary>
	/// プレイヤーステータスの設定
	/// </summary>
	void PlayerStatusSetting();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// ハンマー投げ
	/// </summary>
	void HammerThrow();

	/// <summary>
	/// ハンマー入手
	/// </summary>
	void HammerGet();

	/// <summary>
	/// ハンマー戻り処理
	/// </summary>
	void HammerReturn();

private: //メンバ変数
	//座標
	Vector3 pos_;
	//大きさ
	Vector3 scale_;
	//回転角
	Vector3 rot_;

	//移動速度
	float moveSpeed_;
	//回転速度
	float rotSpeed_;
	//回転リセットタイマー
	float rotResetTimer_;
	//回転リセット時間
	float rotResetTime_;
	//初期回転角
	Vector3 initRot_;

	//仮ハンマー投げ時間
	int32_t hammerTime = 60;
	int32_t hammerTimer = 0;

	//プレイヤーオブジェクト
	std::unique_ptr<Object3d> player_;
	//プレイヤーモデル
	Model* playerModel_[5];

	//ハンマーオブジェクト
	std::unique_ptr<Object3d> hammer_;
	//ハンマーモデル
	Model* hammerModel_;
	//ハンマー投げフラグ
	bool isHammerRelease_;
	//ハンマー座標
	Vector3 hammerPos_;
	//ハンマー投げ速度
	float throwSpeed_;
	//ハンマー投げベクトル
	Vector3 hammerVec_;
	//ハンマー初期値
	Vector3 initHammerPos_;
	Vector3 initHammerScale_;
	Vector3 initHammerRot_;
};

