#include "Stairs.h"
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
	if (Pos.x >= 10) {
		Pos.x = 10;
	}
	else if (Pos.x <= -8.6) {
		Pos.x = -8.6;
	}

	if (Pos.z >= 8.2) {
		Pos.z = 8.2;
	}
	else if (Pos.z <= -11) {
		Pos.z = -11;
	}

	pos_ = Pos;
}

void Stairs::Initialize(const XMFLOAT3& Pos)
{
	LoadCsv();

	pos_ = Pos+pos_;
	stairs_ = make_unique<Object3d>();
	stairs_= Object3d::UniquePtrCreate(ModelManager::GetIns()->GetModel("Kaidan"));
	stairs_->SetPosition(pos_);
}

void Stairs::Update()
{
	stairs_->Update();
}

void Stairs::Draw()
{
	stairs_->Draw();
}
