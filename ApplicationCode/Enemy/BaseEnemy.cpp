#include "BaseEnemy.h"

#include "Collision.h"
#include "KeyInput.h"
#include "MouseInput.h"
#include"Easing.h"


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
	_status.KnockTime = 0;
	//���G�͈͓�������Ǐ]
	if (Collision::GetLength(_player->GetPos(), _status.Pos) < _status.SearchRange)
		_action = FOLLOW;
	
	//�����������Ɉړ�
	//MoveDirection();
}

void BaseEnemy::Walk()
{
	//���G�͈͓�������Ǐ]
	if (Collision::GetLength(_player->GetPos(), _status.Pos) < _status.SearchRange)
		_action = FOLLOW;

	//�����������Ɉړ�
	MoveDirection();
}

void BaseEnemy::RotforPlayer()
{
	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	XMVECTOR PositionA = { _player->GetPos().x,_player->GetPos().y,_player->GetPos().z };
	XMVECTOR PositionB = { _status.Pos.x, _status.Pos.y, _status.Pos.z };

	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	_status.Rot.y =RottoPlayer * 70.f + 180.f;

}

void BaseEnemy::Follow()
{
	AnimTim = 0;

	RotforPlayer();
	//�U������
	if (!_isAttack && Collision::GetLength(_player->GetPos(), _status.Pos) < 2.f) {
		_action = ATTACK;
	}
	
	//�����蔻��
	if (RecvDamage)
		_action = KNOCK;

	//�����������Ɉړ�
	MoveDirection();
}

void BaseEnemy::Attack()
{
	RecvDamage = FALSE;
	_isAttack = true;

	RotforPlayer();
	AttackAction();
}

void BaseEnemy::Knock()
{
	_isFlash = TRUE;
	RecvDamage = FALSE;

	//���������Ɉړ�����
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
	//���������Ɉړ�����
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
	//�̗�0�Ŏ���
	bool l_judg = _status.HP <= 0;

	if (l_judg)return true;
	return false;
}

void BaseEnemy::CollideHummmer()
{
	_status.Obb.SetParam_Pos(_status.Pos);
	_status.Obb.SetParam_Rot(_status.Tex->GetMatRot());
	_status.Obb.SetParam_Scl({1,1,1});
	//�e�̍X�V
}

void BaseEnemy::RecvFlashColor()
{
	constexpr float MaxCount = 60;

	if (!_isFlash&&RecvDamage)_isFlash = true;
	if(FlashCount<3&& _isFlash)
	{
		_color = { 0,0,0,0 };
		//���Ԃ̊��������߂�
	
	}

	if (!_isFlash)FlashCount = 0;
}
