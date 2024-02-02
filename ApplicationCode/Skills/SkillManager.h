#pragma once
#include "SkillList.h"
#include "ISkill.h"
#include "IActiveSkill.h"
#include "ActiveSkillList.h"
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
	void AddPlayerActiveSkill(IActiveSkill* skill);

	/// <summary>
	/// アクティブスキル01名をセット
	/// </summary>
	/// <param name="activeSkillName01">アクティブスキル01名</param>
	void SetActiveSkillName01(const std::string& activeSkillName01);

	/// <summary>
	/// アクティブスキル01を有効化
	/// </summary>
	/// <param name="isActiveSkill01">アクティブスキル01有効化フラグ</param>
	void SetIsActiveSkill01(bool isActiveSkill01);

	/// <summary>
	/// アクティブスキル02名をセット
	/// </summary>
	/// <param name="activeSkillName02">アクティブスキル02名</param>
	void SetActiveSkillName02(const std::string& activeSkillName02);

	/// <summary>
	/// アクティブスキル02を有効化
	/// </summary>
	/// <param name="isActiveSkill02">アクティブスキル02有効化フラグ</param>
	void SetIsActiveSkill02(bool isActiveSkill02);

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
	ActiveSkillList* activeSkillList_;
	SkillList* passiveSkillList_;
	//アクティブスキル01
	std::string activeSkillName01_;
	//アクティブスキル02
	std::string activeSkillName02_;
};

