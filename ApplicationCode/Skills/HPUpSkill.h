#pragma once
#include "ISkill.h"

class HPUpSkill : public ISkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">スキル名</param>
	/// <param name="num">HP上昇量</param>
	HPUpSkill(const std::string& name, int32_t num);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HPUpSkill() override = default;

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Use(class Player* player) override;

private: //メンバ変数
	//HP上昇量
	int32_t num_;
	//有効化フラグ
	bool isActive_;
};

