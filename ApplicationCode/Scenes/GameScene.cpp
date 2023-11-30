#include "GameScene.h"
//#include"//bossScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"

void GameScene::Initialize()
{
	ShowCursor(true);
	//ポストエフェクト初期化
	//画面大きさ設定
	const Vector3 LB = { -1.0f, -1.0f, 0.0f };
	const Vector3 LT = { -1.0f, +1.0f, 0.0f };
	const Vector3 RB = { +1.0f, -1.0f, 0.0f };
	const Vector3 RT = { +1.0f, +1.0f, 0.0f };
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initialize(LT, LB, RT, RB);

	//カメラ初期化
	CameraSetting();
	oldcamerapos_ = cameraPos_.z;
	//ライト初期化
	light_ = LightGroup::UniquePtrCreate();
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}
	//light->SetCircleShadowActive(0, true);
	Object3d::SetLight(light_.get());

	//3dオブジェクト初期化
	player_ = new Player;
	player_->Initialize();
	player_->SetLevel(SceneManager::GetLevel());
	player_->SetEP(SceneManager::GetEP());
	player_->SetHP(SceneManager::GetHP());

	postEffectNo_ = PostEffect::NONE;


	//後でcsvから
	unsigned int EnemySize = 3;

	enemys_.resize(EnemySize);
	vec.resize(EnemySize);

	for (size_t i = 0; i < enemys_.size(); i++) {
		enemys_[i] = new NormalEnemyA();
		enemys_[i]->Init();
		enemys_[i]->SetPlayerIns(player_);

		enemys_[i]->SetOverPos(XMFLOAT3(39.f, -100.f, 5.f), XMFLOAT3(23.f, 100.f, -5.f));
	}
	enemys_[0]->SetPos(Vector3(30, -30, -4));
	enemys_[2]->SetPos(Vector3(25, -30, 2));
	enemys_[2]->SetPos(Vector3(35, -30, 5));


	map_ = make_unique<GameMap>();
	map_->Initalize(player_, cameraPos_, targetPos_, 1);
	
	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFadeNum(1);
	schange->SetFEnd(true);

	SoundManager::GetIns()->StopAllBGM();
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::dungeon, TRUE, 0.4f);

	//草配置決め打ち
	std::unique_ptr<Grass> grass_1 = std::make_unique<Grass>();
	grass_1->Initialize({24, 0, 24});
	std::unique_ptr<Grass> grass_2 = std::make_unique<Grass>();
	grass_2->Initialize({ 24, 0, 35 });
	std::unique_ptr<Grass> grass_3 = std::make_unique<Grass>();
	grass_3->Initialize({ 35, 0, 24 });
	std::unique_ptr<Grass> grass_4 = std::make_unique<Grass>();
	grass_4->Initialize({ 24, 0, 8 });
	std::unique_ptr<Grass> grass_5 = std::make_unique<Grass>();
	grass_5->Initialize({ 35, 0, -9 });
	std::unique_ptr<Grass> grass_6 = std::make_unique<Grass>();
	grass_6->Initialize({ 37, 0, 8 });
	std::unique_ptr<Grass> grass_7 = std::make_unique<Grass>();
	grass_7->Initialize({ -3, 0, 2 });
	std::unique_ptr<Grass> grass_8 = std::make_unique<Grass>();
	grass_8->Initialize({ 5, 0, 8 });
	std::unique_ptr<Grass> grass_9 = std::make_unique<Grass>();
	grass_9->Initialize({ -3, 0, 35 });
	std::unique_ptr<Grass> grass_10 = std::make_unique<Grass>();
	grass_10->Initialize({ 5, 0, 24 });
	grasses_.push_back(std::move(grass_1));
	grasses_.push_back(std::move(grass_2));
	grasses_.push_back(std::move(grass_3));
	grasses_.push_back(std::move(grass_4));
	grasses_.push_back(std::move(grass_5));
	grasses_.push_back(std::move(grass_6));
	grasses_.push_back(std::move(grass_7));
	grasses_.push_back(std::move(grass_8));
	grasses_.push_back(std::move(grass_9));
	grasses_.push_back(std::move(grass_10));
}

void GameScene::Update()
{
	player_->Update();
	oreItems_.remove_if([](std::unique_ptr<Ore>& ore) {return ore == nullptr; });
	if (player_->GetHP() <= 0) {
		SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::dungeon);
	}
	for (std::unique_ptr<Grass>& grass : grasses_) {
		grass->Update(player_->GetPos());
	}

	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			if (ore->GetIsHit() && player_->GetIsHammerSwing() && !player_->OreCountOverMaxCount()) {
				player_->AddOreCount();
				ore = nullptr;
			}
		}
		if (ore != nullptr) {
			ore->Update();
		}
	}

	EnemyProcess();
	if (shake_->GetShakeFlag() == true) {
		cameraPos_.y += shake_->GetShakePos();
		targetPos_.y += shake_->GetShakePos();
	}
	else {
		targetPos_.y = 0;
	}
	cameraPos_.y = 12;
	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	light_->Update();

	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_);
	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition, player_->GetIsHammerRelease()));
	}
	//boss_->SetHummerPos(player_->GetHammer()->GetPosition());

	shake_->Update();
	colManager_->Update();

	//シーン切り替え
	schange->Change(0);
	SceneChange();
}

void GameScene::Draw()
{
	//背景色
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(背景)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	background_->Draw();
	Sprite::PostDraw();
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());

	map_->MapDraw();
	Object3d::PostDraw();
	for (auto i = 0; i < enemys_.size(); i++) {
		if (enemys_[i] != nullptr) {
			enemys_[i]->Draw();
		}
	}
	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	for (std::unique_ptr<Ore>& ore : oreItems_) {
		if (ore != nullptr) {
			ore->Draw();
		}
	}
	//boss_->Draw();
	//boss_->Draw2();
	for (size_t i = 0; i < enemys_.size(); i++)
		enemys_[i]->TexDraw();
	player_->Draw();
	for (std::unique_ptr<Grass>& grass : grasses_) {
		grass->Draw();
	}
	map_->BridgeDraw();

	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲
	//
	D2D1_RECT_F textDrawRange = {600, 0, 1280, 1280 };
	//std::wstring hx = std::to_wstring(player_->GetPos().z);
	//text_->Draw("meiryo", "white", L"ゲームシーン\n左クリックまたはLボタンでタイトルシーン\n右クリックまたはRボタンでリザルトシーン\nシェイクはEnter"+hx, textDrawRange);
	player_->TextUIDraw();
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//ポストエフェクトをかけないスプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	player_->SpriteDraw();
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}


void GameScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	//boss_->Finalize();
	safe_delete(player_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	colManager_->Finalize();
	map_->Finalize();
	grasses_.clear();
}

void GameScene::SceneChange()
{
	SceneManager::SetLevel(player_->GetLevel());
	SceneManager::SetEP(player_->GetEP());
	SceneManager::SetHP(player_->GetHP());

	bool Change = player_->GetNext();
	if (Change||player_->GetIsDead()) {
		schange->SetFStart(true);
		schange->SetFadeNum(0);
	}
	if (schange->GetEnd() == true) {
		SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::dungeon);
		SceneManager::SceneChange(SceneManager::SceneName::IB);
	}

}

void GameScene::CameraSetting()
{
	std::string line;
	Vector3 pos{};
	Vector3 target{};
	std::stringstream stream;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("CameraSetting.csv");

	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("#") == 0) {
			continue;
		}
		if (word.find("pos") == 0) {
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
		}
		if (word.find("target") == 0) {
			line_stream >> target.x;
			line_stream >> target.y;
			line_stream >> target.z;
		}

		cameraPos_ = pos;
		targetPos_ = target;

		camera_ = std::make_unique<Camera>();
		camera_->SetEye(cameraPos_);
		camera_->SetTarget(targetPos_);
	}
}

void GameScene::EnemyProcess()
{
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	Vector3 enemyPos[3] = {};

	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetHP() <= 0)
		{
			player_->AddEP(1);
			enemys_.erase(enemys_.begin() + i);
			continue;
		}
	}
	for (auto i = 0; i < enemys_.size(); i++) {
		if (enemys_[i]->GetHP() <= 0)continue;
		enemyPos[i] = enemys_[i]->GetPos();
		if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { enemyPos[i].x, enemyPos[i].z }, 1.0f) && !player_->GetIsHammerRelease() && player_->GetIsAttack()) {
			Vector3 playerPos = player_->GetPos();
			enemys_[i]->GetDamage();
			vec[i] = playerPos - enemyPos[i];
			vec[i].normalize();
			vec[i].y = 0.0f;
			player_->HitHammerToEnemy(vec[i]);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerAttack, 0.2f);
		}
	}


	//プレイヤーのOBB設定
	XMFLOAT3 trans = { player_->GetHammer()->GetMatWorld().r[3].m128_f32[0],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[1],
		player_->GetHammer()->GetMatWorld().r[3].m128_f32[2]
	};
	OBB l_obb;
	l_obb.SetParam_Pos(trans);
	l_obb.SetParam_Rot(player_->GetHammer()->GetMatRot());
	l_obb.SetParam_Scl({ 1.0f,2.10f,10.0f });

	_hummmerObb = &l_obb;

	for (size_t j = 0; j < enemys_.size(); j++)
	{
		for (size_t i = 0; i < enemys_.size(); i++)
		{
			if (i == j)continue;
			if (Collision::HitCircle(XMFLOAT2(enemys_[i]->GetPos().x, enemys_[i]->GetPos().z), 1.f,
				XMFLOAT2(enemys_[j]->GetPos().x, enemys_[j]->GetPos().z), 1.f))
			{
				XMFLOAT3 pos = enemys_[j]->GetPos();

				pos.x += sin(atan2f((enemys_[j]->GetPos().x - enemys_[i]->GetPos().x), (enemys_[j]->GetPos().z - enemys_[i]->GetPos().z))) * 0.3f;
				pos.z += cos(atan2f((enemys_[j]->GetPos().x - enemys_[i]->GetPos().x), (enemys_[j]->GetPos().z - enemys_[i]->GetPos().z))) * 0.3f;

				enemys_[j]->SetPos(pos);
			}
		}
	}
	for (auto i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetHP() <= 0) { continue; }
		if (enemys_[i] != nullptr) {
			enemys_[i]->SetHammerObb(*_hummmerObb);
			enemys_[i]->Upda(camera_.get());
		}
	}
}

