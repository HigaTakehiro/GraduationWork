#include "TogemaruAct.h"
#include "TogemaruAct.h"

#include <algorithm>
#include <random>

#include "Easing.h"
#include "Helper.h"
#define PI 3.14
#define PI_180 180
#define PI_360 360

Vector3 TogemaruAct::depositPos = { 10, -2.5f, 0.f };
Vector3 TogemaruAct::cameraPos = {};
Vector3 TogemaruAct::oldCameraPos = {};
Vector3 TogemaruAct::DefaultPos= {};
bool TogemaruAct::depositDelF = false;

TogemaruAct::AppearState TogemaruAct::StateArray[TogemaruAct::StateName::P_Num] =
{
	{TogemaruAct::P1,(*Phase1)},
	{TogemaruAct::P2,(*Phase2)},
	{TogemaruAct::P3,(*Phase3)}
};

void TogemaruAct::StateExecute(int state)
{
	int index = 0;

	//state�ɉ������֐��̎��s
	for(int i=0;i<StateName::P_Num;i++){
		if(StateArray[i].state==state)
		{
			StateArray[i].func();
			return;
		}
	}
}

void TogemaruAct::Phase1()
{
	//Pos_ = { 10,0,-10 };
}

void TogemaruAct::Phase2()
{

}

void TogemaruAct::Phase3()
{

}

void TogemaruAct::Transition()
{
	constexpr float inBossroomZ = 12.f;

	if(Appear()==FALSE)
	{
		return;
	}
		
	if (Player_->GetPos().z < inBossroomZ) {
		beginBattle = TRUE;
	}

	//�퓬�J�n�����瓮���悤��
	if (beginBattle) {
		(this->*ActionList[act_])();
	}

	//�j�ƃv���C���[�Փ�
	if (CollideSpear()) {
		Player_->SubHP(1);
	}

	CollideDeposit();
	//����3��ꂽ��
	if (CrushSpear() == TRUE) {
		//�����f��
		act_ = Act::RUNAWAY;
	}
	//�z�ΏՓˎ��̉�ʂ̗h��
	ViewShake();

	//�Փ˔��a
	constexpr float pr = 1.f, er = 1.5f;
	//�Փˎ��̃v���C���[�̂�����
	bool AccelJudg = (rushEaseT >= 0.2f && rushEaseT <= 0.9f) && (act_ == Act::ATTACK_SHOTSPEAR);
	float KnockDis = AccelJudg ? 0.3f : 1.f;//�̂�����l

	//����
	bool isCollide = Helper::GetCircleCollide(Player_->GetPos(), Pos_, pr, er);
	Helper::ColKnock(Player_->GetPos(), Pos_, Player_, isCollide, KnockDis);

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
	rushEaseT = 0.f;
	ShotRange = 0.f;
	spearsAlpha = { 0.f };
	waitShotCount = 0;
	reproductionTime = 0;
	//���W���Z�b�g
	for (size_t i = 0; i < spearSize; i++) {
		SpearPos_[i] = Pos_;
	}
}

void TogemaruAct::Move()
{
	anime_name_ = AnimeName::WALK;
	animationWaitTime = 0;

	movSpeed = 0.3f;
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
	if (++actionCount % ActionInter == 0)
	{
		RushStartPos = Pos_;
		for (size_t i = 0; i < spearSize; i++) {
			spearsAlpha[i] = 1.f;
		}
		anime_name_ = AnimeName::ROLE;
		act_ = Act::ATTACK_SHOTSPEAR;
	}


}

float TogemaruAct::Walk()
{
	return 0;
}

float TogemaruAct::Follow()
{
	float pi_180 = 180.f;
	//
	constexpr float AngleCorrVal = PI_180 / PI;

	//�v���C���[���W
	XMVECTOR posP = { Player_->GetPos().x,Player_->GetPos().y,Player_->GetPos().z };
	//�G���W
	XMVECTOR posE = { Pos_.x,Pos_.y,Pos_.z };
	//�G����v���C���[�ւ̃x�N�g��
	XMVECTOR Vec = XMVectorSubtract(posP, posE);
	//��̃x�N�g�������]�p���߂�
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	//�Ǐ]�p�x�Ԃ�
	return angle * AngleCorrVal;
}

void TogemaruAct::Attack_Rush()
{

}

void TogemaruAct::Attack_ShotSpear()
{
	if (++animationWaitTime < 60)return;

	constexpr float maxRushEaseT = 30.f;

	if (++rushEaseT >= maxRushEaseT) {
		depositCollideF = FALSE;
		isShot = TRUE;//�ːi�I�������j��΂�
	} else {
		//�ːi
		Pos_.x = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.x, RushStartPos.x + move.m128_f32[0] * 100.f);
		Pos_.z = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.z, RushStartPos.z + move.m128_f32[2] * 100.f);

		//�ړ����ɍz�΂ɓ���������
		depositCollideF = TRUE;
	}

	//�j��΂�
	//�҂����Ԍo�߂�����
	bool canShot = ++waitShotCount > 90;
	//���ˏI��
	bool endShot = ShotRange > 20.f;

	if (isShot) {
		anime_name_ = AnimeName::IdlE;
		if (canShot) {
			for (size_t i = 0; i < spearSize; i++) {
				spearsAlpha[i] -= 0.02f;//���񂾂񔖂�
			}
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
	XMVECTOR Vec = XMVectorSubtract(posP, posE);
	//��̃x�N�g�������]�p���߂�
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	if (crushSpearNum >= 3) {
		//�v���C���[�̋t����
		Rot_.y = angle * 60.f + 180.f;

		return true;
	}

	return false;
}

void TogemaruAct::RunAway()
{
	anime_name_ = AnimeName::CRUSH;

	//���񕜂��鎞��
	constexpr int32_t reproductionMaxTime = 240;

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
	if (++reproductionTime > reproductionMaxTime)
	{
		crushSpearNum = 0;
		act_ = Act::MOVE;
	}
}

void TogemaruAct::CollideDeposit()
{
	//����p�̔��a player - boss
	constexpr float r1 = 1.f, r2 = 2.f;

	//�z�Ε�������܂ł�3�b
	constexpr int32_t ReproductionTimeMax = 180;
	if (depositDelF)
	{
		if (++depositDelTime > ReproductionTimeMax)
		{
			//�o��
			depositPos = DepositReproduction();
			depositDelF = FALSE;
		}
	} else
	{
		bool col = Helper::GetCircleCollide({ depositPos.x,depositPos.y,depositPos.z + 3.f }, { Pos_.x,Pos_.y,Pos_.z + 3.f }, r1, r2);
		//�z�΂ƏՓ˂�����
		if (depositCollideF && col) {
			shakeF = TRUE;//��ʗh�炷
			++crushSpearNum;//�j�̐�1���炷(��ꂽ�j�̐��{�P)
			anime_name_ = AnimeName::CRUSH;
			depositDelF = TRUE;
		}
		depositDelTime = 0;
	}
}

bool TogemaruAct::CollideSpear()
{
	//���ˎ��ȊO������Ȃ�
	if (ShotRange <= 0)return false;

	for (size_t i = 0; i < spearSize; i++)
	{
		if (spearsAlpha[i] <= 0.f)continue;
		if (Helper::GetCircleCollide(Player_->GetPos(), { SpearPos_[i].x,SpearPos_[i].y,SpearPos_[i].z + 3.f }
			, 1.f, 1.f))
		{
			spearsAlpha[i] = 0.f;//����������`�����
			return true;
		}
	}
	return false;
}

Vector3 TogemaruAct::DepositReproduction()
{
	//�o�����W��4�_ ��E���E�E�E��
	Vector3 posList[] = { Vector3(0,-2.5f,-10),Vector3(0,-2.5f,10) ,Vector3(10,-2.5f,0) ,Vector3(-10,-2.5f,0) };

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 3);

	//�o��������W��PosList�̒����烉���_��
	return posList[rand(mt)];
}

void TogemaruAct::ViewShake()
{
	//�V�F�C�N����
	constexpr float movInter = 2.f;
	//�V�F�C�N��
	constexpr float shakeVibration = 10.f;

	if (shakeF) {
		shakeT++;
		shakeXVal = sinf(PI * 2 / movInter * shakeT) * shakeVibration;
		shakeYVal = sinf(PI * 2 / movInter * shakeT) * shakeVibration;
		if(shakeT>20.f){
			shakeXVal = DefaultPos.x - oldCameraPos.x;
			shakeYVal = DefaultPos.y - oldCameraPos.y;
			shakeF = FALSE;
		}
		//�V�F�C�N�l�����Z
		//o12  d14 -2  //n10 d15  +5
	}
	else{
		shakeT = 0.f;
		shakeXVal = shakeYVal = 0.f;
	}
	cameraPos.x = shakeXVal;
	cameraPos.y = shakeYVal;
}

bool TogemaruAct::Appear()
{
	StateExecute(0);
	//Pos_ = Vector3(0, 0, -10);
	return true;
}



void TogemaruAct::Death()
{

}

void (TogemaruAct::* TogemaruAct::ActionList[])() =
{
	&TogemaruAct::Move,
	&TogemaruAct::Attack_Rush,
	&TogemaruAct::Attack_ShotSpear,
	&TogemaruAct::RunAway,
	&TogemaruAct::Death
};





