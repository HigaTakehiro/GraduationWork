#pragma once
#include "SkillList.h"
#include "ISkill.h"
#include "Player.h"

class SkillManager
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkillManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkillManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// プレイヤーのインスタンスをセット
	/// </summary>
	/// <param name="player">プレイヤーのインスタンス</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// プレイヤーのスキルを追加
	/// </summary>
	/// <param name="skill">プレイヤースキル</param>
	void AddPlayerSkill(ISkill* skill);

	/// <summary>
	/// スキルが入手状態かどうかを取得
	/// </summary>
	/// <param name="skillName">スキル名</param>
	/// <returns></returns>
	bool GetSkill(const std::string& skillName);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private: //メンバ変数
	//プレイヤー
	Player* player_;
	//プレイヤースキルリスト
	SkillList* skillList_;
};

