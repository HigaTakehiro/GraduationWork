#include "StatusUpSkill.h"

StatusUpSkill::StatusUpSkill(std::string skillName)
{
	name_ = skillName;
}

void StatusUpSkill::AllUse()
{
	printf(this->name_.c_str());
}
