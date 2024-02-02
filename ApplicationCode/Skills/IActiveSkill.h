#pragma once
#include "Player.h"
#include <string>

class IActiveSkill {
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IActiveSkill() {};

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	virtual void Use(Player* player) = 0;

	/// <summary>
	/// �X�L���L�����t���O���Z�b�g
	/// </summary>
	/// <param name="isActive">�X�L���L�����t���O</param>
	virtual void SetIsActive(bool isActive) = 0;

	/// <summary>
	/// �X�L�������擾
	/// </summary>
	/// <returns>�X�L����</returns>
	std::string GetName() const { return name_; }

	/// <summary>
	/// �X�L���L�����t���O���擾
	/// </summary>
	/// <returns>�X�L���L�����t���O</returns>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// �N�[���^�C�������ǂ������擾
	/// </summary>
	/// <returns>�N�[���^�C��</returns>
	bool GetCoolTime() { return coolTimer_ >= coolTime_; }

protected: //�����o�ϐ�
	//�X�L����
	std::string name_;
	//�X�L���L�����t���O
	bool isActive_;
	//�N�[���^�C��
	int32_t coolTime_;
	int32_t coolTimer_;
};