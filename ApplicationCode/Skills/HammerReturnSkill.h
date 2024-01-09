#pragma once
#include "ISkill.h"

class HammerReturnSkill : public ISkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�X�L����</param>
	HammerReturnSkill(const std::string& name);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~HammerReturnSkill() override = default;

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Use(Player* player) override;

};

