#pragma once
#include "ISkill.h"
#include <list>

class SkillList : public ISkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkillList(const std::string& name);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkillList();

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Use(Player* player) override;

	/// <summary>
	/// 指定スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="name">使用したいスキル名</param>
	void SingleUse(Player* player, std::string& name);

	/// <summary>
	/// スキル追加
	/// </summary>
	/// <param name="skill">追加スキル</param>
	void AddSkill(ISkill* skill);

private: //メンバ変数
	//スキルリスト
	std::list<ISkill*> skillList_;
};

