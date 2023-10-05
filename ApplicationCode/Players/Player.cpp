#include "Player.h"
#include "SafeDelete.h"
#include "Shapes.h"
#include "KeyInput.h"
#include "ExternalFileLoader.h"

void Player::Initialize()
{
	//プレイヤー初期化
	for (int32_t i = 0; i < 5; i++) {
		playerModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 64.0f * ((float)i + 1), 64.0f }, "Player.png", { 320, 64 });
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

}

void Player::Update()
{
	static int32_t animeCount = 0;

	if (isHammerRelease_) {
		HammerThrow();
		HammerGet();
	}
	Move();
	Attack();


	if (KeyInput::GetIns()->TriggerKey(DIK_N)) {
		if (++animeCount >= 5) {
			animeCount = 0;
		}
		player_->SetModel(playerModel_[animeCount]);
		player_->Initialize();
	}

	player_->SetPosition(pos_);
	player_->SetRotation(rot_);
	player_->Update();
	hammer_->Update();
}

void Player::Draw()
{
	player_->Draw();
	hammer_->Draw();
}

void Player::Finalize()
{
	for (int32_t i = 0; i < 5; i++) {
		safe_delete(playerModel_[i]);
	}
	safe_delete(hammerModel_);
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
	}

	//初期化
	pos_ = pos;
	initRot_ = rot_ = rot;

	scale_ = scale;

	moveSpeed_ = moveSpeed;
	rotSpeed_ = rotSpeed;
	rotResetTimer_ = rotResetTime_ = rotResetTime;
	throwSpeed_ = throwSpeed;
	
	player_->SetPosition(pos_);
	player_->SetScale(scale_);
	player_->SetRotation(rot_);

}

void Player::Move() {
	const Vector3 upDownMoveVec = { 0.0f, 0.0f, 1.0f };
	const Vector3 leftRightMoveVec = { 1.0f, 0.0f, 0.0f };

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

void Player::Attack() {
	//ハンマーをもっているか
	if (!isHammerRelease_) {
		//スペースキーが押されているか
		if (KeyInput::GetIns()->HoldKey(DIK_SPACE)) {
			//回転攻撃
			rotResetTimer_ = 0.0f;
			rot_.y -= rotSpeed_;
		}
		//スペースキーを離したとき
		else if (KeyInput::GetIns()->ReleaseKey(DIK_SPACE)) {
			isHammerRelease_ = true;
			Vector3 hammerPos = pos_;
			hammerPos.y = 30.0f;
			hammerPos_ = hammerPos;
			hammer_->SetParent(nullptr);
			hammer_->SetScale(scale_);

			//進行ベクトルを求める
			Vector3 vec = hammer_->GetMatWorld().r[3];
			vec.normalize();
			//Y軸ベクトルは余計なので0を入れる
			vec.y = 0.0f;
			hammerVec_ = vec;
			hammer_->SetPosition(hammerPos);
		}
	}
	//攻撃キーを離している時プレイヤーの向きを修正
	if (!KeyInput::GetIns()->PushKey(DIK_SPACE)) {
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
		hammer_->SetRotation(rot);
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
			hammerTimer = 0;
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
