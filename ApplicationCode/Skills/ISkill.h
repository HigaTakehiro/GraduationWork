#pragma once
#include <stdint.h>
#include <string>

class ISkill {
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ISkill() {}
	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="depth"></param>
	virtual void Use(const int32_t depth) = 0;
};