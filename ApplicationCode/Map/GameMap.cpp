#include "GameMap.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"

int Count=0;

void GameMap::LoadCsv()
{
	std::string line;
	int NUMBER = 0;
	int NEXTVERT = 0;
	int NEXTHORY = 0;
	XMFLOAT3 Pos= { 30.f ,0.f,30.f };

	std::stringstream stream;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map.csv");

	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');


		if (word.find("MAP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			NUMBER = x;
		}

		if (word.find("NEXT") == 0) {
			NEXTHORY += 1;
			NEXTVERT = 0;
			NUMBER = 99;
		}

		if (word.find("END") == 0) {
			return;
		}

		if (NUMBER == 0) {
			NEXTVERT += 1;
			continue;
		}
		if (NUMBER == 1) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = NUMBER;
			Map->state_ = Map::Normal;
			Pos = { 30.f*NEXTVERT ,0.f,30.f*NEXTHORY };
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
		}
		else if (NUMBER == 2) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = NUMBER;
			Map->state_ = Map::Forest;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
		}
		else if (NUMBER == 3) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = NUMBER;
			Map->state_ = Map::Enemy;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
		}

	

	}
}

void GameMap::Initalize()
{
	LoadCsv();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sta[j][i] = new Stage;
			sta[j][i]->stage_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			sta[j][i]->stagePos_ = { 30.f * j,0.f,30.f * i};
			sta[j][i]->stage_->SetScale({ 0.1f,0.1f,0.1f });
			sta[j][i]->stage_->SetPosition(sta[j][i]->stagePos_);
			sta[j][i]->num = Count;
			Count += 1;
		}
	}
}

void GameMap::Update()
{
	for (unique_ptr<Stage>& Map : maps_) {
		Map->stage_->Update();
	}

	/*for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sta[j][i]->stage_->Update();
		}
	}*/
}

void GameMap::Draw()
{
	for (unique_ptr<Stage>& Map : maps_) {
		Map->stage_->Draw();
	}

	//for (int i = 0; i < 3; i++) {
	//	for (int j = 0; j < 3; j++) {
	//	//	if(count_==sta[j][i]->num)
	//		sta[j][i]->stage_->Draw();
	//	}
	//}
}

void GameMap::Finalize()
{
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			delete sta[j][i];
		}
	}
}

void GameMap::CheckNowNumber(const XMFLOAT3& pos)
{
	int Value = 10;
	if ((pos.x < sta[0][0]->stagePos_.x + Value && sta[0][0]->stagePos_.x - Value < pos.x) && (pos.z < sta[0][0]->stagePos_.z + Value && sta[0][0]->stagePos_.z - Value < pos.z)) { count_ = sta[0][0]->num;}
	else if ((pos.x < sta[0][1]->stagePos_.x + Value && sta[0][1]->stagePos_.x - Value < pos.x) && (pos.z < sta[0][1]->stagePos_.z + Value && sta[0][1]->stagePos_.z - Value < pos.z)) {count_ = sta[0][1]->num;}
	else if ((pos.x < sta[0][2]->stagePos_.x + Value && sta[0][2]->stagePos_.x - Value < pos.x) && (pos.z < sta[0][2]->stagePos_.z + Value && sta[0][2]->stagePos_.z - Value < pos.z)) {count_ = sta[0][2]->num;}
	else if ((pos.x < sta[1][0]->stagePos_.x + Value && sta[1][0]->stagePos_.x - Value < pos.x) && (pos.z < sta[1][0]->stagePos_.z + Value && sta[1][0]->stagePos_.z - Value < pos.z)) {count_ = sta[1][0]->num;}
	else if ((pos.x < sta[1][1]->stagePos_.x + Value && sta[1][1]->stagePos_.x - Value < pos.x) && (pos.z < sta[1][1]->stagePos_.z + Value && sta[1][1]->stagePos_.z - Value < pos.z)) {count_ = sta[1][1]->num;}
	else if ((pos.x < sta[1][2]->stagePos_.x + Value && sta[1][2]->stagePos_.x - Value < pos.x) && (pos.z < sta[1][2]->stagePos_.z + Value && sta[1][2]->stagePos_.z - Value < pos.z)) {count_ = sta[1][2]->num;}
	else if ((pos.x < sta[2][0]->stagePos_.x + Value && sta[2][0]->stagePos_.x - Value < pos.x) && (pos.z < sta[2][0]->stagePos_.z + Value && sta[2][0]->stagePos_.z - Value < pos.z)) {count_ = sta[2][0]->num; }
	else if ((pos.x < sta[2][1]->stagePos_.x + Value && sta[2][1]->stagePos_.x - Value < pos.x) && (pos.z < sta[2][1]->stagePos_.z + Value && sta[2][1]->stagePos_.z - Value < pos.z)) {count_ = sta[2][1]->num; }
	else if ((pos.x < sta[2][2]->stagePos_.x + Value && sta[2][2]->stagePos_.x - Value < pos.x) && (pos.z < sta[2][2]->stagePos_.z + Value && sta[2][2]->stagePos_.z - Value < pos.z)) {count_ = sta[2][2]->num; }
}

int GameMap::GetCount(const XMFLOAT3& pos)
{
	CheckNowNumber(pos);

	return count_;
}

XMFLOAT3 GameMap::GetNowMapPos()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (count_ == sta[j][i]->num) {
				return sta[j][i]->stagePos_;
			}
		}
	}
}

