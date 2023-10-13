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
