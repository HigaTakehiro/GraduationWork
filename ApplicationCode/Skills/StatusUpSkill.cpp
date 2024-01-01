#include "StatusUpSkill.h"

StatusUpSkill::StatusUpSkill(std::string skillName)
{
	name_ = skillName;
	int32_t num = atoi(name_.c_str());
	if (name_.find("hp")) {
		upStatus_ = StatusNumber::HPUP;
	}
	else if (name_.find("atk")) {
		upStatus_ = StatusNumber::ATKUP;
	}
	else if (name_.find("def")) {
		upStatus_ = StatusNumber::DEFUP;
	}
	else if (name_.find("spd")) {
		upStatus_ = StatusNumber::SPDUP;
	}
	else {
		upStatus_ = StatusNumber::HPUP;
	}

	if (num > 0) {
		upStatusNumber_ = num;
	}
	else {
		upStatusNumber_ = 1;
	}
}

void StatusUpSkill::AllUse(Player* player)
{
	//printf(this->name_.c_str());
	if (!isActive_) {
		if (upStatus_ == StatusNumber::HPUP) {
			player->AddHP(upStatusNumber_);
		}
		else if (upStatus_ == StatusNumber::ATKUP) {

		}
		else if (upStatus_ == StatusNumber::DEFUP) {

		}
		else if (upStatus_ == StatusNumber::SPDUP) {

		}
		isActive_ = true;
	}
}
