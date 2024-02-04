#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "SceneManager.h"
#include "PostEffect.h"


class ClearScene :
    public BaseScene
{
public: //メンバ関数
/// <summary>
/// 初期化関数
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
/// シーン切り替え処理
/// </summary>
	void SceneChange();

	/// <summary>
	/// カメラ初期化設定
	/// </summary>
	void CameraSetting();

	void Animation();
private: //メンバ変数
//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_;
	//ポストエフェクト番号
	PostEffect::PostEffectNo postEffectNo_;
	//ライト
	std::unique_ptr<LightGroup> light_;
	//カメラ
	std::unique_ptr<Camera> camera_;
	//仮地面
	std::unique_ptr<GameMap> map_;
	//背景画像
	unique_ptr<Object3d> idol_;
	//足場
	unique_ptr<Object3d>ground_;
	//モデル
	Model* idolModel_[4];

	Model* housemodel_;

	unique_ptr<Object3d> dome;

	unique_ptr<Object3d>house_[6];

	//カメラ座標
	Vector3 cameraPos_;
	//注視点
	Vector3 targetPos_;

	Shake* shake_;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};

	IntermediateBase* ib_;
	SceneChangeEffect* schange;

	int baseNo = 0;

	//プレイヤーオブジェクト
	std::unique_ptr<Object3d> player_;
	//プレイヤーモデル
	Model* playerModel_[4];
	XMFLOAT3 idolPos_ = { 0.f,0.f,0.f };
	//アニメーションカウント
	int32_t animeCount_ = 0;
	//アニメーションスピード
	int32_t animeSpeed_ = 8;
	//アニメーションタイマー
	int32_t animeTimer_;
	//前フレームアニメーションカウント
	int32_t preAnimeCount_;
	//アニメーションカウント
	int32_t idolanimeCount_ = 0;
	//アニメーションスピード
	int32_t idolanimeSpeed_ = 8;
	//アニメーションタイマー
	int32_t idolanimeTimer_;
	//前フレームアニメーションカウント
	int32_t idolpreAnimeCount_;
};

