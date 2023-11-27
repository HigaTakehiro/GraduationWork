#include "Stairs.h"
#include "Shapes.h"
#include"Player.h"
#include"Modelmanager.h"
#include "ExternalFileLoader.h"

void Stairs::LoadCsv()
{
	std::string line;
	XMFLOAT3 Pos = { 30.f ,0.f,30.f };

	std::stringstream stream;


	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Kaidan.csv");

	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');


		if (word.find("POS") == 0) {
			line_stream >> Pos.x;
			line_stream >> Pos.y;
			line_stream >> Pos.z;
		}
	}
	//ƒŠƒ~ƒbƒg
	if (Pos.x >= 10.f) {
		Pos.x = 10.f;
	}
	else if (Pos.x <= -8.6f) {
		Pos.x = -8.6f;
	}

	if (Pos.z >= 8.2f) {
		Pos.z = 8.2f;
	}
	else if (Pos.z <= -11.f) {
		Pos.z = -11.f;
	}

	pos_ = Pos;
	uipos_ = Pos;
}

void Stairs::Initialize(const XMFLOAT3& Pos, Player* player, int Count)
{
	LoadCsv();
	player_ = player;
	pos_ = Pos + pos_;
	uipos_ = Pos + uipos_;
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
	LoadCsv();
	player_ = player;
	pos_ = Pos + pos_;
	uipos_ = Pos + uipos_;
	uipos_.y += 2;

	stairsModel_ = Shapes::CreateSquare({ 0,0 }, { 64, 64 }, "steps.png", { 2, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 });
	uiModel_ = Shapes::CreateSquare({ 0,0 }, { 192, 64 }, "susumuA.png", { 2.5, 2 }, { 0.5f, 0.5f }, { 0, 0 }, { -192, 64 });
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
	XMFLOAT3 Pos = player_->Get();

	if ((Pos.x >= pos_.x - 2.f && Pos.x <= pos_.x + 1.f) &&
		(Pos.z >= pos_.z-2.f  && Pos.z <= pos_.z + 1.f)) {
		player_->SetNextFlor(true);
		f = true;
	}
	else {
		player_->SetNextFlor(false);
		f = false;
	}
}
