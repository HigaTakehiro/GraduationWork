#include "SceneManager.h"

BaseScene* SceneManager::nowScene = nullptr;
int32_t SceneManager::stageNo_ = 1;
int32_t SceneManager::score = 0;
CollisionManager* SceneManager::colManager_ = nullptr;
int32_t SceneManager::level_ = 1;
int32_t SceneManager::ep_ = 0;

void SceneManager::Initialize() {
	//マウスカーソルを非表示にする
	//ShowCursor(false);
	//DirectWrite初期化
	textDraw = std::make_unique<TextDraw>();
	textDraw->Initialize();
	//シーン切り替え
	SceneChange(SceneName::Title);
}

void SceneManager::Update() {
	nowScene->Update();
}

void SceneManager::Draw() {
	nowScene->Draw();
}

void SceneManager::Finalize() {
	//現在のシーンの終了処理
	nowScene->Finalize();
	//ベースシーン解放
	safe_delete(nowScene);
	//カーソル可視化
	ShowCursor(true);
}

void SceneManager::AddScore(const int32_t score) {
	SceneManager::score += score;
}

void SceneManager::SetScore(const int32_t score)
{
	SceneManager::score = score;
}

void SceneManager::SetStageNo(const int32_t stageNo)
{
	stageNo_ = stageNo;
}

void SceneManager::SceneChange(SceneName scene) {
	if (nowScene != nullptr) {
		nowScene->Finalize();
		safe_delete(nowScene);
	}

	switch (scene) {
	case SceneName::Title:
		nowScene = new TitleScene();
		nowScene->Initialize();
		nowScene->SetCollisionManager(colManager_);
		break;
	case SceneName::Tutorial:
		nowScene = new TutorialScene();
		nowScene->Initialize();
		nowScene->SetCollisionManager(colManager_);
		break;
	case SceneName::Game:
		nowScene = new GameScene();
		nowScene->Initialize();
		nowScene->SetCollisionManager(colManager_);
		break;
	case SceneName::IB:
		nowScene = new IBScene();
		nowScene->Initialize();
		nowScene->SetCollisionManager(colManager_);
		break;
	case SceneName::SKILL:
		nowScene = new SkillScene();
		nowScene->Initialize();
		nowScene->SetCollisionManager(colManager_);
		break;
	case SceneName::Boss:
		nowScene = new BossScene();
		nowScene->Initialize();
		nowScene->SetCollisionManager(colManager_);
		break;
	case SceneName::Result:
		nowScene = new ResultScene();
		nowScene->Initialize();
		nowScene->SetCollisionManager(colManager_);
		break;
	default:
		break;
	}
}