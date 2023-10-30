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
#include "Player.h"
#include"GameMap.h"
#include "Ore.h"
#include"BossBase.h"
#include"BaseEnemy.h"
#include"Shake.h"
class GameScene : public BaseScene
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

protected: //静的メンバ関数

private: //メンバ関数
	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	void SceneChange();

	/// <summary>
	/// カメラ初期化設定
	/// </summary>
	void CameraSetting();

	void NextMap();
private: //メンバ変数
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_;
	//ポストエフェクト番号
	PostEffect::PostEffectNo postEffectNo_;
	//ライト
	std::unique_ptr<LightGroup> light_;
	//カメラ
	std::unique_ptr<Camera> camera_;
	//プレイヤー
	Player* player_;
	//アイテム
	std::unique_ptr<Ore> ore_;
	//鉱石アイテム
	std::list<std::unique_ptr<Ore>> oreItems_;

	std::array<BaseEnemy*, 3> enemys_;
	OBB* _hummmerObb;

	std::unique_ptr<BossBase>boss_;
	//テキスト描画
	TextDraw* text_;
	//仮地面
	std::unique_ptr<GameMap> map_;
	//カメラ座標
	Vector3 cameraPos_;
	//注視点
	Vector3 targetPos_;
	int count_ = 0;

	Shake* shake_;
	int oldcount_ = 0;
	float oldcamerapos_ = 0;
	float time_ = 0;
	XMFLOAT3 nextPos_{};
	int direction = 0;
};