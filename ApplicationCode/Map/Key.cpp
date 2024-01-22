#include "Key.h"
#include "Shapes.h"
#include "SafeDelete.h"
#include"Player.h"

Key::~Key()
{
	for (int i = 0; i < 2; i++) {
		safe_delete(model[2]);
	}
	safe_delete(key_);
}

void Key::Initialize(const Vector3& pos)
{
	for (int i = 0; i < 2; i++) {
		model[i] = Shapes::CreateSquare({ 0.f, 0.f }, { 64.f, 64.f }, "key.png", { 1.8f, 1.8f }, { 0.5f, 0.5f }, { 64.f * (float)i, 1.f }, { 64.f, 64.f });
	}
	key_ = Object3d::Create(model[0]);
	key_->Initialize();
	pos_ = pos;
	key_->SetPosition(pos_);
	key_->SetColor(col_);
	animeTimer_ = 0;
	animeTime_ = 20;
	animeCount_ = 0;
}

void Key::Update(Player* player, bool& lock, bool Display)
{
	if (++animeTimer_ >= animeTime_) {
		if (++animeCount_ >= 2) animeCount_ = 0;
		animeTimer_ = 0;
		key_->SetModel(model[animeCount_]);
		key_->Initialize();
	}
	player_ = player;
	if (!Display) { return; }
	key_->Update();
	Jump();
}

void Key::Draw(bool Display)
{
	if (!Display) { return; }
	key_->Draw();
}

void Key::Jump()
{
	Vector3 Pos= player_->GetPos();
	pos_ = Pos;
	pos_.y = addpos_ + Pos.y+1;
	addpos_ += 0.07f;
	col_.w -=0.03f;
	key_->SetColor(col_);
	key_->SetPosition(pos_);
}