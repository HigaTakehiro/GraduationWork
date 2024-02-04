#include "SkillManager.h"
#include "KeyInput.h"
#include "PadInput.h"
#include "SafeDelete.h"

SkillManager::SkillManager()
{
	player_ = nullptr;
	passiveSkillList_ = new SkillList("PassiveSkill");
	activeSkillList_ = new ActiveSkillList("ActiveSkill");
	activeSkillName01_ = "None";
	activeSkillName02_ = "None";
}

SkillManager::~SkillManager()
{
	safe_delete(passiveSkillList_);
	safe_delete(activeSkillList_);
}

void SkillManager::Update()
{
	if (player_ == nullptr) return;

	if (KeyInput::GetIns()->TriggerKey(DIK_V) || KeyInput::GetIns()->TriggerKey(DIK_Q)) SetIsActiveSkill01(true);
	if (KeyInput::GetIns()->TriggerKey(DIK_B) || KeyInput::GetIns()->TriggerKey(DIK_E)) SetIsActiveSkill02(true);
	if (PadInput::GetIns()->TriggerButton(PadInput::Button_X)) SetIsActiveSkill01(true);
	if (PadInput::GetIns()->TriggerButton(PadInput::Button_Y)) SetIsActiveSkill02(true);

	passiveSkillList_->Use(player_);
	activeSkillList_->Use(player_);
}

void SkillManager::AddPlayerPassiveSkill(ISkill* skill)
{
	if (skill == nullptr) return;

	passiveSkillList_->AddSkill(skill);
}

void SkillManager::AddPlayerActiveSkill(IActiveSkill* skill)
{
	if (skill == nullptr) return;

	activeSkillList_->AddSkill(skill);
}

void SkillManager::SetActiveSkillName01(const std::string& activeSkillName01)
{
	activeSkillName01_ = activeSkillName01;
}

void SkillManager::SetIsActiveSkill01(bool isActiveSkill01)
{
	activeSkillList_->SingleIsActive(activeSkillName01_, isActiveSkill01);
}

void SkillManager::SetActiveSkillName02(const std::string& activeSkillName02)
{
	activeSkillName02_ = activeSkillName02;
}

void SkillManager::SetIsActiveSkill02(bool isActiveSkill02)
{
	activeSkillList_->SingleIsActive(activeSkillName02_, isActiveSkill02);
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
