#include "SkillList.h"
#include "SafeDelete.h"

SkillList::SkillList(const std::string& name)
{
	name_ = name;
}

SkillList::~SkillList()
{
	for (auto itr = skillList_.begin(); itr == skillList_.end(); itr++) {
		safe_delete(*itr);
	}
	skillList_.clear();
}

void SkillList::Use(Player* player)
{
	if (player == nullptr) {
		return;
	}

	for (ISkill* skill : skillList_) {
		skill->Use(player);
	}
}

void SkillList::SingleUse(Player* player, std::string& name)
{
	if (player == nullptr) {
		return;
	}

	for (ISkill* skill : skillList_) {
		if (skill->GetName() == name) {
			skill->Use(player);
		}
	}
}

void SkillList::AddSkill(ISkill* skill)
{
	skillList_.emplace_back(skill);
}
