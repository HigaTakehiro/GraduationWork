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
	/// �v���C���[�̃X�L����ǉ�
	/// </summary>
	/// <param name="skill">�v���C���[�X�L��</param>
	void AddPlayerSkill(ISkill* skill);

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
	SkillList* skillList_;
};

