#include "SkillList.h"

SkillList::SkillList(std::string listName)
{
	name_ = listName;
}

void SkillList::AllUse(Player* player)
{
	std::string message = name_;
	message += "\n";

	printf_s(message.c_str());

	for (ISkill* skill : mList_) {
		skill->AllUse(player);
	}
}

void SkillList::Use(const std::string& name, Player* player)
{
	for (ISkill* skill : mList_) {
		if (skill->GetName() == name) {
			skill->AllUse(player);
		}
	}
}

void SkillList::AddSkill(ISkill* skill)
{
	//ƒXƒLƒ‹‚ð’Ç‰Á
	this->mList_.emplace_back(skill);
}
