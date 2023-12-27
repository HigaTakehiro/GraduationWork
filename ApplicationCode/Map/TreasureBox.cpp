#include "TreasureBox.h"
#include "Shapes.h"
#include"Player.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"
#include<SafeDelete.h>

void TreasureBox::Initialize(bool empmty, const XMFLOAT3& MapPos, Player* player, int Count)
{
	player_ = player;
	uipos_ = MapPos;
	uipos_.y += 2;
	count_ = Count;
	pos_ = MapPos;

	stairsModel_ = Shapes::CreateSquare({ 0,0 }, { 64, 64 }, "steps.png", { 2, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 });
	uiModel_ = Shapes::CreateSquare({ 0,0 }, { 192, 64 }, "susumuA.png", { 6, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { -192, 64 });
	treasurebox_ = make_unique<Object3d>();
	treasurebox_ = Object3d::UniquePtrCreate(stairsModel_);
	treasurebox_->SetIsBillboardY(true);
	treasurebox_->SetPosition(pos_);

	ui_ = make_unique<Object3d>();
	ui_ = Object3d::UniquePtrCreate(uiModel_);
	ui_->SetIsBillboardY(true);
	ui_->SetPosition(uipos_);
}

void TreasureBox::Update()
{
	CheckHit();

	treasurebox_->Update();
	ui_->Update();
}

void TreasureBox::Draw()
{
	treasurebox_->Draw();
	if (!f) { return; }
	ui_->Draw();
}

void TreasureBox::CheckHit()
{
	XMFLOAT3 Pos = player_->GetPos();

	if ((Pos.x >= pos_.x - 1.f && Pos.x <= pos_.x + 1.4f) &&
		(Pos.z >= pos_.z + 1.f && Pos.z <= pos_.z + 4.f)) {
		f = true;
		lock_ = true;
	}
	else {
		
		f = false;
	}
}
