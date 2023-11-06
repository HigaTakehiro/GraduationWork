#include "IntermediateBase.h"
#include"Modelmanager.h"
#pragma warning(disable:4996)
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

void IntermediateBase::FloorSave(int floor)
{
	FILE* fp;
	fp = fopen("Engine/Resources/GameData/floor.csv", "w");
	fprintf(fp, "%d", floor);
	fclose(fp);
}
