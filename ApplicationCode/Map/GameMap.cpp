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
	int COUNT = 0;
	bool NEXTCOUNT = false;
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
			if (!NEXTCOUNT) {
				if (COUNT == 1) {
					nextval_ = 1;
				}
				else if (COUNT == 2) {
					nextval_ = 2;
				}
				else if (COUNT == 3) {
					nextval_ = 3;
				}
				NEXTCOUNT = true;
			}
		}

		if (word.find("END") == 0) {
			return;
		}

		if (NUMBER == 0) {
			NEXTVERT += 1;
			COUNT += 1;
			continue;
		}
		if (NUMBER == 1) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Normal;
			Pos = { 30.f*NEXTVERT ,0.f,30.f*NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 2) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Forest;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 3) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Enemy;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
			COUNT += 1;
		}
	}
}

void GameMap::CreateBridge()
{
	for (unique_ptr<Stage>& Map : maps_) {
		for (unique_ptr<Stage>& Map2 : maps_) {
			if (Map->num == Map2->num) { continue; }
			if (Map->stagePos_.x + 30 == Map2->stagePos_.x && Map->num + 1 == Map2->num) {
				unique_ptr<Object3d> Bridge = make_unique<Object3d>();
				Bridge = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("bridge"));
				XMFLOAT3 Pos = Map->stagePos_;
				Pos.x = Pos.x + 16;
				Bridge->SetPosition(Pos);
				Bridge->SetScale({ 2.f,1.f,2.4f });
				Bridge->SetRotation({ 0.f,90.f,0.f });
				bridge_.push_back(move(Bridge));
			}

			if (Map->stagePos_.z + 30 == Map2->stagePos_.z && Map->num + nextval_ == Map2->num) {
				unique_ptr<Object3d> Bridge = make_unique<Object3d>();
				Bridge = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("bridge"));
				XMFLOAT3 Pos = Map->stagePos_;
				Pos.z = Pos.z + 16;
				Bridge->SetPosition(Pos);
				Bridge->SetScale({ 2.f,1.f,2.4f });
				bridge_.push_back(move(Bridge));
			}
		}
	}
}

void GameMap::Initalize()
{
	LoadCsv();


	CreateBridge();
	/*for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sta[j][i] = new Stage;
			sta[j][i]->stage_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			sta[j][i]->stagePos_ = { 30.f * j,0.f,30.f * i};
			sta[j][i]->stage_->SetScale({ 0.1f,0.1f,0.1f });
			sta[j][i]->stage_->SetPosition(sta[j][i]->stagePos_);
			sta[j][i]->num = Count;
			Count += 1;
		}
	}*/
}

void GameMap::Update()
{
	for (unique_ptr<Stage>& Map : maps_) {
		Map->stage_->Update();
	}

	for (unique_ptr<Object3d>& Bridge : bridge_) {
		Bridge->Update();
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

	for (unique_ptr<Object3d>& Bridge : bridge_) {
		Bridge->Draw();
	}

}

void GameMap::Finalize()
{
	maps_.clear();
	bridge_.clear();
}

void GameMap::CheckNowNumber(const XMFLOAT3& pos)
{
	int Value = 10;
	for (unique_ptr<Stage>& Map : maps_) {
		for (unique_ptr<Stage>& Map2 : maps_) {
			if (Map->num != count_) { continue; }
			//¶
			if (pos.x >= Map->stagePos_.x+10 && Map->num + 1 == Map2->num) {
				if (Map->stagePos_.x + 30 == Map2->stagePos_.x) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}
			//‰E
			if (pos.x <= Map->stagePos_.x - 5 && Map->num - 1 == Map2->num) {
				if (Map->stagePos_.x - 30 == Map2->stagePos_.x) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}

			if (pos.z >= Map->stagePos_.z + 10) {
				if (Map->stagePos_.z + 30 == Map2->stagePos_.z && Map->num + nextval_ == Map2->num) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}

			if (pos.z <= Map->stagePos_.z - 5) {
				if (Map->stagePos_.z - 30 == Map2->stagePos_.z && Map->num - nextval_ == Map2->num) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}
		}
	}
		
}

int GameMap::GetCount(const XMFLOAT3& pos)
{
	if (!stopCount_) {
		CheckNowNumber(pos);
	}
	return count_;
}

XMFLOAT3 GameMap::GetNowMapPos()
{
	for (unique_ptr<Stage>& Map : maps_) {
		if (count_==Map->num)
		{
			return Map->stagePos_;
		}
	}
}

