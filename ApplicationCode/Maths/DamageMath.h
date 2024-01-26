#pragma once
#include <stdint.h>

class DamageMath
{
public: //メンバ関数
	DamageMath() = default;
	~DamageMath() = default;

public: //静的メンバ関数
	/// <summary>
	/// ダメージ計算
	/// </summary>
	/// <param name="atk">攻撃力</param>
	/// <param name="def">防御力</param>
	/// <returns>ダメージ</returns>
	static inline int32_t ReturnDamage(int32_t atk, int32_t def) {
		int32_t damage = 0;
		damage = (atk / 2) - (def / 4);
		if (damage <= 0) damage = 1;
		return damage;
	}
};

