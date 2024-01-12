#include "HPUpSkill.h"

HPUpSkill::HPUpSkill(const std::string& name, int32_t num)
{
	//‰Šú‰»
	name_ = name;
	num_ = num;
	isActive_ = false;
}

void HPUpSkill::Use(Player* player)
{
	if (!isActive_) {
		player->AddMaxHP(num_);
		isActive_ = true;
	}
}
