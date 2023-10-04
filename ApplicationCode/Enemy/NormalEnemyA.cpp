#include "NormalEnemyA.h"
#include"ImageManager.h"
#include"ImageManager.h"
void NormalEnemyA::Init()
{
	_status.TexSize = 3;

	_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	_status.Tex->CreateTexture();
	_status.Tex->SetAnchorPoint({ 0.5f,0.5f });
	_status.SearchRange = 15.f;
	_status.MoveSpeed = 0.1f;
	_status.Pos = { 0,1.5f,0 };
	AnimationInterval = 20;
}

void NormalEnemyA::Upda(Camera* camera)
{
	(this->*stateTable[_action])();

	TextureAnimation();

	if (_status.Tex != nullptr) {
		_status.Scl = { 1,1,1 };
		_status.Tex->SetPosition(_status.Pos);
		_status.Tex->SetScale(_status.Scl);
		_status.Tex->SetRotation({ 180,0,0 });
		_status.Tex->SetBillboard(TRUE);
		_status.Tex->Update(camera);
	}
}

void NormalEnemyA::Draw()
{
	Texture::PreDraw();
	if (_status.Tex != nullptr) {
		_status.Tex->Draw();
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



