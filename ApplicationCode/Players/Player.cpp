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
		playerModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 72.0f * ((float)i + 1), 64.0f }, "tuyu_idle.png", { 320, 64 });
	}

	player_ = Object3d::UniquePtrCreate(playerModel_[0]);
	player_->SetIsBillboardY(true);
	player_->SetColType(Object3d::CollisionType::Sphere);
	player_->SetObjType((int32_t)Object3d::OBJType::Player);
	player_->SetHitRadius(0.5f);

	PlayerStatusSetting();

	initHammerPos_ = { -60, -30, 60 };
	initHammerScale_ = { 1, 1, 1 };
	initHammerRot_ = { -90, 0, 180 };

	//ハンマー初期化
	hammerModel_ = Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Hammer.png");
	hammer_ = Object3d::UniquePtrCreate(hammerModel_);
	hammer_->SetParent(player_.get());
	hammer_->SetPosition(initHammerPos_);
	hammer_->SetScale(initHammerScale_);
	hammer_->SetRotation(initHammerRot_);
	hammer_->SetColType(Object3d::CollisionType::Sphere);
	hammer_->SetObjType((int32_t)Object3d::OBJType::Hammer);
	hammer_->SetHitRadius(1.0f);
	oreCount_ = 0;
	hammerSize_ = initHammerScale_;

	//矢印初期化
	arrowModel_ = Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Arrow.png");
	arrow_ = Object3d::UniquePtrCreate(arrowModel_);
	arrow_->SetParent(player_.get());
	arrow_->SetPosition({ -60, -30, 200 });
	arrow_->SetRotation(initHammerRot_);
}

void Player::Update()
{
	static int32_t animeCount = 0;

	Repulsion();
	HammerPowerUp();

	if (isHammerRelease_) {
		HammerThrow();
		HammerGet();
	}
	if (!stop_) {
		Move();
		Attack();
	}

	if (KeyInput::GetIns()->TriggerKey(DIK_N)) {
		if (++animeCount >= 4) {
			animeCount = 0;
		}
		player_->SetModel(playerModel_[animeCount]);
		player_->Initialize();
	}

	player_->SetPosition(pos_);
	player_->SetRotation(rot_);
	player_->Update();
	hammer_->Update();
	arrow_->Update();
}

void Player::Draw()
{
	player_->Draw();
	hammer_->Draw();
	if ((KeyInput::GetIns()->HoldKey(DIK_SPACE) || PadInput::GetIns()->PushButton(PadInput::Button_B)) && !isHammerRelease_) {
		arrow_->Draw();
	}
}

void Player::Finalize()
{
	for (int32_t i = 0; i < 4; i++) {
		safe_delete(playerModel_[i]);
	}
	safe_delete(hammerModel_);
}

void Player::HitHammerToEnemy(Vector3 vec)
{
	repulsionVec_ = vec;
	repulsionSpeed_ = repulsionPower_;
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
	}

	//初期化
	pos_ = pos;
	initRot_ = rot_ = rot;
	scale_ = scale;
	hp_ = hp;

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

	player_->SetPosition(pos_);
	player_->SetScale(scale_);
	player_->SetRotation(rot_);

}

void Player::Move() {
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
			hammerPos.y = 30.0f;
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
		//時間の割合を求める
		float t = 0.0f;
		if (++rotResetTimer_ < rotResetTime_) {
			t = rotResetTimer_ / rotResetTime_;
			//イージングをかける
			rot_ = easeIn(rot_, initRot_, t);
		}
		else {
			rot_ = initRot_;
		}

	}

	if (rot_.y < 0) {
		rot_.y = 360;
	}
	else if (rot_.y > 360) {
		rot_.y = 0;
	}
}

void Player::HammerThrow() {
	const Vector3 hammerSize = { 0.03f, 0.03f, 0.03f };
	const Vector3 hammerScaleCorrection = { 0.007f, 0.007f, 0.007f };
	hammerSize_ = hammerSize + hammerScaleCorrection * (float)oreCount_;
	hammer_->SetScale(hammerSize_);
	if (++hammerTimer <= hammerTime) {
		//回転角を求める
		Vector3 rot = hammer_->GetRotation();
		rot.y += 5.0f;
		if (rot.y >= 360.0f) {
			rot.y = 0.0f;
		}

		hammerPos_ += hammerVec_ * throwSpeed_;
		hammerPos_.y = 2.0f;
		hammer_->SetPosition(hammerPos_);
		//hammer_->SetRotation(rot);
	}
	else {
		hammerTimer = hammerTime;
	}
}

void Player::HammerGet()
{
	if (hammerTimer >= hammerTime) {
		HammerReturn();
		if (player_->GetIsHit() && hammer_->GetIsHit()) {
			hammer_->SetParent(player_.get());
			hammer_->SetPosition(initHammerPos_);
			hammer_->SetScale(initHammerScale_);
			hammer_->SetRotation(initHammerRot_);
			isHammerRelease_ = false;
			isAttack_ = false;
			hammerTimer = 0;
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
	hammerPos_.y = 2.0f;
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
