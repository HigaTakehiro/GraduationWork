#include "IntermediateBase.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"
#pragma warning(disable:4996)
void IntermediateBase::Initialize()
{
	stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
	baseNo = 0;
	stage_->SetPosition({ -15,0,-50 });
	//stage_->SetScale({ 3,3,3 });
}

void IntermediateBase::Update()
{
	//baseNo++;
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
	fprintf(fp, "floor %d", floor);
	fclose(fp);
}

void IntermediateBase::LoadFloor()
{
	std::string line;
	std::stringstream stream;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("floor.csv");

	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("#") == 0) {
			continue;
		}
		if (word.find("floor") == 0) {
			line_stream >> baseNo;
		}
	}
}
