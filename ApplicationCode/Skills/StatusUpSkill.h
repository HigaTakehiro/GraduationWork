#pragma once
#include "ISkill.h"

class StatusUpSkill : public ISkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="skillName">�X�L����</param>
	StatusUpSkill(std::string skillName) : skillName_(skillName) {};

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	void Use(const int32_t depth) override;

private: //�����o�ϐ�

	//�X�L����
	std::string skillName_;
};

