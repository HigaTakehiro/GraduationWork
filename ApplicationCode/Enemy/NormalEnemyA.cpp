#include "NormalEnemyA.h"

#include <algorithm>
#include <any>
#include <dinput.h>

#include "Collision.h"
#include "CsvLoader.h"
#include "Easing.h"
#include"ImageManager.h"
#include"ImageManager.h"
#include "KeyInput.h"


#define PI 3.14
void NormalEnemyA::Init()
{
	_status.TexSize = 3;
	_color = XMFLOAT4(1, 1, 1, 1);
	_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	_status.Tex->CreateTexture();
	
	_status.Rot = { 180.f,0.f,0.f };
	//CSV読み込み
	_status.HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Engine/Resources/GameData/NormalEnemyA.csv", "HP")));
	_status.SearchRange = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Engine/Resources/GameData/NormalEnemyA.csv", "SearchRange")));
	_status.MoveSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Engine/Resources/GameData/NormalEnemyA.csv", "MoveSpeed")));
	_status.Scl = { 1.3f,1.3f,1.3f };
	//XMFLOAT3だけは今の時点だとこう
	LoadCSV::LoadCsvParam_XMFLOAT3("Engine/Resources/GameData/NormalEnemyA.csv", _status.Pos, "Position");

	//this->SetColType(Object3d::CollisionType::Obb);
	//this->SetObjType(Object3d::CollisionType::Enemy);
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

	//_status.Tex->

	TextureAnimation();

	CollideHummmer();

	RecvFlashColor();

	Jump();

	if (_status.Tex != nullptr) {

		_status.Tex->SetPosition(_status.Pos);
		_status.Tex->SetScale(_status.Scl);
		_status.Tex->SetRotation({ _status.Rot.x,_status.Rot.y,_status.Rot.z});
		_status.Tex->Update(camera);
	}
	

	if (!_isAttack) {back_t = 0.f; }
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
		_status.Tex->SetAnchorPoint({ 0.5f,1.f });
	} else if (AnimationCount == 2 * AnimationInterval) {
		_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_2, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		_status.Tex->CreateTexture();
		_status.Tex->SetAnchorPoint({ 0.5f,1.f });
	} else if (AnimationCount == 3 * AnimationInterval) {
		_status.Tex.reset(Texture::Create(ImageManager::GetIns()->USA_3, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		_status.Tex->CreateTexture();
		_status.Tex->SetAnchorPoint({ 0.5f,1.f });
		AnimationCount = 0;
	}
}


void NormalEnemyA::Jump()
{
	float SubPower = 0.001f;
	//落下の緩急
	constexpr float Distortion = 3.f;
	//地面の高さ
	constexpr float GroundY = 0.5f;
	//ジャンプ高さ
	constexpr float Height = 1.5f;

	//ドッスン挙動

	JFrame += 1.f / 60.f;
	_status.Pos.y = GroundY + (1.0f - pow(1.0f - sin(PI * JFrame), Distortion)) * Height;

	if (JFrame >= 1.f&&_action!=ATTACK)JFrame = 0.f;
	JFrame=std::clamp(JFrame, 0.f, 1.f);
}


void NormalEnemyA::AttackAction()
{
	constexpr float BackRVal = 30.f;
	constexpr float SecMotInter = 20.f;
	constexpr float ThiMotInter = 20.f;
	constexpr float XScling = 0.05f, YScling = 0.13f;

	if (back_t <= BackRVal) {
		_status.Rot.x = Easing::easeIn(back_t, BackRVal, 180.f, 200.f);
	} else if (back_t <= BackRVal + SecMotInter) {
		_status.Scl.x += XScling;
		_status.Scl.y += YScling;
		_status.Rot.x = Easing::easeIn(back_t, BackRVal + SecMotInter, 210.f, 90.f);
	} else if (back_t <= BackRVal + SecMotInter + ThiMotInter * 2) {
		_status.Scl.x -= XScling;
		_status.Scl.y -= YScling;
		_status.Rot.x = Easing::easeIn(back_t, BackRVal +SecMotInter+ ThiMotInter * 2, 90.f, 180.f);
	}

	if (back_t >= BackRVal + SecMotInter+ThiMotInter * 2)
	{
		_isAttack = FALSE;
		_action = FOLLOW;
	} else {
		if(JFrame>=1.f)
		back_t++;
	}
	_status.Scl.x = std::clamp(_status.Scl.x, 1.f, 1.3f);
	_status.Scl.y=std::clamp(_status.Scl.y, 1.3f, 1.8f);
}
