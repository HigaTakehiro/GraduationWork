#pragma once
#include "SkillList.h"
#include "ISkill.h"
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
	void AddPlayerActiveSkill(ISkill* skill);

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
	SkillList* activeSkillList_;
	SkillList* passiveSkillList_;
};

