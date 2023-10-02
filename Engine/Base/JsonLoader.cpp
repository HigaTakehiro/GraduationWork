#include "JsonLoader.h"
#include <istream>

const std::string JsonLoader::baseDirectory = "Engine/Resources/GameData/";
const std::string JsonLoader::extension = ".json";

void JsonLoader::StageDataLoadandSet(const std::string fileName) {
	const std::string fullpath = baseDirectory + fileName + extension;

	std::ifstream file;

	file.open(fullpath);
	if (file.fail()) {
		assert(0);
	}

	nlohmann::json deserialised;

	file >> deserialised;

	assert(deserialised.is_object());
	assert(deserialised.contains("name"));
	assert(deserialised["name"].is_string());

	std::string name = deserialised["name"].get<std::string>();

	assert(name.compare("scene") == 0);

	StageData* stageData = new StageData();

	for (nlohmann::json& object : deserialised["objects"]) {
		assert(object.contains("type"));

		std::string type = object["type"].get<std::string>();

		if (type.compare("MESH") == 0) {
			stageData->stageObjects_.emplace_back(StageData::ObjectData{});
			StageData::ObjectData& objectData = stageData->stageObjects_.back();

			if (object.contains("file_name")) {
				objectData.fileName_ = object["file_name"];
			}
			if (object["shader_type"] == "Wave") {
				objectData.isWave_ = true;
			}
			else {
				objectData.isWave_ = false;
			}

			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.transform_.x = (float)transform["translation"][0];
			objectData.transform_.y = (float)transform["translation"][1];
			objectData.transform_.z = (float)transform["translation"][2];
			//回転角
			objectData.rotation_.x = (float)transform["rotation"][0];
			objectData.rotation_.y = (float)transform["rotation"][1];
			objectData.rotation_.z = (float)transform["rotation"][2];
			//スケーリング
			objectData.scaling_.x = (float)transform["scaling"][0];
			objectData.scaling_.y = (float)transform["scaling"][1];
			objectData.scaling_.z = (float)transform["scaling"][2];
		}
	}

	for (auto& objectData : stageData->stageObjects_) {
		std::string modelName = objectData.fileName_;
		Object3d* newObject = Object3d::Create(ModelManager::GetIns()->GetModel(modelName));
		Vector3 pos;
		pos = objectData.transform_;
		newObject->SetPosition(pos);
		Vector3 rot;
		rot = objectData.rotation_;
		newObject->SetRotation(rot);
		Vector3 scale;
		scale = objectData.scaling_;
		newObject->SetScale(scale);
		newObject->SetAmbient({ 1.0f, 1.0f, 1.0f });
		if (objectData.isWave_ == true) {
			newObject->SetIsWave(true);
		}
		allObjects_.emplace_back(newObject);
	}

	delete(stageData);
	stageData = nullptr;
}

void JsonLoader::EnemyDataLoad(const std::string fileName)
{
	const std::string fullpath = baseDirectory + fileName + extension;

	std::ifstream file;

	file.open(fullpath);
	if (file.fail()) {
		assert(0);
	}

	file >> enemyJsonData_;
}

//void JsonLoader::EnemyDataUpdate(bool isPause)
//{
//	std::string name = enemyJsonData_["name"].get<std::string>();
//
//	assert(name.compare("scene") == 0);
//
//	EnemyData* enemyData = new EnemyData();
//
//	for (nlohmann::json& object : enemyJsonData_["objects"]) {
//		assert(object.contains("type"));
//
//		std::string type = object["type"].get<std::string>();
//
//		if (type.compare("MESH") == 0) {
//			enemyData->enemyObjects_.emplace_back(EnemyData::EnemyStatus{});
//			EnemyData::EnemyStatus& enemyStatus = enemyData->enemyObjects_.back();
//
//			if (object.contains("file_name")) {
//				enemyStatus.fileName_ = object["file_name"];
//			}
//
//			nlohmann::json& transform = object["transform"];
//			//平行移動
//			enemyStatus.transform_.x = (float)transform["translation"][0];
//			enemyStatus.transform_.y = (float)transform["translation"][1];
//			enemyStatus.transform_.z = (float)transform["translation"][2];
//			//回転角
//			enemyStatus.rotation_.x = (float)transform["rotation"][0];
//			enemyStatus.rotation_.y = (float)transform["rotation"][1];
//			enemyStatus.rotation_.z = (float)transform["rotation"][2];
//			//スケーリング
//			enemyStatus.scaling_.x = (float)transform["scaling"][0];
//			enemyStatus.scaling_.y = (float)transform["scaling"][1];
//			enemyStatus.scaling_.z = (float)transform["scaling"][2];
//			//敵タイプ
//			if (enemyJsonData_["enemy_type"] == "STR") {
//				enemyStatus.enemyType_ = EnemyType::STR;
//			}
//			else if (enemyJsonData_["enemy_type"] == "AIM") {
//				enemyStatus.enemyType_ = EnemyType::AIM;
//			}
//			//弾発射間隔
//			enemyStatus.shotCoolTime_ = (int32_t)object["shot_cooltime"] * 60;
//			//出現時間
//			enemyStatus.lifeTime_ = (int32_t)object["life_time"] * 60;
//			//HP
//			enemyStatus.hp_ = (int32_t)object["hp"];
//			//出現待機時間
//			enemyStatus.waitTime_ = (int32_t)object["wait_time"];
//		}
//	}
//
//	for (auto& objectData : enemyData->enemyObjects_) {
//		std::string modelName = objectData.fileName_;
//		if (objectData.enemyType_ == EnemyType::STR) {
//			std::unique_ptr<BaseEnemy> enemy = std::make_unique<StraightEnemy>();
//			enemy->Initialize(modelName, objectData.transform_, objectData.rotation_);
//			
//		}
//	}
//}

void JsonLoader::Update()
{
	const float waveTimer = 1200.0f;
	for (std::unique_ptr<Object3d>& object : allObjects_) {
		object->Update(waveTimer);
	}
}

void JsonLoader::Draw()
{
	for (std::unique_ptr<Object3d>& object : allObjects_) {
		object->Draw();
	}
}

void JsonLoader::Finalize()
{
	allObjects_.clear();
}
