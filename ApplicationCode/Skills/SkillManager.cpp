#include "SkillManager.h"
#include "SafeDelete.h"

SkillManager::SkillManager()
{
	player_ = nullptr;
	passiveSkillList_ = new SkillList("PassiveSkill");
	activeSkillList_ = new SkillList("ActiveSkill");
}

SkillManager::~SkillManager()
{
	safe_delete(passiveSkillList_);
	safe_delete(activeSkillList_);
}

void SkillManager::Update()
{
	if (player_ == nullptr) return;

	passiveSkillList_->Use(player_);
}

void SkillManager::AddPlayerPassiveSkill(ISkill* skill)
{
	if (skill == nullptr) return;

	passiveSkillList_->AddSkill(skill);
}

void SkillManager::AddPlayerActiveSkill(ISkill* skill)
{
	if (skill == nullptr) return;

	activeSkillList_->AddSkill(skill);
}

bool SkillManager::GetSkill(const std::string& skillName)
{
	if (passiveSkillList_->GetSkill(skillName)) return true;
	else if (activeSkillList_->GetSkill(skillName)) return true;
	else return false;
}

void SkillManager::Finalize()
{
	player_ = nullptr;
}
