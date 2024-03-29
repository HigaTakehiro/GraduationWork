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
#include "MessageWindow.h"
#include "Deposit.h"
#include "AttackEffect.h"
#include "ParticleManager.h"
#include "SkillPanel.h"

class TutorialScene :
	public BaseScene
{
private:
    enum Phase {
		Title,
        Description,
        Move,
        Spown,
        Fight,
        Defeat,
        Free,
    }phase_;

private:
    static void (TutorialScene::* FuncTable[])();

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
	/// <summary>
	/// 敵の処理
	/// </summary>
	void EnemyProcess();

	void SleepShale();

	/// <summary>
	/// パーティクル生成
	/// </summary>
	void ParticleCreate();

public:

	void TitlePhase();

    void DescriptionPhase();

    void MovePhase();

    void SpownPhase();

    void FightPhase();

    void DefeatPhase();

    void FreePhase();
private:
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

	//タイトル
	unique_ptr<Sprite> titlefilter_;
	unique_ptr<Sprite> title_[9];
	unique_ptr<Sprite> asist_;
	unique_ptr<Sprite> wake_;
	unique_ptr<Sprite> nextui_;
	unique_ptr<SkillPanel> activeSkillPanel01_;
	unique_ptr<SkillPanel> activeSkillPanel02_;
	unique_ptr<Object3d> sleep_;
	Deposit* deposit_;
	std::vector<BaseEnemy*> enemys_;
	std::vector<Vector3> vec;
	unique_ptr<ParticleManager> invincibleParticle_;
	OBB* _hummmerObb;

	std::unique_ptr<BossBase>boss_;
	//テキスト描画
	TextDraw* text_;
	//仮地面
	std::unique_ptr<GameMap> map_;
	//背景画像
	std::unique_ptr<Sprite> background_;
	//鉱石アイテム
	std::list<std::unique_ptr<Ore>> oreItems_;
	//モデル
	Model* sleepModel_[4];

	Model* titleModel_[9];

	unique_ptr<Object3d> dome;

	Model* sky;

	//テキスト
	TextDraw* movetextui_;
	
	TextDraw* fighttextui_;

	TextDraw* lasttext_;

	//カメラ座標
	Vector3 cameraPos_;
	//注視点
	Vector3 targetPos_;

	Shake* shake_;
	//テキストウィンドウ
	MessageWindow* textWindow_;
	MessageWindow* fighttextwindow_;
	float oldcamerapos_ = 0;
	XMFLOAT3 nextPos_{};

	bool action_ = false;

	float timer_ = 0;

	int description_ = 0;

	bool notlook_ = false;

	XMFLOAT3 startpos_ = {};

	bool titlepos_ = true;

	XMFLOAT2 size_ = { 1280.f,720.f };

	XMFLOAT2 titleposition_{160,100};
	XMFLOAT2 wakePos_{ 500.f,WinApp::window_height - 110 };
	XMFLOAT2 asistPos_{ WinApp::window_width-250 ,WinApp::window_height - 100 };
	XMFLOAT3 sleepPos_ = { 0.f,-2.5f,50.f };
	SceneChangeEffect* schange;
	

	//アニメーションカウント
	int32_t animeCount_;
	//アニメーションスピード
	int32_t animeSpeed_=8;
	//アニメーションタイマー
	int32_t animeTimer_;
	//前フレームアニメーションカウント
	int32_t preAnimeCount_;


	//アニメーションカウント
	int32_t titleanimeCount_=0;
	//アニメーションスピード
	int32_t titleanimeSpeed_ = 8;
	//アニメーションタイマー
	int32_t titleanimeTimer_;
	//前フレームアニメーションカウント
	int32_t titlepreAnimeCount_;

	Vector3 startenemypos_[3];


	bool stop_ = true;

	bool notattack_ = true;

	bool notjump_ = true;

	float movetimer_ = 0;

	int pushCount_ = 0;
	int oldpushCount_ = 0;
	int shaketimer_ = 0;
	float shakeval_ = 0.5f;
	int ShakeCount = 0;
	AttackEffect* aEffect_;
};
