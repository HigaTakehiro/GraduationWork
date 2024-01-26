#include "IBScene.h"
#include "ExternalFileLoader.h"
#include "KeyInput.h"
#include "SoundManager.h"
#include "NormalEnemyA.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "Collision.h"
#include "Dogom.h"
#include "SoundManager.h"
#include"StageCount.h"
#include "ATKUpSkill.h"
#include "HammerReturnSkill.h"
#include "HPUpSkill.h"
#include "DEFUpSkill.h"
#include "SPDUpSkill.h"

void IBScene::Initialize()
{
	ShowCursor(true);
	//ポストエフェクト初期化
	//画面大きさ設定
	const Vector3 LB = { -1.0f, -1.0f, 0.0f };
	const Vector3 LT = { -1.0f, +1.0f, 0.0f };
	const Vector3 RB = { +1.0f, -1.0f, 0.0f };
	const Vector3 RT = { +1.0f, +1.0f, 0.0f };
	if (postEffect_ == nullptr) {
		postEffect_ = std::make_unique<PostEffect>();
	}
	postEffect_->Initialize(LT, LB, RT, RB);

	//カメラ初期化
	CameraSetting();
	oldcamerapos_ = cameraPos_.z;
	//ライト初期化
	if (light_ == nullptr) {
		light_ = LightGroup::UniquePtrCreate();
	}
	for (int32_t i = 0; i < 3; i++) {
		light_->SetDirLightActive(0, true);
		light_->SetPointLightActive(i, false);
		light_->SetSpotLightActive(i, false);
	}
	//light->SetCircleShadowActive(0, true);
	Object3d::SetLight(light_.get());

	//3dオブジェクト初期化
	for (int32_t i = 0; i < 4; i++) {
		if (playerModel_[i] == nullptr) {
			playerModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_rest.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 1.0f }, { 128.0f, 128.0f }, true);
		}
		if (skillPlayer_[i] == nullptr) {
			skillPlayer_[i] = Sprite::UniquePtrCreate((UINT)ImageManager::Image2DName::IdlePlayer, { 150.f, 600.f }, { 1.f, 1.f, 1.f, 1.f }, { 0.5f, 0.5f });
			skillPlayer_[i]->SetTextureRect({ (float)i * 128.f, 2.f }, { 128.f, 128.f });
			skillPlayer_[i]->SetSize({ 128.f, 128.f });
		}
	}

	if (fireModel_ == nullptr) {
		fireModel_ = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "fire.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * 0, 1.0f }, { 128.0f, 128.0f }, true);
	}
	if (player_ == nullptr) {
		player_ = Object3d::UniquePtrCreate(playerModel_[0]);
	}
	player_->SetIsBillboardY(true);
	player_->SetScale({ 0.15f, 0.15f, 0.15f });
	if (fire_ == nullptr) {
		fire_ = Object3d::UniquePtrCreate(fireModel_);
	}
	fire_->SetIsBillboardY(true);
	fire_->SetScale({ 0.1f, 0.1f, 0.1f });

	if (skillB_ == nullptr) {
		skillB_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillButton, { 1000, 50 }, { 1,1,1,1 }, { 0.0f, 0.0f });
	}
	if (skillSprite_ == nullptr) {
		skillSprite_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skill, { 0, 0 }, { 1,1,1,1 }, { 0.0f, 0.0f });
	}
	if (susumu_ == nullptr) {
		susumu_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::susumuButton, { 1000, 150 }, { 1,1,1,1 }, { 0.0f, 0.0f });
	}
	if (arrow == nullptr) {
		arrow = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::Arrow, { 900, 50 }, { 1,1,1,1 }, { 0.0f, 0.0f });
	}
	postEffectNo_ = PostEffect::NONE;

	if (shake_ == nullptr) {
		shake_ = new Shake();
	}
	shake_->Initialize(DirectXSetting::GetIns()->GetDev(), camera_.get());

	if (ib_ == nullptr) {
		ib_ = new IntermediateBase();
	}
	ib_->Initialize();
	ib_->LoadFloor();

	//スキル画面
	//スキルパネル
	if (panelStatus_[0][3].skillPanel_ == nullptr) {
		SkillPanelInitialize();
	}

	//カーソルUI
	if (skillCursor_ == nullptr) {
		skillCursor_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillCursor, { 640.f, 300.f }, { 1.f, 1.0f, 1.0f, 1.f }, { 0.5f, 0.5f });
	}
	skillCursor_->SetSize({ 50.f, 50.f });

	if (schange == nullptr) {
		schange = new SceneChangeEffect();
	}
	schange->Initialize();
	schange->SetFadeNum(1);
	schange->SetFEnd(true);
	/*baseNo = 0;*/
	baseNo = ib_->GetBaseNo();
	animeTimer_ = 0;
	preAnimeCount_ = 999;
	animeSpeed_ = 8;
	if (background_ == nullptr) {
		background_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::background, { 0, 0 });
	}
	baseCount = 0;
	if (playerUI_ == nullptr) {
		playerUI_ = new Player();
	}
	playerUI_->Initialize();
	playerUI_->SetLevel(SceneManager::GetLevel());
	playerUI_->SetEP(SceneManager::GetEP());
	playerUI_->SetHP(SceneManager::GetHP());
	playerUI_->SetMaxHP(SceneManager::GetMaxHP());
	playerUI_->SetATK(SceneManager::GetATK());
	playerUI_->SetDEF(SceneManager::GetDEF());
	playerUI_->SetSPD(SceneManager::GetSPD());
	playerUI_->SetSkillPoint(SceneManager::GetSkillPoint());
	hp_ = playerUI_->GetHP();
	playerUI_->SetStop(true);
	SoundManager::GetIns()->StopAllBGM();
	SoundManager::GetIns()->PlayBGM(SoundManager::BGMKey::restPoint, TRUE, 0.4f);
	int Num = StageCount::GetIns()->Up();

	skillManager_->SetPlayer(playerUI_);
}

void IBScene::Update()
{
	Animation();

	//スキルパネル更新
	panelStatus_[3][3].skillPanel_->Update(skillCursor_->GetPosition());
	for (int32_t i = 0; i < 7; i++) {
		if (i != 3) {
			panelStatus_[i][3].skillPanel_->Update(skillCursor_->GetPosition());
			panelStatus_[3][i].skillPanel_->Update(skillCursor_->GetPosition());
		}

		if (panelStatus_[i][3].skillPanel_->PanelToCursorHit(skillCursor_->GetPosition()) && (KeyInput::GetIns()->TriggerKey(DIK_SPACE) || PadInput::GetIns()->TriggerButton(PadInput::Button_A))) {
			if (playerUI_->GetSkillPoint() > 0 && panelStatus_[i][3].skillPanel_->GetIsActive() && !panelStatus_[i][3].skillPanel_->GetIsSkillGet()) {
				playerUI_->SubSkillPoint(1);
				panelStatus_[i][3].skillPanel_->SetIsSkillGet(true);
				AddSkill(i, 3);
			}
		}
		if (panelStatus_[3][i].skillPanel_->PanelToCursorHit(skillCursor_->GetPosition()) && (KeyInput::GetIns()->TriggerKey(DIK_SPACE) || PadInput::GetIns()->TriggerButton(PadInput::Button_A))) {
			if (playerUI_->GetSkillPoint() > 0 && panelStatus_[3][i].skillPanel_->GetIsActive() && !panelStatus_[3][i].skillPanel_->GetIsSkillGet()) {
				playerUI_->SubSkillPoint(1);
				panelStatus_[3][i].skillPanel_->SetIsSkillGet(true);
				AddSkill(3, i);
			}
		}
	}


	if (KeyInput::GetIns()->TriggerKey(DIK_P) && playerUI_->GetSkillPoint() > 0) {
		playerUI_->SubSkillPoint(1);
		if (!panelStatus_[3][3].skillPanel_->GetIsSkillGet()) {
			panelStatus_[3][3].skillPanel_->SetIsSkillGet(true);
			if (!skillManager_->GetSkill("HammerReturn")) {
				HammerReturnSkill* returnSkill = new HammerReturnSkill("HammerReturn");
				skillManager_->AddPlayerPassiveSkill(returnSkill);
			}
		}
		if (panelStatus_[2][3].skillPanel_->GetIsActive() && !panelStatus_[2][3].skillPanel_->GetIsSkillGet()) {
			panelStatus_[2][3].skillPanel_->SetIsSkillGet(true);
			ATKUpSkill* atkUp = new ATKUpSkill("atkUP", 5);
			skillManager_->AddPlayerPassiveSkill(atkUp);
		}
		if (panelStatus_[1][3].skillPanel_->GetIsActive() && !panelStatus_[1][3].skillPanel_->GetIsSkillGet()) {
			panelStatus_[1][3].skillPanel_->SetIsSkillGet(true);
			ATKUpSkill* atkUp = new ATKUpSkill("atkUP", 5);
			skillManager_->AddPlayerPassiveSkill(atkUp);
		}
	}
	if (KeyInput::GetIns()->TriggerKey(DIK_U) && KeyInput::GetIns()->TriggerKey(DIK_I)) {
		playerUI_->AddSkillPoint(99);
	}
	SkillUIUpdate();

	player_->SetPosition({ -8.0f,2.5f, 8.0f });
	player_->Update();
	fire_->SetPosition({ 0.0f,2.5f, 8.0f });
	fire_->Update();
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

	if (shake_->GetShakeFlag() == true) {
		cameraPos_.y += shake_->GetShakePos();
		targetPos_.y += shake_->GetShakePos();
	}
	else {
		cameraPos_.y = 12;
		targetPos_.y = 0;
	}

	camera_->SetEye(cameraPos_);
	camera_->SetTarget(targetPos_);
	light_->Update();
	playerUI_->Update();
	playerUI_->SetHP(playerUI_->GetMaxHP());

	if (hp_ != 0) {
		if (baseCount < 2) {
			baseCount++;
		}
		if (baseCount == 1) {
			baseNo++;
		}
	}
	else {
		baseNo = ib_->GetBaseNo();
	}

	ib_->Update();
	ib_->FloorSave(baseNo);

	skillB_->SetSize({ 256, 64 });
	//skillSprite_->SetSize({ 1280, 720 });
	susumu_->SetSize({ 256, 64 });
	if (count2 < 6) {
		count2++;
	}
	if (count2 == 6) {
		if (count < 4) {
			count++;
		}
		else {
			count = 0;
		}
		count2 = 0;
	}
	schange->Change(0);
	skillManager_->Update();

	arrow->SetTextureRect({ 0 + 64 * count,0 }, { 64,64 });
	arrow->SetSize({ 64,64 });
	//shake_->Update();
	//colManager_->Update();
	//シーン切り替え
	SceneChange();
}

void IBScene::Draw()
{
	//背景色
	const DirectX::XMFLOAT4 backColor = { 0.5f,0.25f, 0.5f, 0.0f };

	postEffect_->PreDrawScene(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(背景)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	background_->Draw();
	Sprite::PostDraw();
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	ib_->Draw();
	Object3d::PostDraw();
	//3Dオブジェクト描画処理
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	player_->Draw();
	fire_->Draw();
	Object3d::PostDraw();
	shake_->Draw(DirectXSetting::GetIns()->GetCmdList());

	//スプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	if (skillFlag == false) {
		skillB_->Draw();
		susumu_->Draw();
		arrow->Draw();
	}
	else if (skillFlag == true) {
		skillSprite_->Draw();
		for (int32_t i = 0; i < 7; i++) {
			for (int32_t j = 0; j < 7; j++) {
				if (panelStatus_[i][j].panelStatus_ != 0) {
					panelStatus_[i][j].skillPanel_->SpriteDraw();
				}
			}
		}
		skillPlayer_[animeCount_]->Draw();
		skillCursor_->Draw();
	}
	schange->Draw();
	Sprite::PostDraw();
	postEffect_->PostDrawScene(DirectXSetting::GetIns()->GetCmdList());

	DirectXSetting::GetIns()->beginDrawWithDirect2D();
	//テキスト描画範囲

	D2D1_RECT_F textDrawRange = { 300, 520, 700, 750 };
	D2D1_RECT_F skillPointDrawRange = { 550, 100, 900, 200 };
	D2D1_RECT_F skillPanelMessageRange = { 850, 300, 950, 400 };
	//std::wstring hx = std::to_wstring(SceneManager::GetHP());
	//text_->Draw("meiryo", "white", L"" + hx, textDrawRange);
	std::wstring indent = L"\n";
	std::wstring statusMessage = L"HP : ";
	std::wstring skillPointMessage = L"スキルポイント : ";
	skillPointMessage += std::to_wstring(playerUI_->GetSkillPoint());
	statusMessage += std::to_wstring(playerUI_->GetMaxHP());
	statusMessage += indent;
	statusMessage += L"攻撃力 : ";
	statusMessage += std::to_wstring(playerUI_->GetATK());
	statusMessage += indent;
	statusMessage += L"防御力 : ";
	statusMessage += std::to_wstring(playerUI_->GetDef());
	statusMessage += indent;
	statusMessage += L"素早さ : ";
	statusMessage += std::to_wstring(playerUI_->GetSPD());
	if (!skillFlag) {
		playerUI_->TextUIDraw();
	}
	else {
		text_->Draw("bestTen_16", "white", skillPointMessage, skillPointDrawRange);
		text_->Draw("bestTen_16", "white", statusMessage, textDrawRange);
		for (int32_t i = 0; i < 7; i++) {
			for (int32_t j = 0; j < 7; j++) {
				if (panelStatus_[i][j].panelStatus_ != 0) {
					panelStatus_[i][j].skillPanel_->TextMessageDraw();
				}
			}
		}
	}
	DirectXSetting::GetIns()->endDrawWithDirect2D();

	DirectXSetting::GetIns()->PreDraw(backColor);
	//ポストエフェクト描画
	postEffect_->Draw(DirectXSetting::GetIns()->GetCmdList(), 60.0f, postEffectNo_, true);

	//ポストエフェクトをかけないスプライト描画処理(UI等)
	Sprite::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	if (skillFlag != true) {
		playerUI_->SpriteDraw();
	}
	Sprite::PostDraw();
	DirectXSetting::GetIns()->PostDraw();
}


void IBScene::Finalize()
{
	safe_delete(text_);
	skillManager_->Finalize();
	safe_delete(playerUI_);
	//safe_delete(ene);
	//safe_delete(_hummmerObb);
	//colManager_->Finalize();
}

void IBScene::SceneChange()
{
	if (schange->GetFStart() == false) {
		if (skillFlag == false) {
			if (KeyInput::GetIns()->TriggerKey(DIK_UPARROW) || PadInput::GetIns()->TriggerButton(PadInput::Stick_Up)) {
				SoundManager::GetIns()->PlaySE(SoundManager::SEKey::userChoice, 0.1f);
				arrow->SetPosition({ 900,50 });
			}
			else if (KeyInput::GetIns()->TriggerKey(DIK_DOWNARROW) || PadInput::GetIns()->TriggerButton(PadInput::Stick_Down)) {
				SoundManager::GetIns()->PlaySE(SoundManager::SEKey::userChoice, 0.1f);
				arrow->SetPosition({ 900,150 });
			}
			float leftStick = PadInput::GetIns()->leftStickY();
			if (leftStick > 0) {
				soundCount++;
				arrow->SetPosition({ 900,150 });
			}
			else if (leftStick < 0) {
				soundCount++;
				arrow->SetPosition({ 900,50 });
			}
			else {
				soundCount = 0;
			}
		}
	}
	if (soundCount == 1) {
		SoundManager::GetIns()->PlaySE(SoundManager::SEKey::userChoice, 0.1f);
	}
	if (arrow->GetPosition().y == 150) {
		//次のゲームシーンいく
		if (schange->GetEnd() == false) {
			if (schange->GetFStart() == false) {
				if (KeyInput::GetIns()->TriggerKey(DIK_RETURN) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
					SoundManager::GetIns()->PlaySE(SoundManager::SEKey::userDecision, 0.1f);
					schange->SetFStart(true);
					schange->SetFadeNum(0);
				}
			}
		}
		else if (schange->GetEnd() == true) {
			if (baseNo % 2 == 0) {
				//ボスエリアから来た場合
				SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::restPoint);
				SceneManager::SetLevel(playerUI_->GetLevel());
				SceneManager::SetEP(playerUI_->GetEP());
				if (hp_ <= 0) {
					//HP0の時
					SceneManager::SetHP(playerUI_->GetMaxHP());
				}
				else {
					SceneManager::SetHP(playerUI_->GetHP());
				}
				SceneManager::SetMaxHP(playerUI_->GetMaxHP());
				SceneManager::SetATK(playerUI_->GetATK());
				SceneManager::SetDEF(playerUI_->GetDef());
				SceneManager::SetSPD(playerUI_->GetSPD());
				SceneManager::SetSkillPoint(playerUI_->GetSkillPoint());
				if (StageCount::GetIns()->Now() == 5) {
					SceneManager::SceneChange(SceneManager::SceneName::Boss);
				}
				else if (StageCount::GetIns()->Now() == 11 ||
					StageCount::GetIns()->Now() == 17) {
					SceneManager::SceneChange(SceneManager::SceneName::Boss2);
				}
				else {
					SceneManager::SceneChange(SceneManager::SceneName::Game);
				}
			}
			else {
				//通常エリアから来た場合
				SoundManager::GetIns()->StopBGM(SoundManager::BGMKey::restPoint);
				SceneManager::SetLevel(playerUI_->GetLevel());
				SceneManager::SetEP(playerUI_->GetEP());
				if (hp_ <= 0) {
					//HP0の時
					SceneManager::SetHP(playerUI_->GetMaxHP());
				}
				else {
					SceneManager::SetHP(playerUI_->GetHP());
				}
				SceneManager::SetMaxHP(playerUI_->GetMaxHP());
				SceneManager::SetATK(playerUI_->GetATK());
				SceneManager::SetDEF(playerUI_->GetDef());
				SceneManager::SetSPD(playerUI_->GetSPD());
				SceneManager::SetSkillPoint(playerUI_->GetSkillPoint());
				if (StageCount::GetIns()->Now() == 5) {
					SceneManager::SceneChange(SceneManager::SceneName::Boss);
				}
				else if (StageCount::GetIns()->Now() == 10 ||
					StageCount::GetIns()->Now() == 17) {
					SceneManager::SceneChange(SceneManager::SceneName::Boss2);
				}
				else {
					SceneManager::SceneChange(SceneManager::SceneName::Game);
				}
			}
		}
	}
	else if (arrow->GetPosition().y == 50) {
		//スキルエリアに行く
		if (skillFlag == true) {

			if (schange->GetEnd() == false) {
				if (schange->GetFStart() == false) {
					if (KeyInput::GetIns()->TriggerKey(DIK_RETURN) || PadInput::GetIns()->TriggerButton(PadInput::Button_B)) {
						SoundManager::GetIns()->PlaySE(SoundManager::SEKey::userDecision, 0.1f);
						schange->SetFEnd(false);
						schange->SetFStart(true);
						schange->SetFadeNum(0);
					}
				}
			}
			else if (schange->GetEnd() == true) {
				skillCount2 = 0;
				skillFlag = false;
			}
		}
		else if (skillFlag == false) {
			if (schange->GetEnd() == false) {
				if (schange->GetFStart() == false) {
					if (KeyInput::GetIns()->TriggerKey(DIK_RETURN) || PadInput::GetIns()->TriggerButton(PadInput::Button_A)) {
						SoundManager::GetIns()->PlaySE(SoundManager::SEKey::userDecision, 0.1f);
						schange->SetFEnd(false);
						schange->SetFStart(true);
						schange->SetFadeNum(0);
					}
				}
			}
			else if (schange->GetEnd() == true) {
				skillCount2 = 0;
				skillFlag = true;
			}
		}
		if (skillCount2 < 2) {
			skillCount2++;
		}
		if (skillCount2 == 1) {
			schange->SetFStart(false);
			schange->SetFadeNum(1);
			schange->SetFEnd(true);
			schange->SetEnd(false);
		}
	}

}

void IBScene::CameraSetting()
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

void IBScene::Animation()
{
	//タイマーカウント
	if (++animeTimer_ >= animeSpeed_) {
		if (++animeCount_ >= 4) {
			animeCount_ = 0;
		}
		animeTimer_ = 0;
	}

	if (preAnimeCount_ == animeCount_) return;
	player_->SetModel(playerModel_[animeCount_]);
	player_->Initialize();

	preAnimeCount_ = animeCount_;
}

void IBScene::UIUpdate()
{

}

void IBScene::SkillUIUpdate()
{
	//スキルパネル有効化処理
	for (int32_t i = 0; i < 7; i++) {
		for (int32_t j = 0; j < 7; j++) {
			if (panelStatus_[i][j].panelStatus_ == 0) continue;

			bool isActive = panelStatus_[i][j].skillPanel_->GetIsActive();

			if (i == 0 && panelStatus_[i + 1][j].skillPanel_->GetIsSkillGet()) {
				panelStatus_[i][j].skillPanel_->SetIsActive(true);
			}
			if (j == 0 && panelStatus_[i][j + 1].skillPanel_->GetIsSkillGet()) {
				panelStatus_[i][j].skillPanel_->SetIsActive(true);
			}

			if (i == 0 || j == 0) continue;
			if (!isActive && panelStatus_[i - 1][j].skillPanel_->GetIsSkillGet()) {
				panelStatus_[i][j].skillPanel_->SetIsActive(true);
			}
			if (!isActive && panelStatus_[i][j - 1].skillPanel_->GetIsSkillGet()) {
				panelStatus_[i][j].skillPanel_->SetIsActive(true);
			}

			if (i + 1 == 7 || j + 1 == 7) continue;
			if (!isActive && panelStatus_[i + 1][j].skillPanel_->GetIsSkillGet()) {
				panelStatus_[i][j].skillPanel_->SetIsActive(true);
			}
			if (!isActive && panelStatus_[i][j + 1].skillPanel_->GetIsSkillGet()) {
				panelStatus_[i][j].skillPanel_->SetIsActive(true);
			}

		}
	}

	//カーソル移動処理
	const Vector2 sideMoveVec = { 1.f, 0.f };
	const Vector2 verticalMoveVec = { 0.f, 1.f };
	const float moveSpeed = 10.f;

	float leftStick = PadInput::GetIns()->leftStickY();
	Vector2 moveVec = { 0.f, 0.f };
	//縦移動
	if (KeyInput::GetIns()->PushKey(DIK_DOWN) || leftStick > 0) moveVec += verticalMoveVec * moveSpeed;
	else if (KeyInput::GetIns()->PushKey(DIK_UP) || leftStick < 0)  moveVec += verticalMoveVec * -moveSpeed;
	//横移動
	leftStick = PadInput::GetIns()->leftStickX();
	if (KeyInput::GetIns()->PushKey(DIK_RIGHT) || leftStick > 0) moveVec += sideMoveVec * moveSpeed;
	else if (KeyInput::GetIns()->PushKey(DIK_LEFT) || leftStick < 0) moveVec += sideMoveVec * -moveSpeed;
	//現在のカーソル座標に移動ベクトル分加算
	Vector2 cursorPos = skillCursor_->GetPosition() + moveVec;
	//画面外にいかないように現在の座標を代入
	if (cursorPos.x < 0.f || cursorPos.x > 1280.f) cursorPos.x = skillCursor_->GetPosition().x;
	if (cursorPos.y < 0.f || cursorPos.y > 720.f) cursorPos.y = skillCursor_->GetPosition().y;
	//カーソル座標セット
	skillCursor_->SetPosition(cursorPos);

}

void IBScene::SkillPanelInitialize()
{

	for (int32_t i = 0; i < 7; i++) {
		if (i < 3) {
			panelStatus_[3][i].panelStatus_ = 1;
			panelStatus_[i][3].panelStatus_ = 2;
		}
		else {
			panelStatus_[3][i].panelStatus_ = 3;
			panelStatus_[i][3].panelStatus_ = 4;
		}

	}
	panelStatus_[3][3].panelStatus_ = 5;

	Vector2 pos = { 0.f, 0.f };

	for (int32_t i = 0; i < 7; i++) {
		for (int32_t j = 0; j < 7; j++) {
			if (panelStatus_[i][j].panelStatus_ == 1) {
				panelStatus_[i][j].skillPanel_ = std::make_unique<SkillPanel>();
				panelStatus_[i][j].skillPanel_->Initialize(L"HPアップ+5", pos, SkillPanel::HPUP, 5);
			}
			else if (panelStatus_[i][j].panelStatus_ == 2) {
				panelStatus_[i][j].skillPanel_ = std::make_unique<SkillPanel>();
				panelStatus_[i][j].skillPanel_->Initialize(L"ATKアップ+5", pos, SkillPanel::ATKUP, 5);
			}
			else if (panelStatus_[i][j].panelStatus_ == 3) {
				panelStatus_[i][j].skillPanel_ = std::make_unique<SkillPanel>();
				panelStatus_[i][j].skillPanel_->Initialize(L"DEFアップ+5", pos, SkillPanel::DEFUP, 5);
			}
			else if (panelStatus_[i][j].panelStatus_ == 4) {
				panelStatus_[i][j].skillPanel_ = std::make_unique<SkillPanel>();
				panelStatus_[i][j].skillPanel_->Initialize(L"SPDアップ+1", pos, SkillPanel::SPDUP, 1);
			}
			else if (panelStatus_[i][j].panelStatus_ == 5) {
				pos = { 900.f, 300.f };
				panelStatus_[i][j].skillPanel_ = std::make_unique<SkillPanel>();
				panelStatus_[i][j].skillPanel_->Initialize(L"ハンマー回収", pos, SkillPanel::HammerReturn);
				panelStatus_[i][j].skillPanel_->SetIsActive(true);
			}
			else {
				panelStatus_[i][j].skillPanel_ = std::make_unique<SkillPanel>();
				panelStatus_[i][j].skillPanel_->Initialize(L"empty", pos, SkillPanel::HPUP);
			}
		}
	}

	for (int32_t i = 0; i < 7; i++) {
		if (i < 3) {
			pos = { 900.f, 300.f + (50.f * (4.f - ((float)i + 1.f)) + 10.f * (4.f - ((float)i + 1.f))) };
			panelStatus_[i][3].skillPanel_->SetPos(pos);
			pos = { 900.f + (50.f * (4.f - ((float)i + 1.f)) + 10.f * (4.f - ((float)i + 1.f))), 300.f };
			panelStatus_[3][i].skillPanel_->SetPos(pos);
		}
		else if (i > 3) {
			pos = { 900.f, 300.f - (50.f * ((float)i - 3.f) + 10.f * ((float)i - 3.f)) };
			panelStatus_[i][3].skillPanel_->SetPos(pos);
			pos = { 900.f - (50.f * ((float)i - 3.f) + 10.f * ((float)i - 3.f)), 300.f };
			panelStatus_[3][i].skillPanel_->SetPos(pos);
		}
	}
}

void IBScene::AddSkill(int32_t arrayNum_1, int32_t arrayNum_2)
{
	if (panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetSkillType() == SkillPanel::HammerReturn) {
		HammerReturnSkill* hammerReturn = new HammerReturnSkill("hammerReturn");
		skillManager_->AddPlayerPassiveSkill(hammerReturn);
	}
	else if (panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetSkillType() == SkillPanel::HPUP) {
		HPUpSkill* hpUp = new HPUpSkill("HPUp", panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetStatusUpNum());
		skillManager_->AddPlayerPassiveSkill(hpUp);
	}
	else if (panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetSkillType() == SkillPanel::ATKUP) {
		ATKUpSkill* atkUp = new ATKUpSkill("ATKUp", panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetStatusUpNum());
		skillManager_->AddPlayerPassiveSkill(atkUp);
	}
	else if (panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetSkillType() == SkillPanel::DEFUP) {
		DEFUpSkill* defUp = new DEFUpSkill("DEFUp", panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetStatusUpNum());
		skillManager_->AddPlayerPassiveSkill(defUp);
	}
	else if (panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetSkillType() == SkillPanel::SPDUP) {
		SPDUpSkill* spdUp = new SPDUpSkill("SPDUp", panelStatus_[arrayNum_1][arrayNum_2].skillPanel_->GetStatusUpNum());
		skillManager_->AddPlayerPassiveSkill(spdUp);
	}
}

