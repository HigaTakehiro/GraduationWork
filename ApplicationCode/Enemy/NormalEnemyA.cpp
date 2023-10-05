#include "NormalEnemyA.h"

#include "Collision.h"
#include"ImageManager.h"
#include"ImageManager.h"
void NormalEnemyA::Init()
{
	_status.TexSize = 3;

	_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	_status.Tex->CreateTexture();
	_status.Tex->SetAnchorPoint({ 0.5f,0.5f });
	_status.SearchRange = 5.f;
	_status.MoveSpeed = 0.3f;
	_status.Pos = { 10,1.5f,0 };


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
	if (Collision::OBBCollision(_status.Obb, _playerOBB)) {
		RecvDamage = TRUE;
	}
}

void NormalEnemyA::Draw()
{
	Texture::PreDraw();
	if (!Collision::OBBCollision(_status.Obb, _playerOBB)) {
		if (_status.Tex != nullptr) {
			_status.Tex->Draw();
		}
	}
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



