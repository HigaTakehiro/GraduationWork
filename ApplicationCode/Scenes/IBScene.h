#pragma once
#include "BaseScene.h"
#include "PostEffect.h"
#include "SafeDelete.h"
#include "MouseInput.h"
#include "Object3d.h"
#include "Vector3.h"
#include "Camera.h"
#include "MouseInput.h"
#include "Easing.h"
#include "Shapes.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "DebugText.h"
#include "MotionMath.h"
#include "SceneManager.h"
#include "TextDraw.h"
#include "JsonLoader.h"
#include "Sprite.h"
#include"GameMap.h"
#include "Ore.h"
#include"BossBase.h"
#include"BaseEnemy.h"
#include"Shake.h"
#include"IntermediateBase.h"
#include "SceneChange.h"
#include "Player.h"
#include "SkillPanel.h"

class IBScene :public BaseScene
{
public: //構造体
	struct SkillPanelStatus {
		int32_t panelStatus_;
		std::unique_ptr<SkillPanel> skillPanel_;
	};

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
	/// <summary>
	/// アニメーション処理
	/// </summary>
	void Animation();

	/// <summary>
	/// UI更新処理
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// スキル画面更新処理
	/// </summary>
	void SkillUIUpdate();

	/// <summary>
	/// スキルパネル
	/// </summary>
	void SkillPanelInitialize();

	/// <summary>
	/// スキル追加処理
	/// </summary>
	void AddSkill(int32_t arrayNum_1, int32_t arrayNum_2);

private: //メンバ変数
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_;
	//ポストエフェクト番号
	PostEffect::PostEffectNo postEffectNo_;
	//ライト
	std::unique_ptr<LightGroup> light_;
	//カメラ
	std::unique_ptr<Camera> camera_;
	//テキスト描画
	TextDraw* text_;
	//仮地面
	std::unique_ptr<GameMap> map_;
	//背景画像
	std::unique_ptr<Sprite> background_;
	//カメラ座標
	Vector3 cameraPos_;
	//注視点
	Vector3 targetPos_;

	Shake* shake_;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};

	IntermediateBase* ib_;

	int baseNo = 0;

	//プレイヤーオブジェクト
	std::unique_ptr<Object3d> player_;
	//プレイヤーモデル
	Model* playerModel_[4];
	//プレイヤーオブジェクト
	std::unique_ptr<Object3d> fire_;
	//プレイヤーモデル
	Model* fireModel_;
	std::unique_ptr<Sprite> susumu_;
	std::unique_ptr<Sprite> skillSprite_;
	std::unique_ptr<Sprite> skillB_;
	std::unique_ptr<Sprite> arrow;

	//アニメーションカウント
	int32_t animeCount_;
	//アニメーションスピード
	int32_t animeSpeed_;
	//アニメーションタイマー
	int32_t animeTimer_;
	//前フレームアニメーションカウント
	int32_t preAnimeCount_;
	int32_t hp_ = 0;
	float count = 0;
	float count2 = 2;
	float soundCount;
	int32_t baseCount = 0;
	SceneChangeEffect* schange;
	Player* playerUI_;
	bool skillFlag = false;
	int32_t skillCount = 0;
	int32_t skillCount2 = 0;

	//スキル画面スプライト
	//スキル画面プレイヤースプライト
	std::unique_ptr<Sprite> skillPlayer_[4];
	//スキルパネル
	SkillPanelStatus panelStatus_[7][7];
	//スキル画面カーソルスプライト
	std::unique_ptr<Sprite> skillCursor_;
};

