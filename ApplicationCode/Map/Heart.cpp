#include "Heart.h"
#include "Shapes.h"
#include "SafeDelete.h"
#include"Player.h"

Heart::~Heart()
{
	for (int32_t i = 0; i < 2; i++) {
		safe_delete(model_[i]);
	}
	safe_delete(heart_);
}

void Heart::Initialize(const Vector3& pos)
{
	for (int32_t i = 0; i < 2; i++) {
		model_[i] = Shapes::CreateSquare({ 0.f, 0.f }, { 64.f, 64.f }, "heart.png", { 1.8f, 1.8f }, { 0.5f, 0.5f }, { 64.f * (float)i, 1.f }, { 64.f, 64.f });
	}
	heart_ = Object3d::Create(model_[0]);
	heart_->Initialize();
	heart_->SetPosition(pos);
	pos_ = pos;
	animeTimer_ = 0;
	animeTime_ = 20;
	animeCount_ = 0;
}

void Heart::Update(Player* player, bool& Display)
{
	if (++animeTimer_ >= animeTime_) {
		if (++animeCount_ >= 2) animeCount_ = 0;
		animeTimer_ = 0;
		heart_->SetModel(model_[animeCount_]);
		heart_->Initialize();
	}
	player_ = player;
	if (!Display) { return; }
	//HitPlayer(Display);
	heart_->Update();
	Jump();
	get = true;
}

void Heart::Draw(bool Display)
{
	if (!Display) { return; }
	heart_->Draw();
}

void Heart::Jump()
{
	Vector3 Pos= player_->GetPos();
	pos_ = Pos;
	pos_.y = addpos_ + Pos.y+1;
	addpos_ += 0.07f;
	col_.w -=0.03f;
	heart_->SetColor(col_);
	heart_->SetPosition(pos_);
	if (get == false) { return; }
	player_->AddHP(1);
}


