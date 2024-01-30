#pragma once
#include "BaseScene.h"
#include "TitleScene.h"
#include"TutorialScene.h"
#include "GameScene.h"
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
#include "SkillManager.h"

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
		IB,
		SKILL,
		Boss,
		Boss2,
		Boss3,
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
	/// 最大HPをセット
	/// </summary>
	/// <param name="maxHp">最大HP</param>
	static void SetMaxHP(int32_t maxHp) { maxHp_ = maxHp; }

	/// <summary>
	/// 最大HPを取得
	/// </summary>
	/// <returns>最大HP</returns>
	static int32_t GetMaxHP() { return maxHp_; }

	/// <summary>
	/// 攻撃力をセット
	/// </summary>
	/// <param name="atk">攻撃力</param>
	static void SetATK(int32_t atk) { atk_ = atk; }

	/// <summary>
	/// 攻撃力を取得
	/// </summary>
	/// <returns>攻撃力</returns>
	static int32_t GetATK() { return atk_; }

	/// <summary>
	/// 守備力をセット
	/// </summary>
	/// <param name="def">守備力</param>
	static void SetDEF(int32_t def) { def_ = def; }

	/// <summary>
	/// 守備力を取得
	/// </summary>
	/// <returns>守備力</returns>
	static int32_t GetDEF() { return def_; }

	/// <summary>
	/// 素早さをセット
	/// </summary>
	/// <param name="spd">素早さ</param>
	static void SetSPD(int32_t spd) { spd_ = spd; }

	/// <summary>
	/// 素早さを取得
	/// </summary>
	/// <returns>素早さ</returns>
	static int32_t GetSPD() { return spd_; }

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
	/// スキルポイントをセット
	/// </summary>
	/// <param name="skillPoint">スキルポイント</param>
	static void SetSkillPoint(int32_t skillPoint) { skillPoint_ = skillPoint; }

	/// <summary>
	/// スキルポイントを取得
	/// </summary>
	/// <returns>スキルポイント</returns>
	static int32_t GetSkillPoint() { return skillPoint_; }

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

private: //静的メンバ関数
	/// <summary>
	/// シーン初期化処理
	/// </summary>
	static void NowSceneInitialize();

private: //静的メンバ変数
	static BaseScene* nowScene;
	static BaseScene* skillScene_;
	static int32_t stageNo_;
	static int32_t score;
	static CollisionManager* colManager_;
	static SkillManager* skillManager_;
	//プレイヤーレベル
	static int32_t level_;
	//経験値
	static int32_t ep_;
	//HP
	static int32_t hp_;
	//最大HP
	static int32_t maxHp_;
	//攻撃力
	static int32_t atk_;
	//防御力
	static int32_t def_;
	//素早さ
	static int32_t spd_;
	//スキルポイント
	static int32_t skillPoint_;
	//スキルシーンフラグ
	static bool isSkillScene_;

private: //メンバ変数
	std::unique_ptr<TextDraw> textDraw;
};