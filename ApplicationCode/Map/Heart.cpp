#include "Heart.h"
#include "Shapes.h"
#include "SafeDelete.h"
#include"Player.h"

Heart::~Heart()
{
	safe_delete(model);
	safe_delete(heart_);
}

void Heart::Initialize(const Vector3& pos)
{
	model = Shapes::CreateSquare({ 0,0 }, { 64, 64 }, "steps.png", { 2, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 });
	heart_ = Object3d::Create(model);
	heart_->Initialize();
	heart_->SetPosition(pos);
	pos_ = pos;
}

void Heart::Update(Player* player, bool& Display)
{
	player_ = player;
	if (!Display) { return; }
	HitPlayer(Display);
	heart_->Update();
}

void Heart::Draw(bool Display)
{
	if (!Display) { return; }
	heart_->Draw();
}

void Heart::Spown()
{
	if (!spown_) { return; }
	animeTimer_ += 0.1f;
	if (animeTimer_ >= 1) {
		spown_ = true;
		animeTimer_ = 0;
	}
}

void Heart::HitPlayer(bool& Display)
{
	if (!spown_) { return; }
	Vector3 PlayerPos = player_->GetPos();
	Vector3 Pos = heart_->GetPosition();

	if ((PlayerPos.x >= Pos.x - 1.f && PlayerPos.x <= Pos.x + 1.4f) &&
		(PlayerPos.z >= Pos.z + 1.f && PlayerPos.z <= Pos.z + 4.f)) {
		if(player_->GetMaxHP()>player_->GetHP())
		player_->AddHP(1);
		Display = false;
	}
}
