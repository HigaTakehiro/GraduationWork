#pragma once
#include "ISkill.h"

class HPUpSkill : public ISkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�X�L����</param>
	/// <param name="num">HP�㏸��</param>
	HPUpSkill(const std::string& name, int32_t num);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~HPUpSkill() override = default;

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Use(class Player* player) override;

private: //�����o�ϐ�
	//HP�㏸��
	int32_t num_;
	//�L�����t���O
	bool isActive_;
};

