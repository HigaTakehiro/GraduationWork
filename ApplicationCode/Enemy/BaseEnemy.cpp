#include "BaseEnemy.h"

#include "Collision.h"
#include "MouseInput.h"


/***                    GETTER                      ***/
/******************************************************/
unsigned int BaseEnemy::GetHP() const { return _status.HP; }
unsigned int BaseEnemy::GetAttackVal() const { return _status.DamageValue; }

XMFLOAT3 BaseEnemy::GetPos() const { return _status.Pos; }
XMFLOAT3 BaseEnemy::GetRot() const { return _status.Rot; }
XMFLOAT3 BaseEnemy::GetScl() const { return  _status.Scl; }
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
	//プレイヤー仮(クラス作ったらインスタンスは合わせる)
	_player = std::make_shared<XMFLOAT3>();
	*_player = { 10,0,0 };

	//索敵範囲入ったら追従
	if (Collision::GetLength(*_player, _status.Pos) < _status.SearchRange)
		_action = FOLLOW;

	//向いた方向に移動
	//MoveDirection();
}

void BaseEnemy::Walk()
{
	//索敵範囲入ったら追従
	if (Collision::GetLength(*_player, _status.Pos) < _status.SearchRange)
		_action = FOLLOW;

	//向いた方向に移動
	MoveDirection();
}

void BaseEnemy::Follow()
{
	AnimTim = 0;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	XMVECTOR PositionA = {10,0,0};
	XMVECTOR PositionB = { _status.Pos.x, _status.Pos.y, _status.Pos.z };

	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	_status.Rot={ 0.f, RottoPlayer * 60.f + 180.f,0.f };

	//攻撃判定
	if (Collision::GetLength(*_player, _status.Pos) < 1.f)
		_action = ATTACK;

	if (MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK))RecvDamage = TRUE;

	//仰け反り判定
	if (RecvDamage)
		_action = KNOCK;

	//向いた方向に移動
	MoveDirection();
}

void BaseEnemy::Attack()
{
	RecvDamage = FALSE;

	//アニメーションカウンタ進める
	AnimTim++;
	if (AnimTim > 120)
		_action = FOLLOW;
}

void BaseEnemy::Knock()
{
	//向いた方に移動する
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(_status.Rot.y+180.f));

	move = XMVector3TransformNormal(move, matRot);

	_status.Pos = {
					_status.Pos.x + move.m128_f32[0] * _status.MoveSpeed,
				_status.Pos.y,
				_status.Pos.z + move.m128_f32[2] * _status.MoveSpeed
	};

	if(_status.KnockTime>120)
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
	//向いた方に移動する
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
	//体力0で死ぬ
	bool l_judg = _status.HP <= 0;

	if (l_judg)return true;
	return false;
}
