#pragma once
#include "Player.h"
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
	/// <param name="player">�v���C���[</param>
	virtual void Use(Player* player) = 0;

	/// <summary>
	/// ���O�擾
	/// </summary>
	/// <returns>���O</returns>
	std::string GetName() { return name_; }

protected: //�����o�ϐ�
	//���O
	std::string name_;
};
