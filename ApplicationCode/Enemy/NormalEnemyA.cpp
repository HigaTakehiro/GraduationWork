#include "NormalEnemyA.h"
#include"ImageManager.h"
#include"ImageManager.h"
void NormalEnemyA::Init()
{
	_status.Tex.reset(Texture::Create(ImageManager::GetIns()->AL, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	_status.Tex->CreateTexture();
}

void NormalEnemyA::Upda(Camera*camera)
{
	(this->*stateTable[_action])();

	_status.Tex->SetScale({ 1,1,1 });

	_status.Tex->SetBillboard(TRUE);
	_status.Tex->Update(camera);
}

void NormalEnemyA::Draw()
{
	Texture::PreDraw();
	_status.Tex->Draw();
	Texture::PostDraw();
}


