#pragma once
#include <stdint.h>
#include <string>

class ISkill {
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ISkill() {}
	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="depth"></param>
	virtual void Use(const int32_t depth) = 0;
};