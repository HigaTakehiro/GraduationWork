#include "NormalEnemyA.h"

#include <algorithm>
#include <any>
#include <dinput.h>

#include "Collision.h"
#include "CsvLoader.h"
#include "Easing.h"
#include "ExternalFileLoader.h"
#include"ImageManager.h"
#include"ImageManager.h"
#include "KeyInput.h"
#include"Helper.h"

#define PI 3.14
void NormalEnemyA::Init()
{
	type_ = 0;
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
	m_MaxHp = _status.HP;;
	TexInit();
	AnimationInterval = 20;
	std::stringstream stream;
	std::string line;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("NormalEnemyA.csv");
	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("Param") == 0) {
			getline(line_stream, word, ' ');
			guardp = (int)std::atof(word.c_str());
			continue;
		}
	}

	_color = { 1,1,1,1 };
}

void NormalEnemyA::Upda(Camera* camera)
{
	(this->*stateTable[_action])();

	if (_status.Rot.y >= 360.f || _status.Rot.y <= -360.f)_status.Rot.y = 0.f;

	TextureAnimation();

	CollideHummmer();

	RecvFlashColor();

	Jump();


	if (_status.Scl.y <= 1.3f && !damfprot)	Helper::ColKnock(_player->GetPos(), { _status.Pos.x, _status.Pos.y, _status.Pos.z + 1.f }, _player.get(), Collision::HitCircle({ _status.Pos.x,_status.Pos.z + 1.f }, 0.5f, { _player->GetPos().x,_player->GetPos().z }, 1.f), 1.9f);

	DamageFlash();
	if (_status.Tex != nullptr) {

		_status.Tex->SetPosition(_status.Pos);
		_status.Tex->SetScale(_status.Scl);
		_status.Tex->SetRotation({ _status.Rot.x,0,_status.Rot.z});
		_status.Tex->SetBillboard(FALSE);
		_status.Tex->SetColor(_color);
		_status.Tex->Update(camera);
	}

	ClampMap();

	if (!_isAttack) {back_t = 0.f; }

	TexUpda();
}
#include"Helper.h"
void NormalEnemyA::Draw()
{
	if (_status.HP <= 0)return;
	if (_status.Tex == nullptr)return;
	Texture::PreDraw();
	_status.Tex->Draw();
	Texture::PostDraw();

	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
		TexDraw();
	Object3d::PostDraw();
}

void NormalEnemyA::TextureAnimation()
{
	AnimationCount++;
	float NowRota = abs(0 - _status.Rot.y);
	if ( NowRota < 45) {
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
	} else if (NowRota < 135) {
		if (AnimationCount == AnimationInterval) {

			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->RUSA1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
		} else if (AnimationCount == 2 * AnimationInterval) {
			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->RUSA2, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
		} else if (AnimationCount == 3 * AnimationInterval) {
			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->RUSA3, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
			AnimationCount = 0;
		}
	}

	else if (NowRota < 225) {
		if (AnimationCount == AnimationInterval) {

			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->BUSA1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
		} else if (AnimationCount == 2 * AnimationInterval) {
			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->BUSA2, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
		} else if (AnimationCount == 3 * AnimationInterval) {
			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->BUSA3, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
			AnimationCount = 0;
		}
	}

	else if (NowRota < 315) {

		if (AnimationCount == AnimationInterval) {

			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->LUSA1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
		} else if (AnimationCount == 2 * AnimationInterval) {
			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->LUSA2, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
		} else if (AnimationCount == 3 * AnimationInterval) {
			_status.Tex.reset(Texture::Create(ImageManager::GetIns()->LUSA3, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
			_status.Tex->CreateTexture();
			_status.Tex->SetAnchorPoint({ 0.5f,1.f });
			AnimationCount = 0;
		}
	} else {
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
	//AttackAction();

}

void NormalEnemyA::TexDraw()
{
	constexpr float dis_max = 150.f;

	Helper::isDraw(_player->GetPos(), _status.Pos, m_ShadowTex.get(), dis_max, _status.HP <= 0);

	Helper::isDraw(_player->GetPos(),_status.Pos, m_HpTex.get(), dis_max,_status.HP <= 0);
}


void NormalEnemyA::Jump()
{
	float SubPower = 0.001f;
	//落下の緩急
	constexpr float Distortion = 3.f;
	//地面の高さ
	constexpr float GroundY = -2.5f;
	//ジャンプ高さ
	constexpr float Height = 0.6f;

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

	bool isRecv = !PlayerRecv && Collision::GetLength(_status.Pos, _player->GetPos()) < 2.f;

	
	if(isRecv)
	{
		//if(_status.Scl.y > 1.f)
		
		if(m_knockF)
		m_knockF = TRUE;
		
		PlayerRecv = TRUE;
	}

	constexpr uint16_t maxcooltime = 90;
	if(m_knockF)
	{
		if(++m_KnockCoolT>maxcooltime)
		{
			m_knockF = FALSE;
		}
	}
	else
	{
		m_KnockCoolT = 0;
	}

	if(PlayerRecv&& _action != ATTACK)
	{
		
	}
PlayerRecv = FALSE;
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
	bool aj = _status.Scl.y > 1.3f && Collision::HitCircle({ _status.Pos.x,_status.Pos.z + 1.f }, 1.5f, { _player->GetPos().x,_player->GetPos().z }, 2.5f);
	if (aj)
	{
		_player->SubHP(1);
	}

	Helper::ColKnock(_player->GetPos(), { _status.Pos.x, _status.Pos.y, _status.Pos.z + 1.f }, _player.get(), aj, 1.f);

	_status.Scl.x = std::clamp(_status.Scl.x, 1.f, 1.3f);
	_status.Scl.y=std::clamp(_status.Scl.y, 1.3f, 1.8f);
}

void NormalEnemyA::TutorialUpda(Camera* camera, bool flag)
{
	(this->*stateTable[_action])();

	if (_status.Rot.y >= 360.f || _status.Rot.y <= -360.f)_status.Rot.y = 0.f;
	//_status.Tex->
	///_status.HP--;
	//AttackAction();

	if (_status.Scl.y <= 1.3f && !damfprot)	Helper::ColKnock(_player->GetPos(), { _status.Pos.x, _status.Pos.y, _status.Pos.z + 1.f }, _player.get(), Collision::HitCircle({ _status.Pos.x,_status.Pos.z + 1.f }, 0.5f, { _player->GetPos().x,_player->GetPos().z }, 1.f), 1.9f);


	TextureAnimation();

	CollideHummmer();

	RecvFlashColor();

	if (flag == false) {
		Jump();
	}

	DamageFlash();
	if (_status.Tex != nullptr) {

		_status.Tex->SetPosition(_status.Pos);
		_status.Tex->SetScale(_status.Scl);
		_status.Tex->SetRotation({ _status.Rot.x,0,_status.Rot.z });
		_status.Tex->SetBillboard(FALSE);
		_status.Tex->SetColor(_color);
		_status.Tex->Update(camera);
	}

	ClampMap();

	if (!_isAttack) { back_t = 0.f; }

	TexUpda();
}

void NormalEnemyA::TutorialDraw(float Mindis)
{
	
	if (Collision::GetLength(_status.Pos, _player->GetPos()) > Mindis)return;
	if (_status.HP <= 0)return;
	if (_status.Tex == nullptr)return;
	Texture::PreDraw();
	_status.Tex->Draw();
	Texture::PostDraw();

	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	TexDraw();
	Object3d::PostDraw();
}

void NormalEnemyA::ClampMap()
{
	_status.Pos.x = std::clamp(_status.Pos.x, OverPosMin.x, OverPosMax.x);
	_status.Pos.z = std::clamp(_status.Pos.z, OverPosMin.z, OverPosMax.z);
}

void NormalEnemyA::SetPosDeb(Vector3 pos)
{
	
}
