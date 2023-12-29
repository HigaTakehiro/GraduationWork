#include "BaseEnemy.h"

#include "Collision.h"
#include "KeyInput.h"
#include "MouseInput.h"
#include"Easing.h"
#include "Helper.h"
#include "ImageManager.h"
#include "Shapes.h"
#include "SoundManager.h"


/***                    GETTER                      ***/
/******************************************************/
unsigned int BaseEnemy::GetHP() const { return _status.HP; }
unsigned int BaseEnemy::GetAttackVal() const { return _status.DamageValue; }

XMFLOAT3 BaseEnemy::GetPos() const { return _status.Pos; }
XMFLOAT3 BaseEnemy::GetRot() const { return _status.Rot; }
XMFLOAT3 BaseEnemy::GetScl() const { return  _status.Scl; }

XMFLOAT3 BaseEnemy::GetPos2() const { return state_obj_.Pos_; }
XMFLOAT3 BaseEnemy::GetRot2() const { return state_obj_.Rot_; }
XMFLOAT3 BaseEnemy::GetScl2() const { return  state_obj_.Scl; }
/*******************************************************/


/********************************************************/

void (BaseEnemy::* BaseEnemy::stateTable[])() = {
	&BaseEnemy::Idle,
	&BaseEnemy::Walk,
	&BaseEnemy::Follow,
	&BaseEnemy::Knock,
	&BaseEnemy::Attack,
	&BaseEnemy::Death
};

void BaseEnemy::Idle()
{
	_status.KnockTime = 0;
	//õ“G”ÍˆÍ“ü‚Á‚½‚ç’Ç]
	if (Collision::GetLength(_player->GetPos(), _status.Pos) < _status.SearchRange)
		_action = FOLLOW;
	
	//Œü‚¢‚½•ûŒü‚ÉˆÚ“®
	//MoveDirection();
}

void BaseEnemy::Walk()
{
	//õ“G”ÍˆÍ“ü‚Á‚½‚ç’Ç]
	if (Collision::GetLength(_player->GetPos(), _status.Pos) < _status.SearchRange)
		_action = FOLLOW;

	//Œü‚¢‚½•ûŒü‚ÉˆÚ“®
	MoveDirection();
}

void BaseEnemy::RotforPlayer()
{
	//Šp“x‚ÌŽæ“¾ ƒvƒŒƒCƒ„[‚ª“G‚Ìõ“GˆÊ’u‚É“ü‚Á‚½‚çŒü‚«‚ðƒvƒŒƒCƒ„[‚Ì•û‚É
	XMVECTOR PositionA = { _player->GetPos().x,_player->GetPos().y,_player->GetPos().z };
	XMVECTOR PositionB = { _status.Pos.x, _status.Pos.y, _status.Pos.z };

	//ƒvƒŒƒCƒ„[‚Æ“G‚ÌƒxƒNƒgƒ‹‚Ì’·‚³(·)‚ð‹‚ß‚é
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	_status.Rot.y =RottoPlayer * 70.f + 180.f;

}

void BaseEnemy::Follow()
{
	AnimTim = 0;

	RotforPlayer();
	//UŒ‚”»’è
	if (!_isAttack && Collision::GetLength(_player->GetPos(), _status.Pos) < 2.f) {
		_action = ATTACK;
	}
	
	//‹Â‚¯”½‚è”»’è
	if (RecvDamage)
		_action = KNOCK;

	//Œü‚¢‚½•ûŒü‚ÉˆÚ“®
	MoveDirection();
}

void BaseEnemy::Attack()
{
	//RecvDamage = FALSE;
	_isAttack = true;

	RotforPlayer();
	AttackAction();
}

void BaseEnemy::Knock()
{
	_isFlash = TRUE;
	RecvDamage = FALSE;

	//Œü‚¢‚½•û‚ÉˆÚ“®‚·‚é
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(_status.Rot.y+180.f));

	move = XMVector3TransformNormal(move, matRot);

	_status.Pos = {
					_status.Pos.x + move.m128_f32[0] * _status.MoveSpeed,
				_status.Pos.y,
				_status.Pos.z + move.m128_f32[2] * _status.MoveSpeed
	};

	if(++_status.KnockTime>180)
	{
		_action = IDLE;
	}

}

void BaseEnemy::Death()
{
	
}

/********************************************************/

void BaseEnemy::MoveDirection()
{
	//Œü‚¢‚½•û‚ÉˆÚ“®‚·‚é
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(_status.Rot.y));

	move = XMVector3TransformNormal(move, matRot);

	_status.Pos = {
					_status.Pos.x + move.m128_f32[0] * _status.MoveSpeed,
				_status.Pos.y,
				_status.Pos.z + move.m128_f32[2] * _status.MoveSpeed
	};
}


bool BaseEnemy::DeathJudg()
{
	//‘Ì—Í0‚ÅŽ€‚Ê
	bool l_judg = _status.HP <= 0;

	if (l_judg)return true;
	return false;
}

void BaseEnemy::CollideHummmer()
{
	_status.Obb.SetParam_Pos(_status.Pos);
	_status.Obb.SetParam_Rot(_status.Tex->GetMatRot());
	_status.Obb.SetParam_Scl({1,1,1});
	//’e‚ÌXV
}

void BaseEnemy::TexInit()
{
	m_HpTex = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "white1x1.png", { 64, 64 }, { 0.f, 0.5f }, { 0, 0 }, { 128, 128 }));
	m_HpTex->SetColor(XMFLOAT4(1, 0, 0, 1));

	m_ShadowTex = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Shadow.png", { 48, 48 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 }));
	m_ShadowTex->SetRotation(XMFLOAT3( 90.f,0.f,0.f ));

}

void BaseEnemy::TexUpda()
{
	

	constexpr float GroundY = -2.5f;
	constexpr float MagniVal = 0.04f;
	float sx, sy;//HP
	float sx2, sy2;//‰e

	sx = Helper::SmoothStep_Deb(0, m_MaxHp, _status.HP) * MagniVal;
	sy = 0.005f;

	sx2= Helper::SmoothStep_Deb(GroundY, -GroundY, _status.Pos.y) * (MagniVal*4.f);
	sy2 = Helper::SmoothStep_Deb(GroundY, -GroundY, _status.Pos.y) * (MagniVal/2.f);

	m_HpTex->SetScale(XMFLOAT3(sx, sy, 1.f));
	m_HpTex->SetPosition(XMFLOAT3(_status.Pos.x+1.5f, _status.Pos.y+1.5f, _status.Pos.z));
	m_HpTex->Update();

	m_ShadowTex->SetPosition(XMFLOAT3(_status.Pos.x, GroundY, _status.Pos.z));
	m_ShadowTex->SetScale(XMFLOAT3(sx2, sy2, 1.f));
	m_ShadowTex->SetRotation(XMFLOAT3(90, 0, 0));
	m_ShadowTex->Update();
}

void BaseEnemy::TexDraw()
{
	constexpr float dis_max = 15.f;
	if (_player->GetPos().z > 7)return;

	Helper::isDraw(_player->GetPos(), _status.Pos, m_ShadowTex.get(), dis_max, _status.HP <= 0);

	Helper::isDraw(_player->GetPos(), _status.Pos, m_HpTex.get(), dis_max, _status.HP <= 0);
}

void BaseEnemy::TutorialTexDraw()
{
	constexpr float dis_max = 25.f;

	Helper::isDraw(_player->GetPos(), _status.Pos, m_ShadowTex.get(), dis_max, _status.HP <= 0);

	Helper::isDraw(_player->GetPos(), _status.Pos, m_HpTex.get(), dis_max, _status.HP <= 0);
}

void BaseEnemy::DamageFlash()
{
	if (!FlashF)return;

	if (++val > 90) {
		val = 0.f;
		FlashF = FALSE;
	} else {
		_color.y = sinf(val);
		_color.z = sinf(val);
	}
}

void BaseEnemy::PlayerHitBody(float dis, bool& f)
{
	Vector3 posp = {}, pose = {};

	Helper::ColKnock(posp, pose, _player.get(),f,dis);
}



void BaseEnemy::DestryAct(float&alpha,int hp)
{
	XMFLOAT3 pos{};
	if (hp > 0)pos=state_obj_.Pos_;

	else {
		alpha -= 0.02f;
		state_obj_.Pos_ = pos;
		if (alpha < 0.f)
		{
			RefTime++;

		}
	}
	if(RefTime>120)
	{
		state_obj_.Pos_ = { pos.x + rand() % 10 - 5,pos.y,pos.z + rand() % 10 - 5 };
		RefTime = 0;
		alpha = 1.f;
	}
}


void BaseEnemy::RecvFlashColor()
{
	constexpr float MaxCount = 60;

	if (!_isFlash&&RecvDamage)_isFlash = true;
	if(FlashCount<3&& _isFlash)
	{
	//	_color = { 0,0,0,0 };
		//ŽžŠÔ‚ÌŠ„‡‚ð‹‚ß‚é
	
	}

	if (!_isFlash)FlashCount = 0;
}

void BaseEnemy::CollideHammerDeb()
{
	Vector3 vec = {};
	Vector3 hammerPos = _player->GetHammer()->GetPosition();
	if (Collision::GetIns()->HitCircle({ hammerPos.x, hammerPos.z }, 1.0f, { state_obj_.Pos_.x, state_obj_.Pos_.z }, 1.0f) && 
		!_player->GetIsHammerRelease() && _player->GetIsAttack()) {
			Vector3 playerPos = _player->GetPos();
			
			vec = playerPos - state_obj_.Pos_;
			vec.normalize();
			vec.y = 0.0f;
			_player->HitHammerToEnemy(vec);
			SoundManager::GetIns()->PlaySE(SoundManager::SEKey::hammerAttack, 0.2f);
		}
	//int cool=DamCool
}