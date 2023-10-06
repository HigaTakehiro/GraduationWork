#include "GameMap.h"
#include"Modelmanager.h"

int Count=0;

void GameMap::Initalize()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sta[j][i] = new Stage;
			sta[j][i]->stage_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			sta[j][i]->stagePos_ = { 50.f * j-100.f,0.f,50.f * i };
			sta[j][i]->stage_->SetScale({ 0.2f,0.2f,0.2f });
			sta[j][i]->stage_->SetPosition(sta[j][i]->stagePos_);
			sta[j][i]->num = Count;
			Count += 1;
		}
	}

	

}

void GameMap::Update()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sta[j][i]->stage_->Update();
		}
	}
}

void GameMap::Draw()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if(count_==sta[j][i]->num)
			sta[j][i]->stage_->Draw();
		}
	}
}

void GameMap::CheckNowNumber(const XMFLOAT3& pos)
{

	if ((pos.x < sta[0][0]->stagePos_.x + 15 && sta[0][0]->stagePos_.x - 15 < pos.x) && (pos.z < sta[0][0]->stagePos_.z + 15 && sta[0][0]->stagePos_.z - 15 < pos.z)) { count_ = sta[0][0]->num;}
	else if ((pos.x < sta[0][1]->stagePos_.x + 15 && sta[0][1]->stagePos_.x - 15 < pos.x) && (pos.z < sta[0][1]->stagePos_.z + 15 && sta[0][1]->stagePos_.z - 15 < pos.z)) {count_ = sta[0][1]->num;}
	else if ((pos.x < sta[0][2]->stagePos_.x + 15 && sta[0][2]->stagePos_.x - 15 < pos.x) && (pos.z < sta[0][2]->stagePos_.z + 15 && sta[0][2]->stagePos_.z - 15 < pos.z)) {count_ = sta[0][2]->num;}
	else if ((pos.x < sta[1][0]->stagePos_.x + 15 && sta[1][0]->stagePos_.x - 15 < pos.x) && (pos.z < sta[1][0]->stagePos_.z + 15 && sta[1][0]->stagePos_.z - 15 < pos.z)) {count_ = sta[1][0]->num;}
	else if ((pos.x < sta[1][1]->stagePos_.x + 15 && sta[1][1]->stagePos_.x - 15 < pos.x) && (pos.z < sta[1][1]->stagePos_.z + 15 && sta[1][1]->stagePos_.z - 15 < pos.z)) {count_ = sta[1][1]->num;}
	else if ((pos.x < sta[1][2]->stagePos_.x + 15 && sta[1][2]->stagePos_.x - 15 < pos.x) && (pos.z < sta[1][2]->stagePos_.z + 15 && sta[1][2]->stagePos_.z - 15 < pos.z)) {count_ = sta[1][2]->num;}
	else if ((pos.x < sta[2][0]->stagePos_.x + 15 && sta[2][0]->stagePos_.x - 15 < pos.x) && (pos.z < sta[2][0]->stagePos_.z + 15 && sta[2][0]->stagePos_.z - 15 < pos.z)) {count_ = sta[2][0]->num; }
	else if ((pos.x < sta[2][1]->stagePos_.x + 15 && sta[2][1]->stagePos_.x - 15 < pos.x) && (pos.z < sta[2][1]->stagePos_.z + 15 && sta[2][1]->stagePos_.z - 15 < pos.z)) {count_ = sta[2][1]->num; }
	else if ((pos.x < sta[2][2]->stagePos_.x + 15 && sta[2][2]->stagePos_.x - 15 < pos.x) && (pos.z < sta[2][2]->stagePos_.z + 15 && sta[2][2]->stagePos_.z - 15 < pos.z)) {count_ = sta[2][2]->num; }
}

int GameMap::GetCount(const XMFLOAT3& pos)
{
	CheckNowNumber(pos);

	return count_;
}

void GameMap::CheckState()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			
		}
	}


}
