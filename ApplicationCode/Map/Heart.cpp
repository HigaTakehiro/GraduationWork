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
}

void Heart::Update(Player* player)
{
	player_ = player;
	if (!display_) { return; }
	HitPlayer();
	heart_->Update();
}

void Heart::Draw()
{
	if (!display_) { return; }
	heart_->Draw();
}

void Heart::HitPlayer()
{
	Vector3 PlayerPos = player_->GetPos();
	Vector3 Pos = heart_->GetPosition();

	if ((PlayerPos.x >= Pos.x - 1.f && PlayerPos.x <= Pos.x + 1.4f) &&
		(PlayerPos.z >= Pos.z + 1.f && PlayerPos.z <= Pos.z + 4.f)) {
		player_->AddHP(1);
	}
}
