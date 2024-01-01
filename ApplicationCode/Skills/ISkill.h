#pragma once
#include <stdint.h>
#include <string>
#include "Player.h"

class ISkill {
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ISkill() {}
	/// <summary>
	/// �S�X�L���g�p
	/// </summary>
	/// <param name="depth"></param>
	virtual void AllUse(Player* player) = 0;
	/// <summary>
	/// ���O���擾
	/// </summary>
	/// <returns>���O</returns>
	std::string GetName() { return name_; }
protected: //�����o�ϐ�
	/// <summary>
	/// ���O
	/// </summary>
	std::string name_;
};