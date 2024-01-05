#include "SkillManager.h"
#include "SafeDelete.h"

SkillManager::SkillManager()
{
	player_ = nullptr;
	skillList_ = nullptr;
}

SkillManager::~SkillManager()
{
	safe_delete(skillList_);
}
