#pragma once
#include "ISkill.h"

class StatusUpSkill : public ISkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="skillName">�X�L����</param>
	StatusUpSkill(std::string skillName);

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	void AllUse() override;

private: //�����o�ϐ�
};

