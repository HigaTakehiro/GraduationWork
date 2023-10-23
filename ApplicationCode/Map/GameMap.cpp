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
		else if (NUMBER == 4) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("bossmap"));
			Map->num = COUNT;
			Map->state_ = Map::Boss;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 4.15f,0.1f,14.57f });
			Map->stage_->SetRotation({ 0.f,0.f,0.f });
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
				unique_ptr<Bridge> Bridges = make_unique<Bridge>();
				Bridges->bridge_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("bridge"));
				Bridges->state_ = Direction::Beside;
				XMFLOAT3 Pos = Map->stagePos_;
				Pos.x = Pos.x + 16;
				Bridges->bridge_->SetPosition(Pos);
				Bridges->bridge_->SetScale({ 2.f,1.f,2.4f });
				Bridges->bridge_->SetRotation({ 0.f,90.f,0.f });
				Bridges->num = Map->num;
				Bridges->state_ = Direction::Beside;
				bridge.push_back(move(Bridges));
			}

			if (Map->stagePos_.z + 30 == Map2->stagePos_.z && Map->num + nextval_ == Map2->num) {
				unique_ptr<Bridge> Bridges = make_unique<Bridge>();
				Bridges->bridge_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("bridge"));
				Bridges->state_ = Direction::Vertical;
				XMFLOAT3 Pos = Map->stagePos_;
				Pos.z = Pos.z + 15;
				Bridges->bridge_->SetPosition(Pos);
				Bridges->bridge_->SetScale({ 2.f,1.f,2.4f });
				Bridges->num = Map->num;
				Bridges->state_ = Direction::Vertical;
				bridge.push_back(move(Bridges));
			}
		}
	}
}

void GameMap::Initalize()
{
	LoadCsv();


	CreateBridge();
	
}

void GameMap::Update()
{
	for (unique_ptr<Stage>& Map : maps_) {
		Map->stage_->Update();
	}
	
	for (unique_ptr<Bridge>& Bridge : bridge) {
		Bridge->bridge_->Update();
	}

}

void GameMap::Draw(int OldCount)
{
	for (unique_ptr<Stage>& Map : maps_) {
		if (count_ == Map->num||OldCount==Map->num) {
			Map->stage_->Draw();
		}
	}

	for (unique_ptr<Bridge>& Bridge : bridge) {
		if (Bridge->num == count_ || 
			(Bridge->num==count_ - nextval_&&Bridge->state_==Direction::Vertical)||
			(Bridge->num==count_-1&&Bridge->state_==Direction::Beside)
			) {
			Bridge->bridge_->Draw();
		}
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
			if (pos.x >= Map->stagePos_.x+11 && Map->num + 1 == Map2->num) {
				if (Map->stagePos_.x + 30 == Map2->stagePos_.x) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}
			//‰E
			if (pos.x <= Map->stagePos_.x - 6 && Map->num - 1 == Map2->num) {
				if (Map->stagePos_.x - 30 == Map2->stagePos_.x) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}

			if (pos.z >= Map->stagePos_.z + 11) {
				if (Map->stagePos_.z + 30 == Map2->stagePos_.z && Map->num + nextval_ == Map2->num) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}

			if (pos.z <= Map->stagePos_.z - 6) {
				if (Map->stagePos_.z - 30 == Map2->stagePos_.z && Map->num - nextval_ == Map2->num) {
					count_ = Map2->num;
					stopCount_ = true;
					return;
				}
			}
		}
	}
		
}

void GameMap::CheckHitTest(Player* player)
{
	if (nothit_ != false) { return; }
	XMFLOAT3 PlayerPos = player->GetPos();
	for (unique_ptr<Stage>& Map : maps_) {
		
		if(count_!=Map->num){continue;}
		//¶
		if (PlayerPos.x >= Map->stagePos_.x + 11 ) {
			PlayerPos.x = Map->stagePos_.x + 11;
		}
		if (PlayerPos.x <= Map->stagePos_.x - 6) {
			PlayerPos.x = Map->stagePos_.x - 6;
		}

		if (PlayerPos.z >= Map->stagePos_.z + 10) {
			PlayerPos.z = Map->stagePos_.z + 10;
		}

		if (PlayerPos.z <= Map->stagePos_.z - 6) {
			PlayerPos.z = Map->stagePos_.z - 6;
		}
	}
	player->SetPos(PlayerPos);
}


void GameMap::CheckHitBridge(const XMFLOAT3& pos, int& Direction)
{
	for (unique_ptr<Stage>& Map : maps_) {
		for (unique_ptr<Bridge>& Bridge : bridge) {
			if (Map->num != Bridge->num) { continue; }
			XMFLOAT3 Pos = Bridge->bridge_->GetPosition();
			if (Bridge->state_ == Direction::Beside) {
				if ((pos.z<Pos.z + 4 && pos.z>Pos.z - 1)) {
					if (pos.x > Pos.x + 2 && Pos.x + 4 > pos.x) {
						nothit_ = true;
						count_ = Bridge->num;
						Direction = 1;
						stopCount_ = true;
						return;
					}
					else if (pos.x < Pos.x - 2 && Pos.x - 4 < pos.x) {
						nothit_ = true;
						count_ = Bridge->num+1;
						Direction = 2;
						stopCount_ = true;
						return;
					}
				}
			}
			else if (Bridge->state_ == Direction::Vertical) {
				if ((pos.x<Pos.x + 2 && pos.x>Pos.x - 1)) {
					if (pos.z > Pos.z - 4 && Pos.z - 1 > pos.z) {
						nothit_ = true;
						count_ = Bridge->num + nextval_;
						Direction = 3;
						stopCount_ = true;
						return ;
					}
					else if (pos.z < Pos.z + 6 && Pos.z + 1 < pos.z) {
						nothit_ = true;
						count_ = Bridge->num;
						Direction = 4;
						stopCount_ = true;
						return;
					}
				}
			}
		}
	}
	nothit_ = false;
	Direction = 0;
	return;
}

int GameMap::GetCount(const XMFLOAT3& pos, int& Direction)
{
	if (!stopCount_) {
		CheckNowNumber(pos);
	}
	return count_;
}

int GameMap::NextCountconst(const XMFLOAT3& pos, int& Direction)
{
	if (!stopCount_) {
		CheckHitBridge(pos,Direction);
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
