#pragma once
#include <stdint.h>

class DamageMath
{
public: //�����o�֐�
	DamageMath() = default;
	~DamageMath() = default;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �_���[�W�v�Z
	/// </summary>
	/// <param name="atk">�U����</param>
	/// <param name="def">�h���</param>
	/// <returns>�_���[�W</returns>
	static inline int32_t ReturnDamage(int32_t atk, int32_t def) {
		int32_t damage = 0;
		damage = (atk / 2) - (def / 4);
		if (damage <= 0) damage = 1;
		return damage;
	}
};

