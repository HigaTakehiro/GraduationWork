#pragma once
#include <stdint.h>
#include <string>

class ISkill {
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ISkill() {};

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	virtual void UseSkill() = 0;
};