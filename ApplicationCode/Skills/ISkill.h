#pragma once
#include <stdint.h>
#include <string>

class ISkill {
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ISkill() {};

	/// <summary>
	/// スキル使用
	/// </summary>
	virtual void UseSkill() = 0;
};