#include "ActiveSkillList.h"
#include "SafeDelete.h"

ActiveSkillList::ActiveSkillList(const std::string& name)
{
	name_ = name;
}

ActiveSkillList::~ActiveSkillList()
{
	for (auto skill : skillList_) {
		safe_delete(skill);
	}
	skillList_.clear();
}

void ActiveSkillList::Use(Player* player)
{
	if (player == nullptr) {
		return;
	}

	for (IActiveSkill* skill : skillList_) {
		skill->Use(player);
	}
}

void ActiveSkillList::SingleUse(Player* player, const std::string& name)
{
	if (player == nullptr) {
		return;
	}

	for (IActiveSkill* skill : skillList_) {
		if (skill->GetName() == name) {
			skill->Use(player);
		}
	}
}

void ActiveSkillList::SetIsActive(bool isActive)
{
	for (IActiveSkill* skill : skillList_) {
		skill->SetIsActive(isActive);
	}
}

void ActiveSkillList::SingleIsActive(const std::string& name, bool isActive)
{
	for (IActiveSkill* skill : skillList_) {
		if (skill->GetName() == name) {
			skill->SetIsActive(isActive);
		}
	}
}

void ActiveSkillList::AddSkill(IActiveSkill* skill)
{
	if (skill == nullptr) {
		return;
	}

	skillList_.emplace_back(skill);
}

bool ActiveSkillList::GetSkill(const std::string& name)
{
	for (IActiveSkill* skill : skillList_) {
		if (skill->GetName() == name) {
			return true;
		}
	}

	return false;
}
