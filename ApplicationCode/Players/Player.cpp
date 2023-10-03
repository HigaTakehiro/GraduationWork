#include "Player.h"
#include "SafeDelete.h"
#include "Shapes.h"
#include "KeyInput.h"

void Player::Initialize()
{
	for (int32_t i = 0; i < 5; i++) {
		model_[i] = Shapes::CreateSquare({ 0, 0}, {64.0f * ((float)i + 1), 64.0f}, "Player.png", {320, 64});
	}

	player_ = Object3d::UniquePtrCreate(model_[0]);
	player_->SetIsBillboardY(true);
	pos_ = { 0, 3, 0 };
	scale_ = { 0.03f, 0.03f, 0.03f };

	player_->SetPosition(pos_);
	player_->SetScale(scale_);
	player_->SetRotation({ 0, 0, 0 });
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
		rot--;
	}
	else if (KeyInput::GetIns()->HoldKey(DIK_R)) {
		rot++;
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
		player_->SetModel(model_[animeCount]);
		player_->Initialize();
	}

	player_->SetPosition(pos_);
	player_->SetRotation({0.0f, rot, 0.0f});
	player_->Update();
}

void Player::Draw()
{
	player_->Draw();
}

void Player::Finalize()
{
	for (int32_t i = 0; i < 5; i++) {
		safe_delete(model_[i]);
	}
}
