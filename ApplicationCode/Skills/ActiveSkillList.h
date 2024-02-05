#pragma once
#include "IActiveSkill.h"

class ActiveSkillList : public IActiveSkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">スキル名</param>
	ActiveSkillList(const std::string& name);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ActiveSkillList() override;

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Use(Player* player) override;

	/// <summary>
	/// 指定スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="name">スキル名</param>
	void SingleUse(class Player* player, const std::string& name);
	
	/// <summary>
	/// 全スキル有効化
	/// </summary>
	/// <param name="isActive">有効化フラグ</param>
	void SetIsActive(bool isActive) override;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() override;

	/// <summary>
	/// 指定スキル有効化
	/// </summary>
	/// <param name="name">スキル名</param>
	/// <param name="isActive">スキル有効化フラグ</param>
	void SingleIsActive(const std::string& name, bool isActive);

	/// <summary>
	/// 指定スキル有効化フラグ取得
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool GetSingleIsActive(const std::string& name);

	/// <summary>
	/// スキル追加
	/// </summary>
	/// <param name="skill">スキル</param>
	void AddSkill(class IActiveSkill* skill);

	/// <summary>
	/// スキルが取得状態かどうかを取得
	/// </summary>
	/// <param name="name">スキル名</param>
	/// <returns>取得状態</returns>
	bool GetSkill(const std::string& name);

private:
	/// <summary>
	/// スキルリスト
	/// </summary>
	std::list<IActiveSkill*> skillList_;
};

