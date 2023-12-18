#include "NormalEnemyB.h"

#include <algorithm>
#include "Shapes.h"

void NormalEnemyB::Init()
{
	state_obj_.TexSize_ = 6;
	state_obj_.Model_.resize(state_obj_.TexSize_);

	for (size_t i = 0; i < state_obj_.TexSize_; i++) {
		state_obj_.Model_[i] = Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "rockmuni_idle.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 64.0f * (float)i, 2.0f }, { 64.0f, 64.0f });
	}
	state_obj_.obj_ = Object3d::UniquePtrCreate(state_obj_.Model_[m_AnimationCount]);

	state_obj_.obj_->SetColType(Object3d::CollisionType::Obb);
	state_obj_.obj_->SetObjType((int32_t)Object3d::OBJType::Enemy);
	state_obj_.obj_->SetObbScl({ 1.f,1.f,1.f });
	state_obj_.obj_->SetHitRadius(0.5f);
	state_obj_.Scl={ 0.02f, 0.02f, 0.02f };

	Tag_ = "Munni";
	action_ = new MunniAction();

}

void NormalEnemyB::Upda(Camera* camera)
{
	action_->SetPlayer(_player.get());
	//�s���J��
	action_->AttackTransition();
	state_obj_.Pos_ = action_->GetPos();
	state_obj_.Rot_ = action_->GetRor();

	// �A�j���[�V�����̃J�E���^
	{
		//�A�j���[�V�����Ԋu
		constexpr uint32_t animationInter = 30;
		//�C���f�N�X
		int animeTexIndx = m_AnimationCount / animationInter;

		if (++m_AnimationCount % animationInter == 0) {
			// �Ō�܂ōs������O�Ԃ�
			if (animeTexIndx > (state_obj_.TexSize_ - 1)) {
				m_AnimationCount = 0;
			} else {
				//�e�N�X�`���؂�ւ�30�t���[����
				state_obj_.obj_ = Object3d::UniquePtrCreate(state_obj_.Model_[animeTexIndx]);
			}
		}
		//�C���f�b�N�X�͈͐���
		animeTexIndx= std::clamp(animeTexIndx, 0, state_obj_.TexSize_-1);
	}


	//�e��p�����[�^�Z�b�g �X�V
	//state_obj_.obj_->SetRotation(state_obj_.Rot_);
	state_obj_.obj_->SetScale(state_obj_.Scl);
	state_obj_.obj_->SetPosition(state_obj_.Pos_);
	state_obj_.obj_->SetColor(state_obj_.Color_);
	state_obj_.obj_->SetIsBillboardY(true);
	state_obj_.obj_->Update();
}


#include "DirectXSetting.h"
void NormalEnemyB::Draw()
{
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	action_->ImpTexDraw();
	state_obj_.obj_->Draw();
	Object3d::PostDraw();
}

void NormalEnemyB::Jump()
{
	float SubPower = 0.001f;
	//�����̊ɋ}
	constexpr float Distortion = 1.f;
	//�n�ʂ̍���
	constexpr float GroundY = -2.5f;
	//�W�����v����
	constexpr float Height = 0.6f;
	/* �ϊ� */
	constexpr float pi = 3.14f;
	//�h�b�X������

	m_JumpInter_ += 1.f / 60.f;
	_status.Pos.y = GroundY + (1.0f - pow(1.0f - sin(pi * m_JumpInter_), Distortion)) * Height;

	m_JumpInter_ = std::clamp(m_JumpInter_, 0.f, 1.f);
}

void NormalEnemyB::AttackAction()
{

}

void NormalEnemyB::TextureAnimation()
{

}

void NormalEnemyB::TutorialUpda(Camera* camera, bool flag)
{

}
void NormalEnemyB::TutorialDraw(float Mindis)
{

}

void NormalEnemyB::SetPosDeb(Vector3 pos)
{
	//�s���J�ڃN���X�ɑ�����W(����������������)
	action_->SetInitPos(state_obj_.Pos_);
}

