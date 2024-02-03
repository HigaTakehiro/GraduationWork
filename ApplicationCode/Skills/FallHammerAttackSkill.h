#pragma once
#include "IActiveSkill.h"
class FallHammerAttackSkill : public IActiveSkill
{
public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">スキル名</param>
	/// <param name="coolTime">クールタイム</param>
	FallHammerAttackSkill(const std::string& name, int32_t coolTime);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FallHammerAttackSkill() override = default;

	/// <summary>
	/// スキル使用
	/// </summary>
	/// <param name="player"></param>
	void Use(Player* player) override;

	/// <summary>
	/// スキル有効化
	/// </summary>
	/// <param name="isActive">有効化フラグ</param>
	void SetIsActive(bool isActive) override;

};

