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
	SkillList(std::string listName) : listName_(listName) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkillList() = default;

	/// <summary>
	/// 全スキル使用
	/// </summary>
	/// <param name="depth">階層</param>
	void Use(const int32_t depth) override;

	/// <summary>
	/// スキルを追加
	/// </summary>
	/// <param name="skill">スキル</param>
	void AddSkill(ISkill* skill);

private: //メンバ変数
	/// <summary>
	/// スキルリスト名
	/// </summary>
	std::string listName_;
	/// <summary>
	/// スキルリスト
	/// </summary>
	std::vector<ISkill*> mList_;
};

