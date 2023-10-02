#pragma once

#include "BaseScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "DirectXSetting.h"
#include "Sound.h"
#include "KeyInput.h"
#include "SafeDelete.h"
#include "TextDraw.h"

class SceneManager
{
public:
	SceneManager() = default;
	~SceneManager() = default;

public: //シーン管理番号
	enum class SceneName {
		Title,
		Game,
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

private: //メンバ変数
	std::unique_ptr<TextDraw> textDraw;
};