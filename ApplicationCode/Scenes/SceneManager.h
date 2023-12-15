#pragma once

#include "BaseScene.h"
#include "TitleScene.h"
#include"TutorialScene.h"
#include "GameScene.h"
#include"StageOneFloor.h"
#include"IBScene.h"
#include"SkillScene.h"
#include "BossScene.h"
#include "ResultScene.h"
#include "DirectXSetting.h"
#include "Sound.h"
#include "KeyInput.h"
#include "SafeDelete.h"
#include "TextDraw.h"
#include "CollisionManager.h"

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

public: //シーン管理番号
	enum class SceneName {
		Title,
		Tutorial,
		Game,
		Floor1,
		IB,
		SKILL,
		Boss,
		Boss2,
		Result,
		None
	};

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

public: //静的メンバ関数

	/// <summary>
	/// 更新
	/// </summary>
	static void Update();

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

	/// <summary>
	/// シーン切り替え
	/// </summary>
	static void SceneChange(SceneName scene);

	/// <summary>
	/// スコアを加算
	/// </summary>
	/// <param name="score">スコア</param>
	static void AddScore(const int32_t score);

	/// <summary>
	/// スコアをセット
	/// </summary>
	/// <param name="score"></param>
	static void SetScore(const int32_t score);

	/// <summary>
	/// ステージ番号をセット
	/// </summary>
	static void SetStageNo(const int32_t stageNo);

	/// <summary>
	/// 当たり判定マネージャーをセット
	/// </summary>
	/// <param name="colManager">当たり判定クラス</param>
	static void SetCollsionManager(CollisionManager* colManager) { colManager_ = colManager; }

	/// <summary>
	/// HPをセット
	/// </summary>
	/// <param name="hp">HP</param>
	static void SetHP(int32_t hp) { hp_ = hp; }

	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns>HP</returns>
	static int32_t GetHP() { return hp_; }

	/// <summary>
	/// レベルをセット
	/// </summary>
	/// <param name="level">レベル</param>
	static void SetLevel(int32_t level) { level_ = level; }

	/// <summary>
	/// レベルを取得
	/// </summary>
	/// <returns>レベル</returns>
	static int32_t GetLevel() { return level_; }

	/// <summary>
	/// 経験値をセット
	/// </summary>
	/// <param name="ep">経験値</param>
	static void SetEP(int32_t ep) { ep_ = ep; }

	/// <summary>
	/// 経験値を取得
	/// </summary>
	/// <returns>経験値</returns>
	static int32_t GetEP() { return ep_; }

	/// <summary>
	/// ステージ番号取得
	/// </summary>
	/// <returns>ステージ番号</returns>
	static int32_t GetStageNo() { return stageNo_; }

	/// <summary>
	/// スコア取得
	/// </summary>
	/// <returns>スコア</returns>
	static int32_t GetScore() { return score; }

private: //静的メンバ変数
	static BaseScene* nowScene;
	static int32_t stageNo_;
	static int32_t score;
	static CollisionManager* colManager_;
	//プレイヤーレベル
	static int32_t level_;
	//経験値
	static int32_t ep_;
	//HP
	static int32_t hp_;

private: //メンバ変数
	std::unique_ptr<TextDraw> textDraw;
};