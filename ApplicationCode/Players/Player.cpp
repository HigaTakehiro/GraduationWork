#include "Player.h"
#include "SafeDelete.h"
#include "Shapes.h"
#include "KeyInput.h"

void Player::Initialize()
{
	for (int32_t i = 0; i < 5; i++) {
		model_[i] = Shapes::CreateSquare({ 0, 0 }, { 64.0f * (float)i, 64.0f }, "Player.png");
	}

	player_ = Object3d::UniquePtrCreate(model_[0]);
	//player_->SetIsBillboardY(true);
	pos_ = { 0, 10, 0 };
	scale_ = { 50, 50, 50 };

	player_->SetPosition(pos_);
	player_->SetScale(scale_);
}

void Player::Update()
{
	const float moveSpeed = 1.0f;

	if (KeyInput::GetIns()->HoldKey(DIK_LEFT)) {
		pos_.x -= moveSpeed;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_RIGHT)) {
		pos_.x += moveSpeed;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_UP)) {
		pos_.z += moveSpeed;
	}
	if (KeyInput::GetIns()->HoldKey(DIK_DOWN)) {
		pos_.z -= moveSpeed;
	}

	player_->SetPosition(pos_);
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
