#include "TreasureBox.h"
#include "Shapes.h"
#include"Player.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"
#include"PadInput.h"
#include<SafeDelete.h>
#include <random>

void TreasureBox::Initialize(int num, const XMFLOAT3& MapPos, Player* player, int Count)
{
	//óêêîê∂ê¨
	std::random_device rnd2;
	std::mt19937 mt2(rnd2());
	std::uniform_int_distribution<> randX(-8, 8);
	std::uniform_int_distribution<> randZ(-7, 7);
	float posX = MapPos.x + (float)randX(mt2);
	float posZ = MapPos.z + (float)randZ(mt2);

	player_ = player;
	pos_.x = posX;
	pos_.z = posZ;
	pos_.y = -2.5f;
	uipos_ = pos_;
	uipos_.y += 2;
	count_ = Count;

	for (int32_t i = 0; i < 4; i++) {
		stairsModel_[i] = Shapes::CreateSquare({0.f, 0.f}, {64.f, 64.f}, "Tbox.png", {1.8f, 1.8f}, {0.5f, 0.5f}, {64.f * (float)i, 1.f}, {64.f, 64.f});
	}
	
	uiModel_ = Shapes::CreateSquare({ 0,0 }, { 192, 64 }, "susumuA.png", { 6, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { -192, 64 });
	treasurebox_ = make_unique<Object3d>();
	treasurebox_ = Object3d::UniquePtrCreate(stairsModel_[0]);
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
	animeTimer_ = 0;
	animeTime_ = 20;
	animeCount_ = 0;
}

void TreasureBox::Update()
{
	CheckHit();

	if (++animeTimer_ >= animeTime_) {
		if (++animeCount_ >= 4) animeCount_ = 0;
		animeTimer_ = 0;
		treasurebox_->SetModel(stairsModel_[animeCount_]);
		treasurebox_->Initialize();
	}

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
