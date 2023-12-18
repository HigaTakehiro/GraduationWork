#pragma once
#include "ISkill.h"

class StatusUpSkill : public ISkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="skillName">スキル名</param>
	StatusUpSkill(std::string skillName);

	/// <summary>
	/// スキル使用
	/// </summary>
	void AllUse() override;

private: //メンバ変数
};

