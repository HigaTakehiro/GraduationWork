#pragma once
#include "IActiveSkill.h"

class ActiveSkillList : public IActiveSkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�X�L����</param>
	ActiveSkillList(const std::string& name);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ActiveSkillList() override;

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Use(Player* player) override;

	/// <summary>
	/// �w��X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="name">�X�L����</param>
	void SingleUse(class Player* player, const std::string& name);
	
	/// <summary>
	/// �S�X�L���L����
	/// </summary>
	/// <param name="isActive">�L�����t���O</param>
	void SetIsActive(bool isActive) override;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() override;

	/// <summary>
	/// �w��X�L���L����
	/// </summary>
	/// <param name="name">�X�L����</param>
	/// <param name="isActive">�X�L���L�����t���O</param>
	void SingleIsActive(const std::string& name, bool isActive);

	/// <summary>
	/// �w��X�L���L�����t���O�擾
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool GetSingleIsActive(const std::string& name);

	/// <summary>
	/// �X�L���ǉ�
	/// </summary>
	/// <param name="skill">�X�L��</param>
	void AddSkill(class IActiveSkill* skill);

	/// <summary>
	/// �X�L�����擾��Ԃ��ǂ������擾
	/// </summary>
	/// <param name="name">�X�L����</param>
	/// <returns>�擾���</returns>
	bool GetSkill(const std::string& name);

private:
	/// <summary>
	/// �X�L�����X�g
	/// </summary>
	std::list<IActiveSkill*> skillList_;
};

