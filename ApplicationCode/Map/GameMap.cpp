#include "GameMap.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"
#include "Easing.h"
#include "NormalEnemyA.h"
#include "NormalEnemyB.h"
#include "SoundManager.h"
#include "SafeDelete.h"
#include"KeyInput.h"
#include <random>

int Count = 0;

void GameMap::LoadCsv(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, int StageNum)
{

	float startX = 30.f;
	float startZ = 30.f;
	std::string line;
	int NUMBER = 0;
	int NEXTVERT = 0;
	int NEXTHORY = 0;
	int COUNT = 0;
	bool NEXTCOUNT = false;
	int ENEMYCOUNT = 0;
	XMFLOAT3 Pos = { startX ,0.f,startZ };

	std::stringstream stream;

	DrawingMap(StageNum, stream);


	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("KAIDAN") == 0) {
			getline(line_stream, word, ',');
			stairs_ = make_unique<Stairs>();
			XMFLOAT3 AddPos;
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			AddPos.x = x;
			AddPos.y = y;
			AddPos.z = z;
			stairs_->Initialize( Pos,AddPos ,player, COUNT);
			continue;
		}

		if (word.find("ENEMY1") == 0) {
			getline(line_stream, word, ',');
			ENEMYCOUNT = (int)std::atof(word.c_str());
			enemyscount_ = ENEMYCOUNT;
			gameenemyscount_ += ENEMYCOUNT;
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			CreateEnemy(player, Pos, ENEMYCOUNT, COUNT);
			continue;
		}

		if (word.find("ENEMY2") == 0) {
			getline(line_stream, word, ',');
			ENEMYCOUNT = (int)std::atof(word.c_str());
			enemyscount_ = ENEMYCOUNT;
			gameenemyscount_ += ENEMYCOUNT;
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			CreateEnemy2(player, Pos, ENEMYCOUNT, COUNT);
			continue;
		}

		if (word.find("BOX") == 0) {
			getline(line_stream, word, ',');
			int num = (int)std::atof(word.c_str());
			unique_ptr<TreasureBox> Box = make_unique<TreasureBox>();
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			Box->Initialize(num, Pos, player, COUNT);
			box_.push_back(move(Box));
			continue;
		}

		if (word.find("DEPO") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//リミット
			if (x >= 10.f) { x = 10.f; }
			else if (x <= -8.6f) { x = -8.6f; }
			if (z >= 8.2f) { z = 8.2f; }
			else if (z <= -11.f) { z = -11.f; }
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			XMFLOAT3 AddPos;
			AddPos.x = x;
			AddPos.y = -2.5f;
			AddPos.z = z;
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			Pos.x += AddPos.x;
			//Pos.y += AddPos.y;
			Pos.z += AddPos.z;
			CreateDeposits(Pos, COUNT);
			continue;
		}

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
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			maps_.push_back(move(Map));
			CreateGrass(Pos, COUNT);
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 2) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Boss;
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			Map->stage_->SetRotation({ 0.f,0.f,0.f });
			maps_.push_back(move(Map));
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 3) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Kaidan;
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			Count = COUNT;
			stairs_ = make_unique<Stairs>();
			stairs_->Initialize(Pos, {0.f,0.f,0.f}, player, Count);
			maps_.push_back(move(Map));
			CreateGrass(Pos, COUNT);
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 4) {
			count_ = COUNT;
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::Normal;
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
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
			CreateGrass(Pos, COUNT);
			NEXTVERT += 1;
			COUNT += 1;
		}
		else if (NUMBER == 5) {
			unique_ptr<Stage> Map = make_unique<Stage>();
			Map->stage_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("ground"));
			Map->num = COUNT;
			Map->state_ = Map::IfMap;
			Pos = { startX * NEXTVERT ,0.f,30.f * NEXTHORY };
			Map->stagePos_ = Pos;
			Map->stage_->SetPosition(Pos);
			Map->stage_->SetScale({ 0.1f,0.1f,0.1f });
			Map->invisible_ = true;
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
				Pos.x = Pos.x + 15;
				Bridges->bridge_->SetPosition(Pos);
				Bridges->bridge_->SetScale({ 5.f,0.5f,1.f });
				Bridges->bridge_->SetRotation({ 0.f,0.f,0.f });
				Bridges->bridge_->SetColor({ 0.9f,0.8f,0.6f,1.0f });
				Bridges->num = Map->num;
				Bridges->state_ = Direction::Beside;
				if (Map2->state_ == Map::IfMap || Map->state_ == Map::IfMap) {
					Bridges->invisible_ = true;
				}
				bridgeside.push_back(move(Bridges));
			}

			if (Map->stagePos_.z + 30 == Map2->stagePos_.z && Map->num + nextval_ == Map2->num) {
				unique_ptr<Bridge> Bridges = make_unique<Bridge>();
				Bridges->bridge_ = Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("bridge"));
				Bridges->state_ = Direction::Vertical;
				XMFLOAT3 Pos = Map->stagePos_;
				Pos.z = Pos.z + 15.f;
				Pos.x = Pos.x - 0.5f;
				Bridges->bridge_->SetPosition(Pos);
				Bridges->bridge_->SetScale({ 5.f,0.5f,1.f });
				Bridges->bridge_->SetRotation({ 0.f,90.f,0.f });
				Bridges->bridge_->SetColor({ 0.9f,0.8f,0.6f,1.0f });
				Bridges->num = Map->num;
				Bridges->state_ = Direction::Vertical;
				if (Map2->state_ == Map::IfMap || Map->state_ == Map::IfMap) {
					Bridges->invisible_ = true;
				}
				bridgevert.push_back(move(Bridges));
			}
		}
	}
}

void GameMap::CreateGrass(const XMFLOAT3& MapPos, int Count)
{
	//鉱石ドロップベクトル
	int Value = 0;
	//乱数生成
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(4, 10);
	Value = (int)rand(mt);

	//乱数生成
	std::random_device rnd2;
	std::mt19937 mt2(rnd2());
	std::uniform_int_distribution<> randX(-9, 9);
	std::uniform_int_distribution<> randZ(-8, 8);
	for (int i = 0; i < Value; i++) {
		float posX = MapPos.x + (float)randX(mt2);
		float posZ = MapPos.z + (float)randZ(mt2);
		unique_ptr<Grassland>GrassLand = make_unique<Grassland>();
		GrassLand->grass_ = std::make_unique<Grass>();
		GrassLand->grass_->Initialize({ posX, 0, posZ }, i);
		GrassLand->num = Count;
		grass_.push_back(move(GrassLand));
	}
}

void GameMap::CreateDeposits(const XMFLOAT3& MapPos, int MapNum)
{
	//鉱床
	std::unique_ptr<Deposit> deposit = make_unique<Deposit>();
	deposit->Initialize({ MapPos.x, MapPos.y,MapPos.z - 5.f });
	deposit->SetMapNum(MapNum);
	deposits_.push_back(std::move(deposit));
}

void GameMap::CreateEnemy(Player* player, const XMFLOAT3& MapPos, int Enemy, int Count)
{
	for (size_t i = 0; i < Enemy; i++) {
		unique_ptr<BaseEnemy> Enemy1 = make_unique<NormalEnemyA>();
		Enemy1->Init();
		Enemy1->SetPlayerIns(player);
		XMFLOAT3 MapMaxPos = { MapPos.x + limit_.x,100.f,MapPos.z + limit_.z };
		XMFLOAT3 MapMinPos = { MapPos.x - limit_.y,100.f,MapPos.z - limit_.w };
		Enemy1->SetOverPos(MapMaxPos, MapMinPos);
		Enemy1->SetCount(Count);
		//乱数生成
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> randX(-8, 8);
		std::uniform_int_distribution<> randZ(-7, 7);
		Enemy1->SetPos({ MapPos.x + (float)randX(mt),MapPos.y,MapPos.z + (float)randZ(mt) });
		enemys_.push_back(move(Enemy1));
	}
}

void GameMap::CreateEnemy2(Player* player, const XMFLOAT3& MapPos, int Enemy, int Count)
{
	for (size_t i = 0; i < Enemy; i++) {
		unique_ptr<BaseEnemy> Enemy1 = make_unique<NormalEnemyB>();
		Enemy1->Init();
		Enemy1->SetPlayerIns(player);
		XMFLOAT3 MapMaxPos = { MapPos.x + limit_.x,100.f,MapPos.z + limit_.z };
		XMFLOAT3 MapMinPos = { MapPos.x - limit_.y,100.f,MapPos.z - limit_.w };
		Enemy1->SetOverPos(MapMaxPos, MapMinPos);
		Enemy1->SetCount(Count);
		//乱数生成
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> randX(-8, 8);
		std::uniform_int_distribution<> randZ(-7, 7);
		Enemy1->SetPos2({ MapPos.x + (float)randX(mt),-2.5f,MapPos.z + (float)randZ(mt) });
		Enemy1->SetPosDeb(Enemy1->GetPos());
		enemys_.push_back(move(Enemy1));
	}
}

void GameMap::Initalize(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, int StageNum)
{
	LoadCsv(player, CameraPos, TargetPos, StageNum);

	CreateBridge();

	Vector3 Pos = player->GetPos();

	oldcount_ = count_;
}

void GameMap::Update(Player* player, XMFLOAT3& CameraPos, XMFLOAT3& TargetPos, float OldCameraPos, bool flag)
{
	for (int32_t i = 0; i < deposits_.size(); i++) {
		if (deposits_[i]->GetHP() <= 0) {
			deposits_.erase(deposits_.begin() + i);
		}
	}

	CheckHitTest(player);

	if (time_ < 1 && flag == true) {
		NextMap(player, CameraPos, TargetPos, OldCameraPos);
	}
	for (unique_ptr<Stage>& Map : maps_) {
		Map->stage_->Update();
	}

	for (unique_ptr<Bridge>& Bridge : bridgeside) {
		Bridge->bridge_->Update();
	}

	for (unique_ptr<Bridge>& Bridge : bridgevert) {
		Bridge->bridge_->Update();
	}

	for (unique_ptr<Grassland>& GrassLand : grass_) {
		GrassLand->grass_->Update(player->GetPos());
	}

	if (GameEnemyAllKill()) {
		for (unique_ptr<Bridge>& Bridge : bridgeside) {
			Bridge->invisible_ = false;
		}

		for (unique_ptr<Bridge>& Bridge : bridgevert) {
			Bridge->invisible_ = false;
		}
	}

	for (unique_ptr<TreasureBox>& Box : box_) {
		if ((Box != nullptr && Box->GetLock() == true)) {
			for (unique_ptr<Bridge>& Bridge : bridgeside) {
				Bridge->invisible_ = false;
			}

			for (unique_ptr<Bridge>& Bridge : bridgevert) {
				Bridge->invisible_ = false;
			}
		}
		if (Box != nullptr) {
			Box->Update();
		}
	}
	if (!stairs_.get()) { return; }
	stairs_->Update();

	

	for (int32_t i = 0; i < enemys_.size(); i++) {
		if (enemys_[i] == nullptr) { continue; }
		if (enemys_[i]->GetHP() <= 0) {
			player->AddEP(1);
			enemys_[i].release();
			enemyscount_ -= 1;
			gameenemyscount_ -= 1;
		}
	}
}

void GameMap::MapDraw()
{
	for (unique_ptr<Stage>& Map : maps_) {
		if (count_ == Map->num || oldcount_ == Map->num) {
			Map->stage_->Draw();
		}
		if (!stairs_.get()) { continue; }
		if (count_ == stairs_->GetCont()&&display_ == true) {
			stairs_->Draw();
		}
		if (Map->state_ == Map::Boss) { nowstate_ = Map->state_; }
	}
	if (!display_) { return; }
	for (unique_ptr<Grassland>& GrassLand : grass_) {
		if (count_ == GrassLand->num) {
			GrassLand->grass_->Draw();
		}
	}
	for (unique_ptr<TreasureBox>& Box : box_) {
		if (Box != nullptr) {
			if (Box->GetCont() != count_) {continue; }
			Box->Draw();
		}
	}
}

void GameMap::BridgeDraw(bool flag)
{
	if (flag == false) { return; }
	for (unique_ptr<Bridge>& Bridge : bridgevert) {
		if (nowstate_ == Map::Boss && time_ >= 1) { return; }
		if ((Bridge->num == count_ && Bridge->invisible_ == false) ||
			(Bridge->num == count_ - nextval_ && Bridge->state_ == Direction::Vertical && Bridge->invisible_ == false)) {
			Bridge->bridge_->Draw();
		}
	}
	for (unique_ptr<Bridge>& Bridge : bridgeside) {
		if (nowstate_ == Map::Boss && time_ >= 1) { return; }
		if ((Bridge->num == count_ && Bridge->invisible_ == false) ||
			(Bridge->num == count_ - 1 && Bridge->state_ == Direction::Beside && Bridge->invisible_ == false)) {
			Bridge->bridge_->Draw();
		}
	}


}

void GameMap::Finalize()
{
	maps_.clear();
	bridgeside.clear();
	bridgevert.clear();
	stairs_.release();
	grass_.clear();
	deposits_.clear();
	for (int32_t i = 0; i < enemys_.size(); i++) {
		enemys_[i].release();
	}
	box_.clear();
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
		//左
		if (PlayerPos.x >= Map->stagePos_.x + limit_.x) {
			PlayerPos.x = Map->stagePos_.x + limit_.x;
		}
		if (PlayerPos.x <= Map->stagePos_.x - limit_.y) {
			PlayerPos.x = Map->stagePos_.x - limit_.y;
		}
		if (PlayerPos.z >= Map->stagePos_.z + limit_.z) {
			PlayerPos.z = Map->stagePos_.z + limit_.z;
		}
		if (PlayerPos.z <= Map->stagePos_.z - limit_.w) {
			PlayerPos.z = Map->stagePos_.z - limit_.w;
		}
	}
	player->SetPos(PlayerPos);
}

void GameMap::CheckHitBridge(const XMFLOAT3& pos, int& Direction)
{
	for (unique_ptr<Stage>& Map : maps_) {
		for (unique_ptr<Bridge>& Bridge : bridgeside) {
			if (Map->num != Bridge->num) { continue; }
			XMFLOAT3 Pos = Bridge->bridge_->GetPosition();
			if (Bridge->state_ == Direction::Beside && Bridge->invisible_ == false) {
				if ((pos.z<Pos.z + 2 && pos.z>Pos.z - 2.f)) {
					//左に向かう
					if (pos.x > Pos.x  && Pos.x > pos.x - 4.95f) {
						nothit_ = true;
						wallHit_ = false;
						count_ = Bridge->num;
						direction_ = 1;
						stopCount_ = true;
						nowstate_ = Map->state_;
						return;
					}
					//右に向かう
					else if (pos.x < Pos.x && Pos.x - 5.f < pos.x) {
						nothit_ = true;
						wallHit_ = false;
						count_ = Bridge->num + 1;
						direction_ = 2;
						stopCount_ = true;
						nowstate_ = Map->state_;
						return;
					}
				}
			}
		}
		for (unique_ptr<Bridge>& Bridge : bridgevert) {
			if (Map->num != Bridge->num) { continue; }
			XMFLOAT3 Pos = Bridge->bridge_->GetPosition();
			if (Bridge->state_ == Direction::Vertical && Bridge->invisible_ == false) {
				if ((pos.x<Pos.x + 2 && pos.x>Pos.x - 2)) {
					if (pos.z > Pos.z - 6.1f && Pos.z > pos.z) {
						nothit_ = true;
						wallHit_ = false;
						count_ = Bridge->num + nextval_;
						direction_ = 3;
						stopCount_ = true;
						nowstate_ = Map->state_;
						return;
					}
					else if (pos.z < Pos.z + 3.f && Pos.z < pos.z) {
						nothit_ = true;
						wallHit_ = false;
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
	//移動中ではない
	if (player->GetNotNext()) { return; }
	count_ = NextCount(player->GetPos(), direction_);
	//プレイヤーがマップの端に来た時
	player->SetStop(true);
	float NextTarget = 0.f;
	XMFLOAT3 NextPos_ = GetNowMapPos();
	XMFLOAT3 PlayerPos = player->GetPos();
	XMFLOAT3 NEXTPLAYERPOS{};
	NextTarget = OldCameraPos + NextPos_.z - 2.f;

	if (direction_ == 0) { player->SetStop(false); display_ = true; return; }
	if (direction_ == 2) {
		NEXTPLAYERPOS.x = NextPos_.x - 9.f;
		NEXTPLAYERPOS.z = PlayerPos.z;
	}
	else if (direction_ == 1) {
		NEXTPLAYERPOS.x = NextPos_.x + 9.f;
		NEXTPLAYERPOS.z = PlayerPos.z;
	}
	else if (direction_ == 4) {
		NEXTPLAYERPOS.z = NextPos_.z + 8.f;
		NEXTPLAYERPOS.x = PlayerPos.x;
	}
	else if (direction_ == 3) {
		NEXTPLAYERPOS.z = NextPos_.z - 9.f;
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
	if (time_ < 0.3f) {
		display_ = false;
	}
	else {
		display_ = true;
	}
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
	if (StageNum == 0) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("TutorialMap.csv"); }
	else if (StageNum == 2 ) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map2.csv"); }
	else if (StageNum == 3 ) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map3.csv"); }
	else if (StageNum == 4 ) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map4.csv"); }
	else if (StageNum == 6 || StageNum == 12 || StageNum == 18) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("BossMap.csv"); }
	else if(StageNum == 8) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map5.csv"); }
	else if (StageNum == 9) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map6.csv"); }
	else if(StageNum==13){ stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map7.csv"); }
	else if(StageNum==14){ stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map8.csv"); }
	else if (StageNum == 15) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map9.csv"); }
	else if (StageNum == 16) { stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Map10.csv"); }


}

bool GameMap::EnemyAllKill()
{
	if (enemyscount_ <= 0) {
		return true;
	}
	else {
		return false;
	}
}

bool GameMap::GameEnemyAllKill()
{
	if (gameenemyscount_ <= 0) {
		return true;
	}
	else {
		return false;
	}
}

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
}

bool GameMap::ReflectHammer(XMFLOAT3& Pos, bool isHammerRelease)
{
	XMFLOAT3 pos = Pos;
	XMFLOAT3 Limit{};
	for (unique_ptr<Stage>& Map : maps_) {

		if (count_ != Map->num) { continue; }
		//左
		if (pos.x >= Map->stagePos_.x + limit_.x + 1 && isHammerRelease) {
			wallHit_ = true;
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerShake, 0.5f);
			return true;
		}
		else if (pos.x <= Map->stagePos_.x - limit_.y - 1 && isHammerRelease) {
			wallHit_ = true;
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerShake, 0.5f);
			return true;
		}

		else if (pos.z >= Map->stagePos_.z + limit_.z - 1 && isHammerRelease) {
			wallHit_ = true;
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerShake, 0.5f);
			return true;
		}

		else if (pos.z <= Map->stagePos_.z - limit_.w - 2 && isHammerRelease) {
			wallHit_ = true;
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerShake, 0.5f);
			return true;
		}
		
		else{ wallHit_ = false; }

	}

	return false;
}


Deposit* GameMap::GetDePosit()
{
	if (deposit_ != nullptr) {
		return deposit_;
	}
	return nullptr;
}


