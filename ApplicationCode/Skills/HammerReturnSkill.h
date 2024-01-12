#pragma once
#include "ISkill.h"

class HammerReturnSkill : public ISkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">スキル名</param>
	HammerReturnSkill(const std::string& name);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HammerReturnSkill() override = default;

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Use(Player* player) override;

};

