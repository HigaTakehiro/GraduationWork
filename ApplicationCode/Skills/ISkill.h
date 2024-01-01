#pragma once
#include <stdint.h>
#include <string>
#include "Player.h"

class ISkill {
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ISkill() {}
	/// <summary>
	/// 全スキル使用
	/// </summary>
	/// <param name="depth"></param>
	virtual void AllUse(Player* player) = 0;
	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns>名前</returns>
	std::string GetName() { return name_; }
protected: //メンバ変数
	/// <summary>
	/// 名前
	/// </summary>
	std::string name_;
};