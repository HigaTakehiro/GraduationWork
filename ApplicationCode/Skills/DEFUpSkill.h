#pragma once
#include "ISkill.h"
class DEFUpSkill : public ISkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">スキル名</param>
	/// <param name="num">上昇数値</param>
	DEFUpSkill(const std::string& name, int32_t num);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DEFUpSkill() override = default;

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Use(class Player* player) override;

private: //メンバ変数
	//攻撃力上昇数値
	int32_t num_;
	//有効化フラグ
	bool isActive_;
};

