#include "SceneManager.h"

#include "SecBossScene.h"
#include"LastBossScene.h"
#include"ClearScene.h"

BaseScene* SceneManager::nowScene = nullptr;
BaseScene* SceneManager::skillScene_ = nullptr;
int32_t SceneManager::stageNo_ = 1;
int32_t SceneManager::score = 0;
CollisionManager* SceneManager::colManager_ = nullptr;
SkillManager* SceneManager::skillManager_ = nullptr;
int32_t SceneManager::level_ = 1;
int32_t SceneManager::ep_ = 0;
int32_t SceneManager::lvUpEp_ = 0;
int32_t SceneManager::hp_ = 3;
int32_t SceneManager::maxHp_ = 3;
int32_t SceneManager::atk_ = 1;
int32_t SceneManager::def_ = 1;
int32_t SceneManager::spd_ = 1;
int32_t SceneManager::skillPoint_ = 0;
bool SceneManager::isSkillScene_ = false;

void SceneManager::Initialize() {
	//マウスカーソルを非表示にする
	//ShowCursor(false);
	//DirectWrite初期化
	textDraw = std::make_unique<TextDraw>();
	textDraw->Initialize();
	//スキル管理クラス初期化
	skillManager_ = new SkillManager;
	//シーン切り替え
	//SceneChange(SceneName::Game);
	SceneChange(SceneName::Tutorial);

}

void SceneManager::Update() {
	if (!isSkillScene_) {
		nowScene->Update();
	}
	else {
		skillScene_->Update();
	}
}

void SceneManager::Draw() {
	if (!isSkillScene_) {
		nowScene->Draw();
	}
	else {
		skillScene_->Draw();
	}
}

void SceneManager::Finalize() {
	//現在のシーンの終了処理
	if (!isSkillScene_) {
		nowScene->Finalize();
	}
	if (skillScene_ != nullptr) {
		skillScene_->Finalize();
	}
	//ベースシーン解放
	safe_delete(nowScene);
	safe_delete(skillScene_);
	//スキルマネージャ解放
	safe_delete(skillManager_);
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
	isSkillScene_ = false;

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
		if (skillScene_ == nullptr) {
			skillScene_ = new IBScene();
		}
		skillScene_->SetSkillManager(skillManager_);
		skillScene_->Initialize();
		skillScene_->SetCollisionManager(colManager_);
		isSkillScene_ = true;
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
	case SceneName::Boss3:
		nowScene = new LastBossScene();
		NowSceneInitialize();
		break;
	case SceneName::Result:
		nowScene = new ResultScene();
		NowSceneInitialize();
		break;
	case SceneName::Clear:
		nowScene = new ClearScene();
		NowSceneInitialize();
		break;
	default:
		break;
	}
}