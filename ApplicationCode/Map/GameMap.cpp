#include "GameMap.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"
#include "Easing.h"

int Count = 0;

void GameMap::LoadCsv(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, int StageNum)
{
	std::string line;
	int NUMBER = 0;
	int NEXTVERT = 0;
	int NEXTHORY = 0;
	int COUNT = 0;
	bool NEXTCOUNT = false;
	XMFLOAT3 Pos = { 30.f ,0.f,30.f };

	std::stringstream stream;

	DrawingMap(StageNum, stream);


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
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Normal;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
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
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Boss;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			Map->stage_->SetRotation({ 0.f,0.f,0.f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 5) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Kaidan;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			stairs_ = make_unique<Stairs>();
			stairs_->Initialize(Pos, player, Count);
			maps_.push_back(move(Map));
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 6) {
			count_ = COUNT;
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Normal;
			Pos = { 30.f * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			startpos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			Vector3 PlayerPos = player->GetPos();
			PlayerPos = PlayerPos + Pos;
			player->SetPos(PlayerPos);
			float NextTarget = 0;
			XMFLOAT3 NextPos_ = GetNowMapPos();
			CameraPos.x = startpos_.x;
			TargetPos.x = startpos_.x;
			TargetPos.z = startpos_.z;
			CameraPos.z = CameraPos.z + startpos_.z - 2.f;
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
				Bridges->bridge_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("tunnel"));
				Bridges->state_ = Direction::Beside;
				XMFLOAT3 Pos = Map->stagePos_;
				Pos.x = Pos.x + 17;
				Bridges->bridge_->SetPosition(Pos);
				Bridges->bridge_->SetScale({ 0.55f,0.5f,0.5f });
				Bridges->bridge_->SetRotation({ 0.f,0.f,0.f });
				Bridges->num = Map->num;
				Bridges->state_ = Direction::Beside;
				bridge.push_back(move(Bridges));
			}

			if (Map->stagePos_.z + 30 == Map2->stagePos_.z && Map->num + nextval_ == Map2->num) {
				unique_ptr<Bridge> Bridges = make_unique<Bridge>();
				Bridges->bridge_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("tunnel"));
				Bridges->state_ = Direction::Vertical;
				XMFLOAT3 Pos = Map->stagePos_;
				Pos.z = Pos.z + 15.f;
				Pos.x = Pos.x - 0.5f;
				Bridges->bridge_->SetPosition(Pos);
				Bridges->bridge_->SetScale({ 0.5f,0.5f,0.5f });
				Bridges->bridge_->SetRotation({ 0.f,90.f,0.f });
				Bridges->num = Map->num;
				Bridges->state_ = Direction::Vertical;
				bridge.push_back(move(Bridges));
			}
		}
	}
}

void GameMap::Initalize(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, int StageNum)
{
	LoadCsv(player, CameraPos, TargetPos, StageNum);

	CreateBridge();

	//CreateRock();

	oldcount_ = count_;
}

void GameMap::Update(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, float OldCameraPos)
{
	CheckHitTest(player);

	if (time_ < 1) {
		NextMap(player, CameraPos, TargetPos, OldCameraPos);
	}
	for (unique_ptr<Stage>& Map : maps_) {
		Map->stage_->Update();
	}

	for (unique_ptr<Bridge>& Bridge : bridge) {
		Bridge->bridge_->Update();
	}
	if (!stairs_.get()) { return; }
	stairs_->Update();
	/*for (unique_ptr<Object3d>& Rock : rock_) {
		Rock->Update();
	}*/
}

void GameMap::MapDraw()
{
	for (unique_ptr<Stage>& Map : maps_) {
		if (count_ == Map->num || oldcount_ == Map->num) {
			Map->stage_->Draw();
		}
		if (!stairs_.get()) { continue; }
		if (count_ == stairs_->GetCont()) {
			stairs_->Draw();
		}
		if (Map->state_ == Map::Boss) { nowstate_ = Map->state_; }
	}
	/*for (unique_ptr<Object3d>& Rock : rock_) {
		Rock->Draw();
	}*/
}

void GameMap::BridgeDraw()
{
	for (unique_ptr<Bridge>& Bridge : bridge) {
		if (nowstate_ == Map::Boss && time_ >= 1) { return; }
		if (Bridge->num == count_ ||
			(Bridge->num == count_ - nextval_ && Bridge->state_ == Direction::Vertical) ||
			(Bridge->num == count_ - 1 && Bridge->state_ == Direction::Beside)
			) {
			Bridge->bridge_->Draw();
		}
	}

}

void GameMap::Finalize()
{
	maps_.clear();
	bridge.clear();
	stairs_.release();
}

void GameMap::CheckHitTest(Player* player)
{
	XMFLOAT3 PlayerPos = player->GetPos();
	bool Flag = player->GetStop();
	if (Flag) {
		nothit_ = true;
	}
	if (nothit_ != false && time_ != 0.f) { return; }

	for (unique_ptr<Stage>& Map : maps_) {

		if (count_ != Map->num) { continue; }
		//��
		if (PlayerPos.x >= Map->stagePos_.x + 9.3f) {
			PlayerPos.x = Map->stagePos_.x + 9.3f;
		}
		if (PlayerPos.x <= Map->stagePos_.x - 7.f) {
			PlayerPos.x = Map->stagePos_.x - 7.f;
		}

		if (PlayerPos.z >= Map->stagePos_.z + 10.f) {
			PlayerPos.z = Map->stagePos_.z + 10.f;
		}

		if (PlayerPos.z <= Map->stagePos_.z - 6.f) {
			PlayerPos.z = Map->stagePos_.z - 6.f;
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
				if ((pos.z<Pos.z + 5 && pos.z>Pos.z + 2)) {
					if (pos.x > Pos.x + 2 && Pos.x + 6.f > pos.x) {
						nothit_ = true;
						count_ = Bridge->num;
						direction_ = 1;
						stopCount_ = true;
						nowstate_ = Map->state_;
						return;
					}
					else if (pos.x < Pos.x - 2 && Pos.x - 7.7f < pos.x) {
						nothit_ = true;
						count_ = Bridge->num + 1;
						direction_ = 2;
						stopCount_ = true;
						nowstate_ = Map->state_;
						return;
					}
				}
			}
			else if (Bridge->state_ == Direction::Vertical) {
				if ((pos.x<Pos.x + 2 && pos.x>Pos.x - 1)) {
					if (pos.z > Pos.z - 5 && Pos.z - 2 > pos.z) {
						nothit_ = true;
						count_ = Bridge->num + nextval_;
						direction_ = 3;
						stopCount_ = true;
						nowstate_ = Map->state_;
						return;
					}
					else if (pos.z < Pos.z + 9 && Pos.z + 2 < pos.z) {
						nothit_ = true;
						count_ = Bridge->num;
						direction_ = 4;
						stopCount_ = true;
						nowstate_ = Map->state_;
						return;
					}
				}
			}
		}
	}
	nothit_ = false;
	direction_ = 0;
}

int GameMap::NextCount(const XMFLOAT3& pos, int& Direction)
{
	if (!stopCount_) {
		CheckHitBridge(pos, Direction);
	}
	return count_;
}

XMFLOAT3 GameMap::GetNowMapPos()
{
	for (unique_ptr<Stage>& Map : maps_) {
		if (count_ == Map->num)
		{
			return Map->stagePos_;
		}
	}
}

void GameMap::NextMap(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, float OldCameraPos)
{

	//�ړ����ł͂Ȃ�
	if (player->GetNotNext()) { return; }
	count_ = NextCount(player->GetPos(), direction_);
	//�v���C���[���}�b�v�̒[�ɗ�����
	player->SetStop(true);
	float NextTarget = 0.f;
	XMFLOAT3 NextPos_ = GetNowMapPos();
	XMFLOAT3 PlayerPos = player->GetPos();
	XMFLOAT3 NEXTPLAYERPOS{};
	NextTarget = OldCameraPos + NextPos_.z - 2.f;

	if (direction_ == 0) { player->SetStop(false); return; }
	if (direction_ == 2) {
		NEXTPLAYERPOS.x = NextPos_.x - 5.f;
		NEXTPLAYERPOS.z = PlayerPos.z;
	}
	else if (direction_ == 1) {
		NEXTPLAYERPOS.x = NextPos_.x + 7.f;
		NEXTPLAYERPOS.z = PlayerPos.z;
	}
	else if (direction_ == 4) {
		NEXTPLAYERPOS.z = NextPos_.z + 9.f;
		NEXTPLAYERPOS.x = PlayerPos.x;
	}
	else if (direction_ == 3) {
		NEXTPLAYERPOS.z = NextPos_.z - 4.f;
		NEXTPLAYERPOS.x = PlayerPos.x;
	}


	time_ += 0.01f;
	CameraPos.x = Easing::easeIn(time_, 0.7f, CameraPos.x, NextPos_.x);
	TargetPos.x = Easing::easeIn(time_, 0.7f, TargetPos.x, NextPos_.x);
	CameraPos.z = Easing::easeIn(time_, 0.7f, CameraPos.z, NextTarget);
	TargetPos.z = Easing::easeIn(time_, 0.7f, TargetPos.z, NextPos_.z);
	PlayerPos.x = Easing::easeIn(time_, 0.3f, PlayerPos.x, NEXTPLAYERPOS.x);
	PlayerPos.z = Easing::easeIn(time_, 0.3f, PlayerPos.z, NEXTPLAYERPOS.z);

	player->SetPos(PlayerPos);
	if (time_ >= 0.7) {
		oldcount_ = count_; SetStop(false); player->SetStop(false);
		if (nowstate_ != Map::Boss) {
			time_ = 0;
		}
		else {
			time_ = 1;
			nothit_ = false;
		}
	}
}

void GameMap::DrawingMap(int StageNum, std::stringstream& stream)
{
	if (StageNum == 100) {
		stream = ExternalFileLoader::GetIns()->ExternalFileOpen("BossMap.csv");
	}
	else {
		stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map2.csv");
	}
}

//void GameMap::CreateRock()
//{
//	//�c
//	int Vert = -2;
//	//��
//	int Hori = -2;
//	XMFLOAT3 Pos{ 5.f,0.f,5.f };
//	while (rockPos_.z >= 70 && rockPos_.x >= 70) {
//		int Num = 0;
//		rockPos_.x += Pos.x * Hori;
//		rockPos_.z += Pos.z * Vert;
//		if (CheckRockToMap(rockPos_)) {
//			unique_ptr Rock = make_unique<Object3d>();
//			//if (Num == 0) { Rock = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("rock")); }
//			/*else if (Num == 1) { Rock = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("rock2")); }
//			else if (Num == 2) { Rock = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("rock3")); }
//			else if (Num == 3) { Rock = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("rock4")); }
//			else if (Num == 4) { Rock = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("rock5")); }*/
//			Rock->SetScale({ 1.f,1.f,1.f });
//			Rock->SetPosition(rockPos_);
//			rock_.emplace_back(Rock);
//		}
//		Hori += 1;
//		if (rockPos_.x >= 70.f) {
//			Hori = 0;
//			Vert += 1;
//		}
//	}
//
//}

bool GameMap::CheckRockToMap(const XMFLOAT3& RockPos)
{
	for (unique_ptr<Stage>& Map : maps_) {
		XMFLOAT3 Pos = Map->stagePos_;
		if ((Pos.x - 7.f < RockPos.x && Pos.x + 9.2f > RockPos.x) &&
			(Pos.z - 6.f < RockPos.z && Pos.z + 10.f > RockPos.z)) {
			return false;
		}
		return true;
	}
	return false;
}

void GameMap::ReflectHammer(XMFLOAT3& Pos)
{
	XMFLOAT3 Limit{};
	for (unique_ptr<Stage>& Map : maps_) {

		if (count_ != Map->num) { continue; }
		//��
		if (Pos.x >= Map->stagePos_.x + 9.3f) {
			Pos.x = Map->stagePos_.x + 9.3f;
		}
		if (Pos.x <= Map->stagePos_.x - 7.f) {
			Pos.x = Map->stagePos_.x - 7.f;
		}

		if (Pos.z >= Map->stagePos_.z + 10.f) {
			Pos.z = Map->stagePos_.z + 10.f;
		}

		if (Pos.z <= Map->stagePos_.z - 6.f) {
			Pos.z = Map->stagePos_.z - 6.f;
		}

	}
}


