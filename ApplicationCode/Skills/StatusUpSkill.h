#pragma once
#include "ISkill.h"

class StatusUpSkill : public ISkill
{
public:
	enum class StatusNumber {
		HPUP,
		ATKUP,
		DEFUP,
		SPDUP
	};

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="skillName">�X�L����</param>
	StatusUpSkill(std::string skillName);

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	void AllUse(Player* player) override;

private: //�����o�ϐ�
	//�L�����t���O
	bool isActive_;
	//�オ��X�e�[�^�X
	StatusNumber upStatus_;
	//�ǂꂭ�炢�X�e�[�^�X���㏸���邩
	int32_t upStatusNumber_;
};

