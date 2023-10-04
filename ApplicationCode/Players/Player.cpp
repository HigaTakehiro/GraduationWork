#include "Player.h"
#include "SafeDelete.h"
#include "Shapes.h"
#include "KeyInput.h"

void Player::Initialize()
{
	for (int32_t i = 0; i < 5; i++) {
		playerModel_[i] = Shapes::CreateSquare({ 0, 0}, {64.0f * ((float)i + 1), 64.0f}, "Player.png", {320, 64});
	}

	player_ = Object3d::UniquePtrCreate(playerModel_[0]);
	player_->SetIsBillboardY(true);
	pos_ = { 0, 3, 0 };
	scale_ = { 0.03f, 0.03f, 0.03f };

	player_->SetPosition(pos_);
	player_->SetScale(scale_);
	player_->SetRotation({ 0, 0, 0 });

	hammerModel_ = Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Hammer.png");
	hammer_ = Object3d::UniquePtrCreate(hammerModel_);
	hammer_->SetParent(player_.get());
	hammer_->SetPosition({ -60, -30, 60 });
	//hammer_->SetScale(scale_);
	hammer_->SetRotation({ -90, 0, 180 });
}

void Player::Update()
{
	const float moveSpeed = 0.1f;
	static float rot = 180.0f;
	static int32_t animeCount = 0;

	if (KeyInput::GetIns()->HoldKey(DIK_LEFT)) {
		pos_.x += moveSpeed;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_RIGHT)) {
		pos_.x -= moveSpeed;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_UP)) {
		pos_.z -= moveSpeed;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_DOWN)) {
		pos_.z += moveSpeed;
	}

	if (KeyInput::GetIns()->HoldKey(DIK_R) && KeyInput::GetIns()->HoldKey(DIK_LSHIFT)) {
		rot -= 5;
	}
	else if (KeyInput::GetIns()->HoldKey(DIK_R)) {
		rot += 5;
	}

	if (rot < 0) {
		rot = 360;
	}
	else if (rot > 360) {
		rot = 0;
	}

	if (KeyInput::GetIns()->TriggerKey(DIK_N)) {
		if (++animeCount >= 5) {
			animeCount = 0;
		}
		player_->SetModel(playerModel_[animeCount]);
		player_->Initialize();
	}

	player_->SetPosition(pos_);
	player_->SetRotation({0.0f, rot, 0.0f});
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
