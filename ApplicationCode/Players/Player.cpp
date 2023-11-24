#include "Player.h"
#include "SafeDelete.h"
#include "Shapes.h"
#include "KeyInput.h"
#include "ExternalFileLoader.h"
#include "PadInput.h"
#include "SoundManager.h"

void Player::Initialize()
{
	//プレイヤー初期化
	for (int32_t i = 0; i < 4; i++) {
		playerModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_idle.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 2.0f }, { 128.0f, 128.0f });
		frontMoveModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_move.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 2.0f }, { 128.0f, 128.0f });
		backMoveModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_moveBack.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 2.0f }, { 128.0f, 128.0f });
		leftMoveModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_Rmove.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 2.0f }, { 128.0f, 128.0f }, true);
		rightMoveModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_Rmove.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 2.0f }, { 128.0f, 128.0f });
		rotAttackModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "tuyu_rot.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 2.f }, { 128, 128 });
	}

	player_ = Object3d::UniquePtrCreate(playerModel_[0]);
	player_->SetIsBillboardY(true);
	player_->SetColType(Object3d::CollisionType::Obb);
	player_->SetObjType((int32_t)Object3d::OBJType::Player);
	player_->SetObbScl({ 2.f,2.f,2.f });
	player_->SetHitRadius(0.5f);
	player_->SetScale({ 0.0f, 0.0f, 0.0f });

	rotAttackPlayer_ = Object3d::UniquePtrCreate(rotAttackModel_[0]);
	rotAttackPlayer_->SetScale({ 0, 0, 0 });
	rotAttackPlayer_->SetIsBillboardY(true);

	shadowModel_ = Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Shadow.png", { 48, 48 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 });
	shadow_ = Object3d::UniquePtrCreate(shadowModel_);
	shadow_->SetParent(player_.get());
	shadow_->SetRotation({ -90, 0, 0 });
	shadow_->SetPosition({ 0, -30, 0 });

	initHammerPos_ = { 0, 0, 30 };
	initHammerScale_ = { 1, 1, 1 };
	initHammerRot_ = { -90, 0, 180 };

	//ハンマー初期化
	hammerModel_ = Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Hammer.png", { 64, 64 }, { 0.5f, 0.5f }, { 0, 0 }, { 128, 128 });
	hammer_ = Object3d::UniquePtrCreate(hammerModel_);
	hammer_->SetParent(player_.get());
	hammer_->SetPosition(initHammerPos_);
	hammer_->SetScale(initHammerScale_);
	hammer_->SetRotation(initHammerRot_);
	hammer_->SetColType(Object3d::CollisionType::Obb);
	hammer_->SetObbScl({ 1.0f,1.f,5.f });
	hammer_->SetObjType((int32_t)Object3d::OBJType::Hammer);
	hammer_->SetHitRadius(1.0f);
	oreCount_ = 0;
	hammerSize_ = initHammerScale_;

	//矢印初期化
	arrowModel_ = Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Arrow.png", { 64, 64 }, { 0.5f, 0.5f });
	arrow_ = Object3d::UniquePtrCreate(arrowModel_);
	arrow_->SetParent(player_.get());
	arrow_->SetPosition({ -60, -30, 200 });
	arrow_->SetRotation(initHammerRot_);

	animeTimer_ = 0;
	preAnimeCount_ = 999;

	//レベル初期化
	if (level_ <= 0) {
		level_ = 1;
		ep_ = 0;
	}

	//UI初期化
	//HPバー
	hpBar_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 20, 50 }, { 0.2f, 0.6f, 0.2f, 1.0f }, { 0.0f, 0.0f });
	hpBarBack_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 20, 50 }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f });
	hpBarSize_ = 200.0f;
	hpBar_->SetSize({ hpBarSize_, 20 });
	hpBar_->SetLeftSizeCorrection(true);
	hpBarBack_->SetSize({ hpBarSize_, 20 });
	//経験値バー
	epBar_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 20, 70 }, { 0.5f, 0.5f, 0.2f, 1.0f }, { 0.0f, 0.0f });
	epBarBack_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 20, 70 }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f });
	epBarSize_ = 200.0f;
	epBar_->SetSize({ epBarSize_, 20 });
	epBar_->SetLeftSizeCorrection(true);
	epBarBack_->SetSize({ epBarSize_, 20 });

	PlayerStatusSetting();

}

void Player::Update()
{

	Repulsion();
	HammerPowerUp();
	LevelUp();

	if (KeyInput::GetIns()->PushKey(DIK_SLASH) && level_ < 99) {
		AddEP(1);
	}
	
	if (isHammerRelease_) {
		HammerThrow();
		HammerGet();
	}
	if (!stop_) {
		UIUpdate();
		Move();
		Animation();
		Attack();
	}

	player_->SetPosition(pos_);
	player_->SetRotation(rot_);
	player_->Update();
	rotAttackPlayer_->SetPosition(pos_);
	rotAttackPlayer_->Update();
	shadow_->Update();
	hammer_->Update();
	arrow_->Update();
}

void Player::Draw()
{
	shadow_->Draw();
	if (isAttack_) {
		hammer_->Draw();
	}
	if (look_ == false) {
		if ((KeyInput::GetIns()->HoldKey(DIK_SPACE) || PadInput::GetIns()->PushButton(PadInput::Button_B)) && !isHammerRelease_) {
			arrow_->Draw();
		}
	}
	if (!isHammerSwing_) {
		player_->Draw();
	}
	else {
		rotAttackPlayer_->Draw();
	}
}

void Player::Finalize()
{
	for (int32_t i = 0; i < 4; i++) {
		safe_delete(playerModel_[i]);
		safe_delete(frontMoveModel_[i]);
		safe_delete(backMoveModel_[i]);
		safe_delete(leftMoveModel_[i]);
		safe_delete(rightMoveModel_[i]);
		safe_delete(rotAttackModel_[i]);
	}
	safe_delete(hammerModel_);
	safe_delete(shadowModel_);
	safe_delete(text_);
}

void Player::HitHammerToEnemy(Vector3 vec, float dis)
{
	repulsionVec_ = vec;
	repulsionSpeed_ = repulsionPower_ / dis;
}

void Player::SpriteDraw()
{
	hpBarBack_->Draw();
	hpBar_->Draw();
	epBarBack_->Draw();
	epBar_->Draw();
}

void Player::PlayerStatusSetting() {
	std::string line;
	Vector3 pos{};
	Vector3 scale{};
	Vector3 rot{};
	float moveSpeed;
	float rotSpeed;
	float throwSpeed;
	float rotResetTime;
	float maxSpeed;
	float acc;
	float ref;
	int32_t hp;
	int32_t atk;
	int32_t maxOreCount;
	int32_t animeSpeed;
	int32_t ep;
	float magEp;
	float hammerRotCoeff;
	Vector3 sizeUp;
	std::stringstream stream;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("PlayerStatus.csv");

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
		if (word.find("scale") == 0) {
			line_stream >> scale.x;
			line_stream >> scale.y;
			line_stream >> scale.z;
		}
		if (word.find("rot") == 0) {
			line_stream >> rot.x;
			line_stream >> rot.y;
			line_stream >> rot.z;
		}
		if (word.find("Smove") == 0) {
			line_stream >> moveSpeed;
		}
		if (word.find("Srot") == 0) {
			line_stream >> rotSpeed;
		}
		if (word.find("RrotTime") == 0) {
			line_stream >> rotResetTime;
		}
		if (word.find("throwSpeed") == 0) {
			line_stream >> throwSpeed;
		}
		if (word.find("hp") == 0) {
			line_stream >> hp;
		}
		if (word.find("maxS") == 0) {
			line_stream >> maxSpeed;
		}
		if (word.find("Sacc") == 0) {
			line_stream >> acc;
		}
		if (word.find("refPower") == 0) {
			line_stream >> ref;
		}
		if (word.find("atk") == 0) {
			line_stream >> atk;
		}
		if (word.find("maxOre") == 0) {
			line_stream >> maxOreCount;
		}
		if (word.find("hammerRotCoeff") == 0) {
			line_stream >> hammerRotCoeff;
		}
		if (word.find("sizeUp") == 0) {
			line_stream >> sizeUp.x;
			line_stream >> sizeUp.y;
			line_stream >> sizeUp.z;
		}
		if (word.find("animeSpeed") == 0) {
			line_stream >> animeSpeed;
		}
		if (word.find("epUp") == 0) {
			line_stream >> ep;
		}
		if (word.find("MagEp") == 0) {
			line_stream >> magEp;
		}
	}

	//初期化
	pos_ = pos;
	initRot_ = rot_ = rot;
	scale_ = scale;
	hp_ = hp;
	maxHp_ = hp;

	moveSpeed_ = moveSpeed;
	rotSpeed_ = initRotSpeed_ = rotSpeed;
	rotResetTimer_ = rotResetTime_ = rotResetTime;
	throwSpeed_ = throwSpeed;
	maxMoveSpeed_ = maxSpeed;
	hammerAcc_ = acc;
	repulsionPower_ = ref;
	attackPoint_ = initAtkPoint_ = atk;
	maxOreCount_ = maxOreCount;
	hammerRotCoeff_ = hammerRotCoeff;
	hammerSizeUp_ = sizeUp;
	if (repulsionPower_ <= 0.0f) {
		repulsionPower_ = 1.0f;
	}
	animeSpeed_ = animeSpeed;
	levelUpEp_ = ep;
	magEp_ = magEp;

	player_->SetPosition(pos_);
	player_->SetScale(scale_);
	player_->SetRotation(rot_);

	rotAttackPlayer_->SetScale(scale_);

}

void Player::Move() {
	if (isStop_)return;

	const Vector3 upDownMoveVec = { 0.0f, 0.0f, 1.0f };
	const Vector3 leftRightMoveVec = { 1.0f, 0.0f, 0.0f };

	float leftStick = PadInput::GetIns()->leftStickX();

	//減速処理
	if (acc_.x < 0) {
		acc_.x += hammerAcc_ / 5;
	}
	else if (acc_.x > 0) {
		acc_.x -= hammerAcc_ / 5;
	}
	if (acc_.z < 0) {
		acc_.z += hammerAcc_ / 5;
	}
	else if (acc_.z > 0) {
		acc_.z -= hammerAcc_ / 5;
	}
	//通常移動
	if ((KeyInput::GetIns()->HoldKey(DIK_SPACE) || PadInput::GetIns()->PushButton(PadInput::Button_B)) && !isHammerRelease_) {
		if (leftStick > 0) {
			acc_ += leftRightMoveVec * -hammerAcc_;
		}
		if (leftStick < 0) {
			acc_ += leftRightMoveVec * hammerAcc_;
		}
		leftStick = PadInput::GetIns()->leftStickY();
		if (leftStick > 0) {
			acc_ += upDownMoveVec * hammerAcc_;
		}
		if (leftStick < 0) {
			acc_ += upDownMoveVec * -hammerAcc_;
		}

		if (KeyInput::GetIns()->HoldKey(DIK_LEFT)) {
			acc_ += leftRightMoveVec * hammerAcc_;
		}
		if (KeyInput::GetIns()->HoldKey(DIK_RIGHT)) {
			acc_ += leftRightMoveVec * -hammerAcc_;
		}
		if (KeyInput::GetIns()->HoldKey(DIK_UP)) {
			acc_ += upDownMoveVec * -hammerAcc_;
		}
		if (KeyInput::GetIns()->HoldKey(DIK_DOWN)) {
			acc_ += upDownMoveVec * hammerAcc_;
		}

		//加速度補正
		if (acc_.x >= maxMoveSpeed_) {
			acc_.x = maxMoveSpeed_;
		}
		else if (acc_.x <= -maxMoveSpeed_) {
			acc_.x = -maxMoveSpeed_;
		}
		if (acc_.z >= maxMoveSpeed_) {
			acc_.z = maxMoveSpeed_;
		}
		else if (acc_.z <= -maxMoveSpeed_) {
			acc_.z = -maxMoveSpeed_;
		}

		//プレイヤーに移動速度を加算
		pos_ += acc_;
	}
	//回転攻撃時移動
	else {

		if (leftStick > 0) {
			pos_ += leftRightMoveVec * -moveSpeed_;
		}
		if (leftStick < 0) {
			pos_ += leftRightMoveVec * moveSpeed_;
		}
		leftStick = PadInput::GetIns()->leftStickY();
		if (leftStick > 0) {
			pos_ += upDownMoveVec * moveSpeed_;
		}
		if (leftStick < 0) {
			pos_ += upDownMoveVec * -moveSpeed_;
		}
		if (KeyInput::GetIns()->HoldKey(DIK_LEFT)) {
			pos_ += leftRightMoveVec * moveSpeed_;
		}
		if (KeyInput::GetIns()->HoldKey(DIK_RIGHT)) {
			pos_ += leftRightMoveVec * -moveSpeed_;
		}
		if (KeyInput::GetIns()->HoldKey(DIK_UP)) {
			pos_ += upDownMoveVec * -moveSpeed_;
		}
		if (KeyInput::GetIns()->HoldKey(DIK_DOWN)) {
			pos_ += upDownMoveVec * moveSpeed_;
		}
	}

	if (nextflor_) {
		if (KeyInput::GetIns()->PushKey(DIK_Z) || PadInput::GetIns()->PushButton(PadInput::Button_A)) {
			next_ = true;
		}
		else {
			next_ = false;
		}
	}


}

void Player::Attack() {
	rotSpeed_ = initRotSpeed_ - (hammerRotCoeff_ * (float)oreCount_);
	//ハンマーをもっているか
	if (!isHammerRelease_) {
		//スペースキーまたはBボタンが押されているか
		if (KeyInput::GetIns()->HoldKey(DIK_SPACE) || PadInput::GetIns()->PushButton(PadInput::Button_B)) {
			notnext_ = true;
			isAttack_ = true;
			isHammerSwing_ = true;
			//回転攻撃
			rotResetTimer_ = 0.0f;
			rot_.y -= rotSpeed_;
		}
		//スペースキーまたはBボタンを離したとき
		else if (KeyInput::GetIns()->ReleaseKey(DIK_SPACE) || PadInput::GetIns()->ReleaseButton(PadInput::Button_B)) {
			isHammerRelease_ = true;
			isHammerSwing_ = false;
			Vector3 hammerPos = pos_;
			hammerPos.y = -30.0f;
			hammerPos_ = hammerPos;
			hammer_->SetParent(nullptr);
			hammer_->SetScale(scale_);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerRelease, 0.2f);
			//進行ベクトルを求める
			Vector3 vec = arrow_->GetMatWorld().r[3] - hammer_->GetMatWorld().r[3];
			vec.normalize();
			//Y軸ベクトルは余計なので0を入れる
			vec.y = 0.0f;
			hammerVec_ = vec;
			hammer_->SetPosition(hammerPos);
		}
	}
	//攻撃キーを離している時プレイヤーの向きを修正
	else {
		rot_ = initRot_;
	}

	if (rot_.y < 0) {
		rot_.y = 360;
	}
	else if (rot_.y > 360) {
		rot_.y = 0;
	}
}

void Player::HammerThrow() {
	const Vector3 hammerSize = { 0.025f, 0.025f, 0.025f };
	const Vector3 hammerScaleCorrection = { 0.007f, 0.007f, 0.007f };
	hammerSize_ = hammerSize + hammerScaleCorrection * (float)oreCount_;
	hammer_->SetScale(hammerSize_);
	if (!isHammerReflect_) {
		//回転角を求める
		Vector3 rot = hammer_->GetRotation();
		rot.y += 5.0f;
		if (rot.y >= 360.0f) {
			rot.y = 0.0f;
		}

		hammerPos_ += hammerVec_ * throwSpeed_;
		hammerPos_.y = -2.0f;
		hammer_->SetPosition(hammerPos_);
		//hammer_->SetRotation(rot);
	}
}

void Player::HammerGet()
{
	if (isHammerReflect_) {
		HammerReturn();
		if (player_->GetIsHit() && hammer_->GetIsHit()) {
			hammer_->SetParent(player_.get());
			hammer_->SetPosition(initHammerPos_);
			hammer_->SetScale(initHammerScale_);
			hammer_->SetRotation(initHammerRot_);
			isHammerRelease_ = false;
			isAttack_ = false;
			isHammerReflect_ = false;
			notnext_ = false;
		}
	}
}

void Player::HammerReturn()
{
	//回転角を求める
	Vector3 rot = hammer_->GetRotation();
	rot.y += 5.0f;
	if (rot.y >= 360.0f) {
		rot.y = 0.0f;
	}

	Vector3 hammerToPlayerVec = pos_ - hammerPos_;
	hammerToPlayerVec.normalize();
	hammerPos_ += hammerToPlayerVec * throwSpeed_;
	hammerPos_.y = -2.0f;
	hammer_->SetPosition(hammerPos_);
	hammer_->SetRotation(rot);
}

void Player::HammerPowerUp()
{
	const Vector3 hammerScale = { 0.2f, 0.2f, 0.2f };
	hammerSize_ = initHammerScale_ + (hammerSizeUp_ * (float)oreCount_);
	//hammerPos_ = initHammerPos_ + initHammerPos_ * 0.5f;
	//hammerPos_.y = -30;
	hammer_->SetScale(hammerSize_);
	attackPoint_ = 1 + oreCount_;
	//hammer_->SetPosition(hammerPos_);
}

void Player::Repulsion()
{
	if (repulsionSpeed_ >= 0.0f) {
		pos_ += repulsionVec_ * repulsionSpeed_;
		repulsionSpeed_ -= 0.1f;
	}
	else {
		repulsionSpeed_ = 0.0f;
	}
}

void Player::Animation()
{
	bool isNotMoveUpDown = false;
	bool isNotMoveLeftRight = false;

	//タイマーカウント
	if (++animeTimer_ >= animeSpeed_) {
		if (++animeCount_ >= 4) {
			animeCount_ = 0;
		}
		animeTimer_ = 0;
	}

	//回転アニメーション
	const float up = 360.0f;
	const float down = 180.0f;
	const float left = 90.0f;
	const float right = 270.0f;
	const float rotAnimeRange = 45.0f;

	if (rot_.y <= left + rotAnimeRange && rot_.y >= left - rotAnimeRange) {
		rotAttackPlayer_->SetModel(rotAttackModel_[3]);
		rotAttackPlayer_->Initialize();
	}
	else if (rot_.y <= down + rotAnimeRange && rot_.y >= down - rotAnimeRange) {
		rotAttackPlayer_->SetModel(rotAttackModel_[0]);
		rotAttackPlayer_->Initialize();
	}
	else if (rot_.y <= right + rotAnimeRange && rot_.y >= right - rotAnimeRange) {
		rotAttackPlayer_->SetModel(rotAttackModel_[1]);
		rotAttackPlayer_->Initialize();
	}
	else if (rot_.y - up <= up + rotAnimeRange && rot_.y + up >= up - rotAnimeRange) {
		rotAttackPlayer_->SetModel(rotAttackModel_[2]);
		rotAttackPlayer_->Initialize();
	}


	if (preAnimeCount_ == animeCount_) return;

	//左スティック
	float leftStick = PadInput::GetIns()->leftStickY();

	if (KeyInput::GetIns()->PushKey(DIK_DOWN) || leftStick > 0) {
		player_->SetModel(frontMoveModel_[animeCount_]);
		player_->Initialize();
	}
	else if (KeyInput::GetIns()->PushKey(DIK_UP) || leftStick < 0) {
		player_->SetModel(backMoveModel_[animeCount_]);
		player_->Initialize();
	}
	else {
		isNotMoveUpDown = true;
	}

	leftStick = PadInput::GetIns()->leftStickX();

	if (KeyInput::GetIns()->PushKey(DIK_RIGHT) || leftStick > 0) {
		player_->SetModel(rightMoveModel_[animeCount_]);
		player_->Initialize();
	}
	else if (KeyInput::GetIns()->PushKey(DIK_LEFT) || leftStick < 0) {
		player_->SetModel(leftMoveModel_[animeCount_]);
		player_->Initialize();
	}
	else {
		isNotMoveLeftRight = true;
	}

	if (isNotMoveUpDown && isNotMoveLeftRight) {
		player_->SetModel(playerModel_[animeCount_]);
		player_->Initialize();
	}

	preAnimeCount_ = animeCount_;

}

void Player::UIUpdate()
{
	const float BarSize = 200;
	hpBarSize_ = ((float)hp_ / (float)maxHp_) * BarSize;
	hpBar_->SetSize({ hpBarSize_, 20 });
	epBarSize_ = ((float)ep_ / (float)levelUpEp_) * BarSize;
	epBar_->SetSize({ epBarSize_, 20 });
}

void Player::LevelUp()
{
	const int32_t maxLevel = 99;
	maxHp_ = 3 + 2 * (level_ - 1);

	if (level_ >= maxLevel) return;

	if (ep_ >= levelUpEp_) {
		SoundManager::GetIns()->PlaySE(SoundManager::SEKey::playerLevelUp, 0.3f);
		level_++;
		ep_ = 0;
		levelUpEp_ = levelUpEp_ + (int32_t)((float)level_ * magEp_);
		maxHp_ = 3 + 2 * (level_ - 1);
		hp_ = maxHp_;
	}
}

void Player::TutorialUpdate(bool Stop, bool NotAttack)
{
	Repulsion();
	HammerPowerUp();
	LevelUp();
	look_ = NotAttack;
	if (isHammerRelease_) {
		HammerThrow();
		HammerGet();
	}
	if (!stop_) {
		UIUpdate();
		if (Stop == false) {
			Move();
			Animation();
		}
		if (NotAttack == false) {
			Attack();
		}
	}

	player_->SetPosition(pos_);
	player_->SetRotation(rot_);
	player_->Update();
	rotAttackPlayer_->SetPosition(pos_);
	rotAttackPlayer_->Update();
	shadow_->Update();
	hammer_->Update();
	arrow_->Update();
}

bool Player::OreCountOverMaxCount()
{
	if (oreCount_ >= maxOreCount_) return true;

	return false;
}

void Player::TextUIDraw()
{
	D2D1_RECT_F HPTextDrawRange = { 30, 48, 158, 176 };
	D2D1_RECT_F EPTextDrawRange = { 30, 68, 158, 176 };
	D2D1_RECT_F LevelTextDrawRange = { 30, 28, 158, 176 };
	std::wstring hp = std::to_wstring(hp_);
	std::wstring maxHP = std::to_wstring(maxHp_);
	std::wstring ep = std::to_wstring(ep_);
	std::wstring maxEP = std::to_wstring(levelUpEp_);
	text_->Draw("meiryo_16", "white", hp + L"/" + maxHP, HPTextDrawRange);
	text_->Draw("meiryo_16", "white", ep + L"/" + maxEP, EPTextDrawRange);
	std::wstring level = std::to_wstring(level_);
	text_->Draw("meiryo_16", "white", L"LV : " + level, LevelTextDrawRange);
}
