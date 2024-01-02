#pragma once
#include "Player.h"
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
	/// <param name="player">プレイヤー</param>
	virtual void Use(Player* player) = 0;

	/// <summary>
	/// 名前取得
	/// </summary>
	/// <returns>名前</returns>
	std::string GetName() { return name_; }

protected: //メンバ変数
	//名前
	std::string name_;
};
