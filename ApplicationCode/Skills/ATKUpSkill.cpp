#include "ATKUpSkill.h"

ATKUpSkill::ATKUpSkill(const std::string& name, int32_t num)
{
	name_ = name;
	num_ = num;
	isActive_ = false;
}

void ATKUpSkill::Use(Player* player)
{
	if (!isActive_) {
		player->AddATK(num_);
		isActive_ = true;
	}
}
