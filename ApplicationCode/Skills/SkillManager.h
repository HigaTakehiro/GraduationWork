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
	/// プレイヤーのスキルを追加(パッシブスキル)
	/// </summary>
	/// <param name="skill">プレイヤーパッシブスキル</param>
	void AddPlayerPassiveSkill(ISkill* skill);

	/// <summary>
	/// プレイヤーのスキルを追加(アクティブスキル)
	/// </summary>
	/// <param name="skill">プレイヤーアクティブスキル</param>
	void AddPlayerActiveSkill(ISkill* skill);

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
	SkillList* activeSkillList_;
	SkillList* passiveSkillList_;
};

