#include "NormalEnemyA.h"

#include <any>

#include "Collision.h"
#include "CsvLoader.h"
#include"ImageManager.h"
#include"ImageManager.h"
void NormalEnemyA::Init()
{
	_status.TexSize = 3;
	
	_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	_status.Tex->CreateTexture();
	_status.Tex->SetAnchorPoint({ 0.5f,0.5f });

	//CSVì«Ç›çûÇ›
	_status.HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Engine/Resources/GameData/NormalEnemyA.csv", "HP")));
	_status.SearchRange = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Engine/Resources/GameData/NormalEnemyA.csv", "SearchRange")));
	_status.MoveSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Engine/Resources/GameData/NormalEnemyA.csv", "MoveSpeed")));

	//XMFLOAT3ÇæÇØÇÕç°ÇÃéûì_ÇæÇ∆Ç±Ç§
	LoadCSV::LoadCsvParam_XMFLOAT3("Engine/Resources/GameData/NormalEnemyA.csv", _status.Pos, "Position");

	/*
	 * 	for (auto i = 0; i < _status.TexSize; i++)
		_status.TexModel[i] = Shapes::CreateSquare({ 0, 0 }, { 64.0f * ((float)i + 1), 64.0f }, "usa_idle.png", { 320, 64 });

	_status.Tex = Object3d::UniquePtrCreate(_status.TexModel[0]);
	_status.Tex->SetModel(_status.TexModel[AnimationCount]);
	_status.Tex->Initialize();
	_status.Tex->SetIsBillboardY(true);

	 */
	AnimationInterval = 20;
}

void NormalEnemyA::Upda(Camera* camera)
{
	(this->*stateTable[_action])();

	TextureAnimation();

	CollideHummmer();
	if (_status.Tex != nullptr) {
		_status.Scl = { 1,1,1 };
		_status.Tex->SetPosition(_status.Pos);
		_status.Tex->SetScale(_status.Scl);
		_status.Tex->SetRotation({ 180,0,0 });
		
		_status.Tex->Update(camera);_status.Tex->SetBillboard(TRUE);
	}
	if (!RecvDamage&&Collision::OBBCollision(_status.Obb, _playerOBB)) {
		_status.HP--;
		RecvDamage = TRUE;
	}
}

void NormalEnemyA::Draw()
{
	if (_status.HP <= 0)return;
	if (_status.Tex == nullptr)return;
	Texture::PreDraw();
	_status.Tex->Draw();
	Texture::PostDraw();
}

void NormalEnemyA::TextureAnimation()
{
	AnimationCount++;

	if (AnimationCount == AnimationInterval) {
		_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		_status.Tex->CreateTexture();
	} else if (AnimationCount == 2 * AnimationInterval) {
		_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_2, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		_status.Tex->CreateTexture();
	} else if (AnimationCount == 3 * AnimationInterval) {
		_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_3, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		_status.Tex->CreateTexture();
		AnimationCount = 0;
	}
}



