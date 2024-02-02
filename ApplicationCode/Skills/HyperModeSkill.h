#pragma once
#include "IActiveSkill.h"
class HyperModeSkill : public IActiveSkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�X�L����</param>
	/// <param name="coolTime">�N�[���^�C��</param>
	/// <param name="activeTime">�L������</param>
	HyperModeSkill(const std::string& name, int32_t coolTime, int32_t activeTime);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~HyperModeSkill() override = default;

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Use(Player* player) override;

	/// <summary>
	/// �X�L���L����
	/// </summary>
	/// <param name="isActive">�L�����t���O</param>
	void SetIsActive(bool isActive) override;

private: //�����o�ϐ�
	//�L�����ԃ^�C�}�[
	int32_t activeTimer_;
	//�L������
	int32_t activeTime_;
};

