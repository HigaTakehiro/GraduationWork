#include "BossScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
//#include "Normal//enemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"
void BossScene::Initialize()
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

	boss_.reset(new Dogom());
	boss_->Init();
	boss_->SetPlayerIns(player_);


	map_ = make_unique<GameMap>();
	map_->Initalize(player_, cameraPos_, targetPos_,100);
	
	schange = new SceneChangeEffect();
	schange->Initialize();
	schange->SetFEnd(true);
	schange->SetFadeNum(1);

	shake_ = new Shake();
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
	boss_->GetCSPos(cameraPos_);

	m_Stairs.reset(new Stairs());
	m_Stairs->BossInitialize(Vector3(0, -3.5f, 0), player_);

	SoundManager::GetIns()->StopAllBGM();
}

void BossScene::Update()
{
	if (!boss_.get()) return;

	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::firstBoss,TRUE,0.4f);

	player_->Update();
	Vector3 hammerPos = player_->GetHammer()->GetMatWorld().r[3];
	//Vector3 //enemyPos[3] = {};

	
	
	//デバッグカメラ移動処理
	if (KeyInput::GetIns()->HoldKey(DIK_W)) {
		cameraPos_.z += 1.0f;
		targetPos_.z += 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_S)) {
		cameraPos_.z -= 1.0f;
		targetPos_.z -= 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_A)) {
		cameraPos_.x += 1.0f;
		targetPos_.x += 1.0f;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_D)) {
		cameraPos_.x -= 1.0f;
		targetPos_.x -= 1.0f;
	}
	//HPデバッグ処理
	if (KeyInput::GetIns()->TriggerKey(DIK_O)) {
		player_->SubHP(1);
	}
	if (KeyInput::GetIns()->TriggerKey(DIK_R)) {
		player_->SetHP(3);
	}

	/*if (shake_->GetShakeFlag() == true) {
		cameraPos_.y += shake_->GetShakePos();
		targetPos_.y += shake_->GetShakePos();
	}*/
	else {
		cameraPos_.y = 12;
		targetPos_.y = 0;

	}
	//if (boss_->GetAppearFlag() == FALSE) {
		camera_->SetEye(cameraPos_);
		camera_->SetTarget(targetPos_);
	//}//
		//boss_->SetCamera(camera_.get());
	light_->Update();

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

	boss_->Upda();

	map_->Update(player_, cameraPos_, targetPos_, oldcamerapos_);
	Vector3 hammerPosition = player_->GetHammer()->GetMatWorld().r[3];
	if (!player_->GetIsHammerReflect()) {
		player_->SetIsHammerReflect(map_->ReflectHammer(hammerPosition));
	}

		boss_->SetHummerPos(player_->GetHammer()->GetPosition());

		
		//shake_->Update();
	colManager_->Update();
		//boss_->SetHummerPos(player_->GetHammer()->GetPosition());
		
	schange->Change(0);
	
	m_Stairs->Update();
	//シーン切り替え
	SceneChange();if (boss_->GetClearF() && player_->GetNextFlor())
	{
		SceneManager::SceneChange(SceneManager::SceneName::Title);
	}
}

void BossScene::Draw()
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
	if(boss_->GetClearF())
	m_Stairs->Draw();

	Object3d::PostDraw();


	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());

	boss_->Draw();
	player_->Draw();
	map_->BridgeDraw();

	boss_->Draw2();
	Object3d::PostDraw();
	//shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲

	D2D1_RECT_F textDrawRange = { 0, 0, 700, 700 };
	std::wstring hp = boss_->GetStr();
	//text_->Draw("meiryo", "white", L"ボスシーン\n左クリックまたはLボタンでタイトルシーン\n右クリックまたはRボタンでリザルトシーン\nシェイクはEnter\nHP : " + hp, textDrawRange);
	player_->TextUIDraw();
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//ポストエフェクトをかけないスプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	player_->SpriteDraw();
	boss_->SpriteDraw();
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}

void BossScene::Finalize()
{
	safe_delete(text_);
	player_->Finalize();
	//boss_->Finalize();
	safe_delete(player_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	colManager_->Finalize();
	map_->Finalize();
}

void BossScene::SceneChange()
{
	//bool Change = player_->GetNext();
	//if (Change) {
	//	SceneManager::SceneChange(SceneManager::SceneName::Title);
	//}
	//if (/*MouseInput::GetIns()->TriggerClick(MouseInput::LEFT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_LB)) {
	//	SceneManager::SceneChange(SceneManager::SceneName::Title);
	//}
	//else if (/*MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK) || */PadInput::GetIns()->TriggerButton(PadInput::Button_RB)) {
	//	SceneManager::SceneChange(SceneManager::SceneName::Result);
	//}
}


void BossScene::CameraSetting()
{
	//if (boss_->GetAppearFlag())return;
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
