#include "BaseSkill.h"

void BaseSkill::Initialize(std::string skillName, int32_t activeTime, int32_t recastTime)
{
	skillName_ = skillName;
	activeTime_ = activeTime;
	recastTime_ = recastTime;
}
