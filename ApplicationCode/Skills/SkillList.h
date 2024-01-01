#pragma once
#include "ISkill.h"
#include <vector>

class SkillList : public ISkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="listName">リスト名</param>
	SkillList(std::string listName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkillList() = default;

	/// <summary>
	/// 全スキル使用
	/// </summary>
	void AllUse(Player* player) override;

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="name">スキル名</param>
	void Use(const std::string& name, Player* player);

	/// <summary>
	/// スキルを追加
	/// </summary>
	/// <param name="skill">スキル</param>
	void AddSkill(ISkill* skill);

private: //メンバ変数
	/// <summary>
	/// スキルリスト
	/// </summary>
	std::vector<ISkill*> mList_;
};

