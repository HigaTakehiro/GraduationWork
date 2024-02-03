#pragma once
#include "IActiveSkill.h"
class FallHammerAttackSkill : public IActiveSkill
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="name">�X�L����</param>
	/// <param name="coolTime">�N�[���^�C��</param>
	FallHammerAttackSkill(const std::string& name, int32_t coolTime);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FallHammerAttackSkill() override = default;

	/// <summary>
	/// �X�L���g�p
	/// </summary>
	/// <param name="player"></param>
	void Use(Player* player) override;

	/// <summary>
	/// �X�L���L����
	/// </summary>
	/// <param name="isActive">�L�����t���O</param>
	void SetIsActive(bool isActive) override;

};

