#pragma once
#include "ISkill.h"

class StatusUpSkill : public ISkill
{
public:
	enum class StatusNumber {
		HPUP,
		ATKUP,
		DEFUP,
		SPDUP
	};

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="skillName">スキル名</param>
	StatusUpSkill(std::string skillName);

	/// <summary>
	/// スキル使用
	/// </summary>
	void AllUse(Player* player) override;

private: //メンバ変数
	//有効化フラグ
	bool isActive_;
	//上がるステータス
	StatusNumber upStatus_;
	//どれくらいステータスが上昇するか
	int32_t upStatusNumber_;
};

