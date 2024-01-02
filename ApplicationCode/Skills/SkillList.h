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
	~SkillList();

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Use(Player* player) override;

	/// <summary>
	/// �w��X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="name">�g�p�������X�L����</param>
	void SingleUse(Player* player, std::string& name);

	/// <summary>
	/// �X�L���ǉ�
	/// </summary>
	/// <param name="skill">�ǉ��X�L��</param>
	void AddSkill(ISkill* skill);

private: //�����o�ϐ�
	//�X�L�����X�g
	std::list<ISkill*> skillList_;
};

