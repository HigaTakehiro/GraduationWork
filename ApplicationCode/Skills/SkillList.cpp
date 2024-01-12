#include "SkillList.h"
#include "SafeDelete.h"

SkillList::SkillList(const std::string& name)
{
	name_ = name;
}

SkillList::~SkillList()
{
	for (auto skill : skillList_) {
		safe_delete(skill);
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

bool SkillList::GetSkill(const std::string& name)
{
	for (ISkill* skill : skillList_) {
		if (skill->GetName() == name) {
			return true;
		}
	}

	return false;
}
