#include "Stairs.h"
#include "Shapes.h"
#include"Player.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"
#include<SafeDelete.h>


Stairs::~Stairs()
{
	safe_delete(stairsModel_);
	safe_delete(uiModel_);
	ui_.release();
	stairs_.release();
}

void Stairs::LimiAddPos()
{
	//ƒŠƒ~ƒbƒg
	if (addpos_.x >= 10.f) {
		addpos_.x = 10.f;
	}
	else if (addpos_.x <= -8.6f) {
		addpos_.x = -8.6f;
	}
	if (addpos_.z >= 8.2f) {
		addpos_.z = 8.2f;
	}
	else if (addpos_.z <= -11.f) {
		addpos_.z = -11.f;
	}

}

void Stairs::Initialize(const XMFLOAT3& Pos, const Vector3& AddPos,Player* player, int Count)
{
	addpos_ = AddPos;
	LimiAddPos();
	addpos_.y = -2.5f;
	player_ = player;
	pos_ = Pos + addpos_;
	uipos_ = Pos + uipos_+addpos_;
	uipos_.y += 2;
	count_ = Count;

	stairsModel_=Shapes::CreateSquare({0,0}, { 64, 64 }, "steps.png", { 2, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 });
	uiModel_=Shapes::CreateSquare({ 0,0 }, { 192, 64 }, "susumuA.png", { 6, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { -192, 64 });
	stairs_ = make_unique<Object3d>();
	stairs_ = Object3d::UniquePtrCreate(stairsModel_);
	stairs_->SetIsBillboardY(true);
	stairs_->SetPosition(pos_);

	ui_ = make_unique<Object3d>();
	ui_ = Object3d::UniquePtrCreate(uiModel_);
	ui_->SetIsBillboardY(true);
	ui_->SetPosition(uipos_);
}

void Stairs::BossInitialize(const XMFLOAT3& Pos, Player* player)
{
	LimiAddPos();
	addpos_.y = -2.5f;
	player_ = player;
	pos_ = Pos + addpos_;
	uipos_ = Pos + uipos_ + addpos_;
	uipos_.y += 2;

	stairsModel_ = Shapes::CreateSquare({ 0,0 }, { 64, 64 }, "steps.png", { 2, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 });
	uiModel_ = Shapes::CreateSquare({ 0,0 }, { 192, 64 }, "susumuA.png", { 6, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { -192, 64 });
	stairs_ = make_unique<Object3d>();
	stairs_ = Object3d::UniquePtrCreate(stairsModel_);
	stairs_->SetIsBillboardY(true);
	stairs_->SetPosition(pos_);


	ui_ = make_unique<Object3d>();
	ui_ = Object3d::UniquePtrCreate(uiModel_);
	ui_->SetIsBillboardY(true);
	ui_->SetPosition(uipos_);
}

void Stairs::Update()
{
	CheckHit();

	stairs_->Update();
	ui_->Update();
}

void Stairs::Draw()
{
	stairs_->Draw();
	if (!f) { return; }
	ui_->Draw();
}

void Stairs::CheckHit()
{
	XMFLOAT3 Pos = player_->GetPos();

	if ((Pos.x >= pos_.x - 1.f && Pos.x <= pos_.x + 1.4f) &&
		(Pos.z >= pos_.z + 1.f  && Pos.z <= pos_.z + 4.f)) {
		player_->SetNextFlor(true);
		f = true;
	}
	else {
		player_->SetNextFlor(false);
		f = false;
	}
}
