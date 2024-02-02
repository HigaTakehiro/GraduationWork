#pragma once
#include "SkillList.h"
#include "ISkill.h"
#include "IActiveSkill.h"
#include "ActiveSkillList.h"
#include "Player.h"

class SkillManager
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SkillManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SkillManager();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �v���C���[�̃C���X�^���X���Z�b�g
	/// </summary>
	/// <param name="player">�v���C���[�̃C���X�^���X</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// �v���C���[�̃X�L����ǉ�(�p�b�V�u�X�L��)
	/// </summary>
	/// <param name="skill">�v���C���[�p�b�V�u�X�L��</param>
	void AddPlayerPassiveSkill(ISkill* skill);

	/// <summary>
	/// �v���C���[�̃X�L����ǉ�(�A�N�e�B�u�X�L��)
	/// </summary>
	/// <param name="skill">�v���C���[�A�N�e�B�u�X�L��</param>
	void AddPlayerActiveSkill(IActiveSkill* skill);

	/// <summary>
	/// �A�N�e�B�u�X�L��01�����Z�b�g
	/// </summary>
	/// <param name="activeSkillName01">�A�N�e�B�u�X�L��01��</param>
	void SetActiveSkillName01(const std::string& activeSkillName01);

	/// <summary>
	/// �A�N�e�B�u�X�L��01��L����
	/// </summary>
	/// <param name="isActiveSkill01">�A�N�e�B�u�X�L��01�L�����t���O</param>
	void SetIsActiveSkill01(bool isActiveSkill01);

	/// <summary>
	/// �A�N�e�B�u�X�L��02�����Z�b�g
	/// </summary>
	/// <param name="activeSkillName02">�A�N�e�B�u�X�L��02��</param>
	void SetActiveSkillName02(const std::string& activeSkillName02);

	/// <summary>
	/// �A�N�e�B�u�X�L��02��L����
	/// </summary>
	/// <param name="isActiveSkill02">�A�N�e�B�u�X�L��02�L�����t���O</param>
	void SetIsActiveSkill02(bool isActiveSkill02);

	/// <summary>
	/// �X�L���������Ԃ��ǂ������擾
	/// </summary>
	/// <param name="skillName">�X�L����</param>
	/// <returns></returns>
	bool GetSkill(const std::string& skillName);

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

private: //�����o�ϐ�
	//�v���C���[
	Player* player_;
	//�v���C���[�X�L�����X�g
	ActiveSkillList* activeSkillList_;
	SkillList* passiveSkillList_;
	//�A�N�e�B�u�X�L��01
	std::string activeSkillName01_;
	//�A�N�e�B�u�X�L��02
	std::string activeSkillName02_;
};

