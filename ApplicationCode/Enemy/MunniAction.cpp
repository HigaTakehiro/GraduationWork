#include "MunniAction.h"

#include <algorithm>
#include"Collision.h"
#include "Easing.h"
#include "Shapes.h"
#include"Helper.h"

MunniAction::MunniAction()
{
	ImpTexInit();
}

MunniAction* MunniAction::GetIns()
{
	static MunniAction ins;
	return &ins;
}

//
// �e�N�X�`��������
//
void MunniAction::ImpTexInit()
{
	Model*model= Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "impact.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 0.f, 2.0f }, { 64.0f, 64.0f });
	Model*model2= Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "Area.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 0.f, 2.0f }, { 64.0f, 64.0f });

	ImpactTex = Object3d::UniquePtrCreate(model);
	ImpactTex->SetColType(Object3d::CollisionType::Obb);
	ImpactTex->SetObjType((int32_t)Object3d::OBJType::Enemy);
	ImpactTex->SetObbScl({ 1.f,1.f,1.f });
	ImpactTex->SetHitRadius(0.5f);

	ImpactAreaTex = Object3d::UniquePtrCreate(model2);
	ImpactAreaTex->SetColType(Object3d::CollisionType::Obb);
	ImpactAreaTex->SetObjType((int32_t)Object3d::OBJType::Enemy);
	ImpactAreaTex->SetObbScl({ 1.f,1.f,1.f });
	ImpactAreaTex->SetHitRadius(0.5f);

	impTexScl = Vector3(0, 0, 0);
}


//
// �ړ�
//
void MunniAction::Move()
{
	XMFLOAT2 Pos_P = { m_Player_->GetPos().x,m_Player_->GetPos().z };//Player
	XMFLOAT2 Pos_E = { Pos_.x,Pos_.z+3.f };//Enemy

	if(Collision::HitCircle(Pos_E,1.f,Pos_P,3.f)&&!isSearch)
	{
		isFollow = !isFollow;//�t���O�؂�ւ�
		isSearch = TRUE;//�ǐՂ���
	}

	constexpr float pr = 1.f, er = 1.2f;
	bool isCollide = Helper::GetCircleCollide(m_Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z+3.f}, pr, er);
	Helper::ColKnock(m_Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z}, m_Player_, isCollide, 0.8f);

	//�ǐՎ���
	FollowCount = isSearch ? ++FollowCount : 0;

	//�ǐՏI���̏���
	bool FolSuspension = FollowCount > 180;
	if(FolSuspension)
	{
		isSearch = FALSE;
		isFollow = isSearch;
	}

	//���������Ɉړ�����
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//��]��Y���͊֐��̖߂�l(angle)�Ŕ���
	Rot_.y = isFollow ? Follow() : Walk();

	if(isFollow)
	{
		//���͈͓��{30�t���[���߂�������U������
		constexpr uint32_t nearPlayerCountMax = 30;
		if (Collision::HitCircle(Pos_E, 1.f, Pos_P, 2.f)) {
			if (++atckCoolTime >nearPlayerCountMax ){
				isAtck = TRUE;
				list_ = List::ATTACK;
			}
		}

		//�e�p�����[�^���Z�b�g�Ȃ�
		oldRotY = Rot_.y;//Y����Rot�ێ�
		randAddRotVal = rand() % 60 + 60;//��̉�]�l�ɉ�����l
		BeginWalkCount = 0;//���邫���̃J�E���g���Z�b�g
		rotEaseT = 0.f;//�C�[�W���O�J�E���g
	}

	//������������������{�ǐՏ�Ԃ̎�
	if (BeginWalkCount > 0||isFollow) {
		//���W���f
		Pos_ = {
			Pos_.x += move.m128_f32[0] * movSpeed,
			Pos_.y,
			Pos_.z += move.m128_f32[2] * movSpeed,
		};
	}

	// �U���Ɏg���p�����[�^���Z�b�g
	m_JumpInter_ = 0;
	atckTimer_ = 0;
}

//
// �ړ�(�p�j)
//
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

//
// �ړ�(�Ǐ])
//
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

//
// �U��
//
void MunniAction::Attack()
{
	//�҂�����
	constexpr uint32_t waitTime = 90;
	//�W�����v
	constexpr uint32_t next_Phase = 30;
	//�W�����v�̒��n��
	bool onGround = m_JumpInter_ >= 1.f;

	atckTimer_ = isAtck ? ++atckTimer_ : 0;

	if (atckTimer_ > waitTime) {//�����҂�����
		Jump();//���
		if(onGround) {//�n�ʒ��n���
			isTexScling = TRUE;//�Ռ��g�o��
			ImpactTex->SetPosition(Vector3(Pos_.x, InitPos.y+0.1f, Pos_.z));//�e�N�X�`�����W(InitPos�͒n�ʗp)
			isAtck = false;//�U���t���O�؂�
			list_ = List::MOVE;//�ړ��ɖ߂�
		}
		else
		{
			ImpactAreaTex->SetPosition(Vector3(Pos_.x, InitPos.y + 0.1f, Pos_.z));
			impAreaalpha += 0.02f;
		}
		shakeCount = 0.f;
	}
	//�Ղ�Ղ�k����
	else
	{
		shakeCount++;
	}
	//�k�킷�̂�sin�g
	Pos_.x = Pos_.x + (sinf(shakeCount) / 30.f);
	//
	isFollow = isSearch = FALSE;
	atckCoolTime = 0;
}

//
// ����
//
void MunniAction::Death()
{
	
}

//
// �U��(Jump)
//
void MunniAction::Jump()
{
	float SubPower = 0.001f;
	//�����̊ɋ}
	constexpr float Distortion = 3.f;
	//�n�ʂ̍���
	float GroundY = InitPos.y;
	//�W�����v����
	constexpr float Height = 4.6f;
	/* �ϊ� */
	constexpr float pi = 3.14f;
	//�h�b�X������

	m_JumpInter_ += 1.f / 60.f;
	Pos_.y = GroundY + (1.0f - pow(1.0f - sin(pi * m_JumpInter_), Distortion)) * Height;

	m_JumpInter_ = std::clamp(m_JumpInter_, 0.f, 1.f);
}

//
// �e�N�X�`���X�V
//
//
void MunniAction::ImpTexUpda()
{
	//���Z�l,���Z�l
	constexpr float SubTexAlphaVal = 0.05f;//alpha
	constexpr float AddTexScl = 0.005f;//scl

	if (isTexScling)
	{
		constexpr float pr = 1.f, er = 2.5f;

		bool isCollide = Helper::GetCircleCollide(m_Player_->GetPos(), Pos_, pr, er);

		Helper::ColKnock(m_Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z }, m_Player_, isCollide, 1.f);
		if (isCollide) {
			m_Player_->SubHP(1);
		}
		// �g��
		impTexScl = Vector3(impTexScl.x + AddTexScl,
			impTexScl.y + AddTexScl,
			impTexScl.z
		);
		// �A���t�@�l���Z
		impTexAlpha -= SubTexAlphaVal;

		if (impTexAlpha <= 0.f) {
			isTexScling = FALSE;
		}
		impAreaalpha -= 0.05f;
	} else
	{
		impTexScl = Vector3(0, 0, 0);
		impTexAlpha = 1.f;
	}

	//�W�����v�̒��n���ɏo��Ռ��g
	ImpactTex->SetScale(impTexScl);
	ImpactTex->SetRotation(Vector3(90, 0, 0));
	ImpactTex->SetColor({ 1,1,1,impTexAlpha });
	ImpactTex->Update();

	//�͈�
	ImpactAreaTex->SetScale({0.05f,0.05f,1.f});
	ImpactAreaTex->SetRotation(Vector3(90, 0, 0));
	ImpactAreaTex->SetColor({ 1,1,1,impAreaalpha });
	ImpactAreaTex->Update();

	impAreaalpha = std::clamp(impAreaalpha, 0.f, 0.3f);
	impTexAlpha = std::clamp(impTexAlpha, 0.f, 1.f);
}

// �s���J��
//
void MunniAction::AttackTransition()
{
	(this->*ActionList[list_])();

	ImpTexUpda();
}

//
// �֐��|�C���^(�s���p��)
//
void (MunniAction::* MunniAction::ActionList[])() = {
	&MunniAction::Move,
	&MunniAction::Attack,
	&MunniAction::Death
};

//
// �Ռ��g�̃e�N�X�`���`��
//
void MunniAction::ImpTexDraw()
{
	ImpactAreaTex->Draw();
	ImpactTex->Draw();
}

void MunniAction::IdleShake(bool shakestop)
{
	shakeCount++;

	Pos_.x = Pos_.x + (sinf(shakeCount)/30.f);
}
