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
#include"IntermediateBase.h"
#include "Grass.h"
#include"AttackEffect.h"
#include "ParticleManager.h"
#include "SkillPanel.h"

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

	/// <summary>
	/// 敵に関係する処理
	/// </summary>
	void EnemyProcess();

	/// <summary>
	/// パーティクル生成
	/// </summary>
	void ParticleCreate();
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
	//鉱石アイテム
	std::list<std::unique_ptr<Ore>> oreItems_;
	//無敵状態パーティクル
	std::unique_ptr<ParticleManager> invincibleParticle_;
	//スキルパネル
	std::unique_ptr<SkillPanel> activeSkillPanel01_;
	std::unique_ptr<SkillPanel> activeSkillPanel02_;

	std::vector<BaseEnemy*> enemys_;
	std::vector<Vector3> vec;
	OBB* _hummmerObb;

	unique_ptr<Object3d> dome;

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
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};
	Vector3 pos{};
	Vector3 target{};
	SceneChangeEffect* schange;
	AttackEffect* aEffect_;
	int aeCount = 0;
	int ShakeCount = 0;
};