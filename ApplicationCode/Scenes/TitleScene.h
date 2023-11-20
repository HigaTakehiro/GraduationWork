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
#include "TextDraw.h"
#include "SceneManager.h"
#include "JsonLoader.h"
#include "Button.h"
#include "SceneChange.h"
class TitleScene : public BaseScene
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

private: //静的メンバ変数

private: //メンバ変数
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_;
	//ポストエフェクト番号
	PostEffect::PostEffectNo postEffectNo_;
	//ライト
	std::unique_ptr<LightGroup> light_;
	//カメラ
	std::unique_ptr<Camera> camera_;

	//カメラ座標
	Vector3 cameraPos_;
	//注視点
	Vector3 targetPos_;

	//テキスト描画
	TextDraw* text_;
	SceneChangeEffect* schange;
};