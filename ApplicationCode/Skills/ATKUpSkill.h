#pragma once
#include "ISkill.h"

class ATKUpSkill : public ISkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�X�L����</param>
	/// <param name="num">�㏸���l</param>
	ATKUpSkill(const std::string& name, int32_t num);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ATKUpSkill() override = default;

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Use(class Player* player) override;

private: //�����o�ϐ�
	//�U���͏㏸���l
	int32_t num_;
	//�L�����t���O
	bool isActive_;
};

