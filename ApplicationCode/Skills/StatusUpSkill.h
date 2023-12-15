#pragma once
#include "ISkill.h"

class StatusUpSkill : public ISkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="skillName">スキル名</param>
	StatusUpSkill(std::string skillName) : skillName_(skillName) {};

	/// <summary>
	/// スキル使用
	/// </summary>
	void Use(const int32_t depth) override;

private: //メンバ変数

	//スキル名
	std::string skillName_;
};

