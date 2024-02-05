#include "FallHammerAttackSkill.h"

FallHammerAttackSkill::FallHammerAttackSkill(const std::string& name, int32_t coolTime)
{
	name_ = name;
	coolTime_ = coolTimer_ = coolTime;
	isActive_ = false;
}

void FallHammerAttackSkill::Use(Player* player)
{
	if (isActive_ == true && !player->GetIsHammerRelease() && !player->GetIsHammerSwing()) {
		coolTimer_ = 0;
 		player->ActiveFallHammer();
	}
	else {
		if (coolTimer_ <= coolTime_) coolTimer_++;
	}

	if (player->GetIsHammerReflect()) {
		isActive_ = false;
	}
}

void FallHammerAttackSkill::SetIsActive(bool isActive)
{
	if (coolTimer_ >= coolTime_) {
		isActive_ = isActive;
	}
}

bool FallHammerAttackSkill::GetIsActive()
{
	bool isActive = false;
	if (coolTimer_ >= coolTime_) isActive = true;

	return isActive;
}
