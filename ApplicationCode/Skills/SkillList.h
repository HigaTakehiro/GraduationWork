#pragma once
#include "ISkill.h"
#include <list>

class SkillList : public ISkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SkillList(const std::string& name);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SkillList() override;

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Use(class Player* player) override;

	/// <summary>
	/// �w��X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="name">�g�p�������X�L����</param>
	void SingleUse(class Player* player, std::string& name);

	/// <summary>
	/// �X�L���ǉ�
	/// </summary>
	/// <param name="skill">�ǉ��X�L��</param>
	void AddSkill(class ISkill* skill);

	/// <summary>
	/// �X�L���������Ԃ��ǂ������擾
	/// </summary>
	/// <param name="name">�m�F����X�L����</param>
	/// <returns>���肵�Ă��邩�ǂ���</returns>
	bool GetSkill(const std::string& name);

private: //�����o�ϐ�
	//�X�L�����X�g
	std::list<ISkill*> skillList_;
};

