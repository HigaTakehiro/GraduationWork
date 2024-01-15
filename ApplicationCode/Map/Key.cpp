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
	key_->SetPosition(pos);
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
	//HitPlayer(lock);
	key_->Update();
}

void Key::Draw(bool Display)
{
	if (!Display) { return; }
	key_->Draw();
}

void Key::HitPlayer(bool& lock)
{
	Vector3 PlayerPos = player_->GetPos();
	Vector3 Pos = key_->GetPosition();

	if ((PlayerPos.x >= Pos.x - 1.f && PlayerPos.x <= Pos.x + 1.4f) &&
		(PlayerPos.z >= Pos.z + 1.f && PlayerPos.z <= Pos.z + 4.f)) {
		lock = true;
		display_ = false;
	}
}
