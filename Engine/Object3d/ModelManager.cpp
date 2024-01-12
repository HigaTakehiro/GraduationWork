#include "ModelManager.h"

ModelManager::~ModelManager() {
	for (auto model : models_) {
		delete model.second;
	}
	for (auto model : fbxModels_) {
		delete model.second;
	}
	models_.clear();
	fbxModels_.clear();
}

ModelManager* ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::Initialize() {
	LoadModel("ground", "ground");
	LoadModel("bridge", "bridge");
	LoadModel("bossmap", "bossmap");
	LoadModel("Kaidan", "Kaidan");
	LoadModel("tunnel", "tunnel");
	LoadModel("rock", "rock");
	LoadModel("rock2", "rock2");
	LoadModel("rock3", "rock3");
	LoadModel("rock4", "rock4");
	LoadModel("rock5", "rock5");
	LoadModel("skydome", "skydome");
}

void ModelManager::LoadModel(const std::string modelName, const std::string modelKey, bool isSmoothing) {
	Model* newModel = nullptr;
	newModel = Model::CreateModel(modelName, isSmoothing);
	models_[modelKey] = newModel;
	newModel = nullptr;
	delete newModel;
}

void ModelManager::LoadFBXModel(const std::string modelName, const std::string modelKey)
{
	FBXModel* newModel = nullptr;
	newModel = FbxLoader::GetInstance()->LoadModelFromFile(modelName);
	fbxModels_[modelKey] = newModel;
	newModel = nullptr;
	delete newModel;
}
