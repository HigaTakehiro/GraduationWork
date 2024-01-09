#include "SPDUpSkill.h"

SPDUpSkill::SPDUpSkill(const std::string& name, int32_t num)
{
	name_ = name;
	num_ = num;
	isActive_ = false;
}

void SPDUpSkill::Use(Player* player)
{
	if (!isActive_) {
		player->AddSPD(num_);
		isActive_ = true;
	}
}
