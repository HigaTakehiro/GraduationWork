#include "DEFUpSkill.h"

DEFUpSkill::DEFUpSkill(const std::string& name, int32_t num)
{
	name_ = name;
	num_ = num;
	isActive_ = false;
}

void DEFUpSkill::Use(Player* player)
{
	if (!isActive_) {
		player->AddDEF(num_);
		isActive_ = true;
	}
}
