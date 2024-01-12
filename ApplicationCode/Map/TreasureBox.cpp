#include "TreasureBox.h"
#include "Shapes.h"
#include"Player.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"
#include"PadInput.h"
#include<SafeDelete.h>

void TreasureBox::Initialize(int num, const XMFLOAT3& MapPos, Player* player, int Count)
{
	player_ = player;
	pos_ = MapPos;
	pos_.y = -2.5f;
	uipos_ = MapPos;
	uipos_.y = -2.5f;
	uipos_.y += 2;
	count_ = Count;

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

	if (num == 1) {
		key_= make_unique<Key>();
		key_->Initialize(pos_);
	}
	else {
		heart_ = make_unique<Heart>();
		heart_->Initialize(pos_);
	}
}

void TreasureBox::Update()
{
	CheckHit();

	treasurebox_->Update();
	ui_->Update();
	if (key_ != nullptr ) {
		key_->Update(player_, lock_, display_);
	}
	if (heart_ != nullptr && display_ == true) {
		heart_->Update(player_, display_);
	}
}

void TreasureBox::Draw()
{
	if (boxdisplay_) {
		treasurebox_->Draw();
		if (f) {
			ui_->Draw();
		}
	}

	if (key_ != nullptr ) { key_->Draw(display_); }
	if (heart_ != nullptr ) { heart_->Draw(display_); }
}

void TreasureBox::CheckHit()
{
	XMFLOAT3 Pos = player_->GetPos();

	if ((Pos.x >= pos_.x - 1.f && Pos.x <= pos_.x + 1.4f) &&
		(Pos.z >= pos_.z + 1.f && Pos.z <= pos_.z + 4.f)) {
		f = true;
		if (PadInput::GetIns()->PushButton(PadInput::Button_A)) {
			boxdisplay_ = false;
			display_ = true;
		}
	}
	else {
		f = false;
	}
}
