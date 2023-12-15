#pragma once
#include "ISkill.h"
#include <vector>

class SkillList : public ISkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="listName">���X�g��</param>
	SkillList(std::string listName) : listName_(listName) {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SkillList() = default;

	/// <summary>
	/// �S�X�L���g�p
	/// </summary>
	/// <param name="depth">�K�w</param>
	void Use(const int32_t depth) override;

	/// <summary>
	/// �X�L����ǉ�
	/// </summary>
	/// <param name="skill">�X�L��</param>
	void AddSkill(ISkill* skill);

private: //�����o�ϐ�
	/// <summary>
	/// �X�L�����X�g��
	/// </summary>
	std::string listName_;
	/// <summary>
	/// �X�L�����X�g
	/// </summary>
	std::vector<ISkill*> mList_;
};

