#include "Key.h"
#include "Shapes.h"
#include "SafeDelete.h"
#include"Player.h"

Key::~Key()
{
	safe_delete(model);
	safe_delete(key_);
}

void Key::Initialize(const Vector3& pos)
{
	model = Shapes::CreateSquare({ 0,0 }, { 64, 64 }, "steps.png", { 2, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 });
	key_ = Object3d::Create(model);
	key_->Initialize();
	key_->SetPosition(pos);
}

void Key::Update(Player* player, bool& lock, bool Display)
{
	player_ = player;
	if (!Display) { return; }
	HitPlayer(lock);
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
