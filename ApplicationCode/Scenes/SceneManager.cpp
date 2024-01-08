#include "SceneManager.h"

#include "SecBossScene.h"

BaseScene* SceneManager::nowScene = nullptr;
int32_t SceneManager::stageNo_ = 1;
int32_t SceneManager::score = 0;
CollisionManager* SceneManager::colManager_ = nullptr;
SkillManager* SceneManager::skillManager_ = nullptr;
int32_t SceneManager::level_ = 1;
int32_t SceneManager::ep_ = 0;
int32_t SceneManager::hp_ = 3;
int32_t SceneManager::maxHp_ = 3;
int32_t SceneManager::atk_ = 1;
int32_t SceneManager::def_ = 1;
int32_t SceneManager::spd_ = 1;

void SceneManager::Initialize() {
	//マウスカーソルを非表示にする
	//ShowCursor(false);
	//DirectWrite初期化
	textDraw = std::make_unique<TextDraw>();
	textDraw->Initialize();
	//スキル管理クラス初期化
	skillManager_ = new SkillManager;
	//シーン切り替え
	SceneChange(SceneName::Tutorial);

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

void SceneManager::NowSceneInitialize()
{
	nowScene->SetSkillManager(skillManager_);
	nowScene->Initialize();
	nowScene->SetCollisionManager(colManager_);
}

void SceneManager::SceneChange(SceneName scene) {
	if (nowScene != nullptr) {
		nowScene->Finalize();
		safe_delete(nowScene);
	}

	switch (scene) {
	case SceneName::Title:
		nowScene = new TitleScene();
		NowSceneInitialize();
		break;
	case SceneName::Tutorial:
		nowScene = new TutorialScene();
		NowSceneInitialize();
		break;
	case SceneName::Game:
		nowScene = new GameScene();
		NowSceneInitialize();
		break;
	case SceneName::IB:
		nowScene = new IBScene();
		NowSceneInitialize();
		break;
	case SceneName::SKILL:
		nowScene = new SkillScene();
		NowSceneInitialize();
		break;
	case SceneName::Boss:
		nowScene = new BossScene();
		NowSceneInitialize();
		break;
	case SceneName::Boss2:
		nowScene = new SecBossScene();
		NowSceneInitialize();
		break;
	case SceneName::Result:
		nowScene = new ResultScene();
		NowSceneInitialize();
		break;
	default:
		break;
	}
}