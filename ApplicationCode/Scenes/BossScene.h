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
#include "SceneChange.h"
class BossScene :
    public BaseScene
{
public:
    void Initialize();

    void Update();

    void Draw();

    void Finalize();

private:
	/// <summary>
	/// シーン切り替え処理
	/// </summary>
	void SceneChange();

    /// <summary>
    /// カメラ初期化設定
    /// </summary>
    void CameraSetting();

private:
	//ポストエフェクト
	std::unique_ptr<PostEffect> postEffect_;
	//ポストエフェクト番号
	PostEffect::PostEffectNo postEffectNo_;
	//ライト
	std::unique_ptr<LightGroup> light_;
	//カメラ
	std::unique_ptr<Camera> camera_;
	//
	std::unique_ptr<Stairs>m_Stairs;
	//プレイヤー
	Player* player_;

	//鉱石アイテム
	std::list<std::unique_ptr<Ore>> oreItems_;

	std::vector<BaseEnemy*> enemys_;
	std::vector<Vector3> vec;
	OBB* _hummmerObb;

	std::unique_ptr<BossBase>boss_;
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
	std::unique_ptr<Sprite>m_ClearTex = nullptr;
	XMFLOAT2 m_ClearTexScl;
	float ClearTexEaseT;
	bool NextClearF;
	bool touchFlor;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};
	SceneChangeEffect* schange;
};

