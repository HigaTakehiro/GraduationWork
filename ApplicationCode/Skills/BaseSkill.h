#pragma once
#include "Player.h"
#include <stdint.h>
#include <string>

class BaseSkill {
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseSkill() {};

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player"></param>
	virtual void UseSkill(Player* player) = 0;

public: //�����o�֐�
	/// <summary>
	/// �X�L�����A�N�e�B�u��Ԃ��ǂ������擾
	/// </summary>
	bool GetIsActive() { return isActive_; }
	/// <summary>
	/// �X�L�������擾
	/// </summary>
	/// <returns></returns>
	std::string GetSkillName() { return skillName_; }
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="skillName">�X�L����</param>
	/// <param name="activeTime">�X�L���g�p����</param>
	/// <param name="recastTime">�X�L���Ďg�p����</param>
	void Initialize(std::string skillName, int32_t activeTime = 0, int32_t recastTime = 0);

protected: //�����o�ϐ�
	//�X�L����
	std::string skillName_;
	//�A�N�e�B�u�t���O
	bool isActive_;

	//�X�L���g�p����
	int32_t activeTime_;
	//�X�L���g�p�^�C�}�[
	int32_t activeTimer_;
	//�X�L���Ďg�p�^�C��
	int32_t recastTime_;
	//�X�L���Ďg�p�^�C�}�[
	int32_t recastTimer_;
};
