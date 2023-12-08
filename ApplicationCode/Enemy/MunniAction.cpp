#include "MunniAction.h"

#include "Easing.h"

void MunniAction::Move()
{
	//���������Ɉړ�����
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//��]��Y���͊֐��̖߂�l(angle)�Ŕ���
	Rot_.y = isFollow ? Follow() : Walk();

	if(isFollow)
	{
		oldRotY = Rot_.y;
		randAddRotVal = rand() % 60 + 60;
		BeginWalkCount = 0;
		rotEaseT = 0.f;
	}

	if (BeginWalkCount > 0) {
		//���W���f
		Pos_ = {
			Pos_.x += move.m128_f32[0] * movSpeed,
			Pos_.y,
			Pos_.z += move.m128_f32[2] * movSpeed,
		};
	}
}

float MunniAction::Walk()
{
	//�J�E���^���
	constexpr float CountMax = 90;
	constexpr uint32_t WalkEndTime=120;

	//�ҋ@�I�� ���邫�n�߂�
	if(rotEaseT>=CountMax)
	{
		oldRotY = Rot_.y;
		randAddRotVal = rand() % 60+60;

		if(++BeginWalkCount>WalkEndTime)
		{
			rotEaseT = 0.f;
		}
	}
	//�ҋ@�� ����������
	else
	{
		BeginWalkCount = 0;
		rotEaseT++;
		return Easing::easeIn(rotEaseT, 60.f, oldRotY, oldRotY + (float)randAddRotVal);
	}
	return Rot_.y;
}

float MunniAction::Follow()
{
	constexpr float AngleCorrVal = 70.f;
	float pi_180 = 180.f;
	//�p�x�̎擾 
	XMVECTOR PositionA = { m_Player_->GetPos().x,m_Player_->GetPos().y,m_Player_->GetPos().z };
	XMVECTOR PositionB = { Pos_.x, Pos_.y, Pos_.z };

	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	float Angle;
	Angle = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	return Angle * AngleCorrVal + pi_180;
}

void MunniAction::Attack()
{
	
}

void MunniAction::Death()
{
	
}



