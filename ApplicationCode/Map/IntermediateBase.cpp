#include "IntermediateBase.h"
#include"Modelmanager.h"
void IntermediateBase::Initialize()
{
	stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
}

void IntermediateBase::Update()
{
	stage_->Update();
}

void IntermediateBase::Draw()
{
	stage_->Draw();
}
