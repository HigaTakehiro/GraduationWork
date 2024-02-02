#include "HyperModeSkill.h"

HyperModeSkill::HyperModeSkill(const std::string& name, int32_t coolTime, int32_t activeTime)
{
	name_ = name;
	coolTime_ = coolTime;
	activeTime_ = activeTime;
	coolTimer_ = coolTime_;
	activeTimer_ = 0;
	isActive_ = false;
}

void HyperModeSkill::Use(Player* player)
{
	if (isActive_) {
		coolTimer_ = 0;
		if (activeTimer_ <= activeTime_) activeTimer_++;
		else {
			activeTimer_ = activeTime_;
			isActive_ = false;
		}
		player->SetIsInvicible(true);
	}
	else {
		activeTimer_ = 0;
		coolTimer_++;
		if (coolTimer_ >= coolTime_) coolTimer_ = coolTime_;
		player->SetIsInvicible(false);
	}
}

void HyperModeSkill::SetIsActive(bool isActive)
{
	if (coolTimer_ >= coolTime_) {
		isActive_ = isActive;
	}
}
