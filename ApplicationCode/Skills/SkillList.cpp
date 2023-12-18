#include "SkillList.h"

void SkillList::Use(const int32_t depth)
{
	std::string message = this->listName_;
	message += "\n";

	printf_s(message.c_str());

	for (ISkill* skill : mList_) {
		skill->Use(depth + 1);
	}
}

void SkillList::AddSkill(ISkill* skill)
{
	//ƒXƒLƒ‹‚ð’Ç‰Á
	this->mList_.emplace_back(skill);
}
