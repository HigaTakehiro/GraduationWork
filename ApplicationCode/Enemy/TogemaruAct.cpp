#include "TogemaruAct.h"

#include <algorithm>

#include "Easing.h"
#define PI 3.14
#define PI_180 180
#define PI_360 360

void TogemaruAct::Transition()
{
	constexpr float inBossroomZ = 12.f;
	if (Player_->GetPos().z < inBossroomZ)
	{
		beginBattle = TRUE;
	}

	//�퓬�J�n�����瓮���悤��
	if (beginBattle) {
		(this->*ActionList[act_])();
	}

	//���W�͈͎̔w��
	Pos_.x = std::clamp(Pos_.x, -10.f, 10.f);
	Pos_.z = std::clamp(Pos_.z, -12.f, 8.8f);
}

//
//
//
void TogemaruAct::ResetParam_Spear()
{
	//
	rushEaseT =0.f;
	ShotRange = 0.f;
	spearsAlpha = 0.f;
	waitShotCount = 0;
	reproductionTime = 0;
	//���W���Z�b�g
	for (size_t i = 0; i < spearSize; i++) {
		SpearPos_[i] = Pos_;
	}
}

void TogemaruAct::Move()
{
	movSpeed = 0.1f;
	//���������Ɉړ�����
	move = { 0.f,0.f, 0.1f, 0.0f };
	matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//��ɒǏ]
	isFollow = TRUE;

	Rot_.y = isFollow ? Follow() : Walk();

	//���W���f(����������)
	Pos_ = {
		Pos_.x += move.m128_f32[0] * movSpeed,
		Pos_.y,
		Pos_.z += move.m128_f32[2] * movSpeed,
	};

	ResetParam_Spear();

	constexpr uint32_t ActionInter = 60;
	//�U���Ɉڍs
	if(++actionCount%ActionInter==0)
	{
		RushStartPos = Pos_;
		spearsAlpha = 1.f;
		act_ = Act::ATTACK_SHOTSPEAR;
	}

	//����3��ꂽ��
	if(CrushSpear()==TRUE)
	{
		//�����f��
		act_ = Act::RUNAWAY;
	}
}

float TogemaruAct::Walk()
{
	return 0;
}

float TogemaruAct::Follow()
{
	//
	constexpr float AngleCorrVal = 70.f;
	float pi_180 = 180.f;

	//�v���C���[���W
	XMVECTOR posP = { Player_->GetPos().x,Player_->GetPos().y,Player_->GetPos().z };
	//�G���W
	XMVECTOR posE = { Pos_.x,Pos_.y,Pos_.z };
	//�G����v���C���[�ւ̃x�N�g��
	XMVECTOR Vec = XMVectorSubtract(posE, posP);
	//��̃x�N�g�������]�p���߂�
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	//�Ǐ]�p�x�Ԃ�
	return angle * AngleCorrVal + pi_180;
}

void TogemaruAct::Attack_Rush()
{
	
}

void TogemaruAct::Attack_ShotSpear()
{
	constexpr float maxRushEaseT = 30.f;

	if (++rushEaseT >= maxRushEaseT) {
		isShot = TRUE;//�ːi�I�������j��΂�
	}
	else {
			//�ːi
			Pos_.x = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.x, RushStartPos.x + move.m128_f32[0] * 100.f);
			Pos_.z = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.z, RushStartPos.z + move.m128_f32[2] * 100.f);
	}

	//�j��΂�
	//�҂����Ԍo�߂�����
	bool canShot = ++waitShotCount > 90;
	//���ˏI��
	bool endShot =ShotRange>20.f;

	if (isShot) {
		if (canShot) {
			spearsAlpha -= 0.02f;//���񂾂񔖂�
			ShotRange += 0.2f;//�͈͍L���Ă�
		}
		//���ˏI��
		if (endShot) {
			isShot = FALSE;//
			act_ = MOVE;//��Ԃ��ړ���
		}
	}
	//�j�z�u(�~��)
	for (size_t i = 0; i < spearSize; i++) {
		//�e����ԍۂ̉�]�p
		SpearAngle_[i] = static_cast<float>(i) * (PI_360 / (float)spearSize) + PI_180;

		//���W�ړ�
		SpearPos_[i].x = Pos_.x + sinf(SpearAngle_[i] * (PI / PI_180)) * ShotRange;
		SpearPos_[i].z = Pos_.z + cosf(SpearAngle_[i] * (PI / PI_180)) * ShotRange;
	}

	//�A�N�V�����̃C���^�[�o�����Z�b�g
	actionCount = 1;
}

bool TogemaruAct::CrushSpear()
{
	//�v���C���[���W
	XMVECTOR posP = { Player_->GetPos().x,Player_->GetPos().y,Player_->GetPos().z };
	//�G���W
	XMVECTOR posE = { Pos_.x,Pos_.y,Pos_.z };
	//�G����v���C���[�ւ̃x�N�g��
	XMVECTOR Vec = XMVectorSubtract(posE, posP);
	//��̃x�N�g�������]�p���߂�
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	if(crushSpearNum>=3){
		//�v���C���[�̋t����
		Rot_.y=angle * 70.f;

		return true;
	}

	return false;
}

void TogemaruAct::RunAway()
{
	//���񕜂��鎞��
	constexpr int32_t reproductionMaxTime = 180;

	//���������Ɉړ�����
	move = { 0.f,0.f, 0.1f, 0.0f };
	matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//���W���f(����������)
	Pos_ = {
		Pos_.x += move.m128_f32[0] * movSpeed,
		Pos_.y,
		Pos_.z += move.m128_f32[2] * movSpeed,
	};

	actionCount = 0;
	//����������������ړ��J�n
	if(++reproductionTime>reproductionMaxTime)
	{
		crushSpearNum = 0;
		act_ = Act::MOVE;
	}
}


void TogemaruAct::Death()
{
	
}

void (TogemaruAct::*TogemaruAct::ActionList[])() =
{
	&TogemaruAct::Move,
	&TogemaruAct::Attack_Rush,
	&TogemaruAct::Attack_ShotSpear,
	&TogemaruAct::RunAway,
	&TogemaruAct::Death
};





