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
	//�v���C���[��(�N���X�������C���X�^���X�͍��킹��)
	_player = std::make_shared<XMFLOAT3>();
	*_player = { 10,0,0 };

	//���G�͈͓�������Ǐ]
	if (Collision::GetLength(*_player, _status.Pos) < _status.SearchRange)
		_action = FOLLOW;

	//�����������Ɉړ�
	//MoveDirection();
}

void BaseEnemy::Walk()
{
	//���G�͈͓�������Ǐ]
	if (Collision::GetLength(*_player, _status.Pos) < _status.SearchRange)
		_action = FOLLOW;

	//�����������Ɉړ�
	MoveDirection();
}

void BaseEnemy::Follow()
{
	AnimTim = 0;

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	XMVECTOR PositionA = {10,0,0};
	XMVECTOR PositionB = { _status.Pos.x, _status.Pos.y, _status.Pos.z };

	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	float RottoPlayer;
	RottoPlayer = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	_status.Rot={ 0.f, RottoPlayer * 60.f + 180.f,0.f };

	//�U������
	if (Collision::GetLength(*_player, _status.Pos) < 1.f)
		_action = ATTACK;

	if (MouseInput::GetIns()->TriggerClick(MouseInput::RIGHT_CLICK))RecvDamage = TRUE;

	//�����蔻��
	if (RecvDamage)
		_action = KNOCK;

	//�����������Ɉړ�
	MoveDirection();
}

void BaseEnemy::Attack()
{
	RecvDamage = FALSE;

	//�A�j���[�V�����J�E���^�i�߂�
	AnimTim++;
	if (AnimTim > 120)
		_action = FOLLOW;
}

void BaseEnemy::Knock()
{
	//���������Ɉړ�����
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
