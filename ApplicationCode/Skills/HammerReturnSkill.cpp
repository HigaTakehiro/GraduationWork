#include "HammerReturnSkill.h"

HammerReturnSkill::HammerReturnSkill(const std::string& name)
{
	name_ = name;
}

void HammerReturnSkill::Use(Player* player)
{
	if (player->GetIsHammerReflect()) {
		player->HammerReturn();
	}
}
