#include "NormalEnemyB.h"

#include <algorithm>

#include "Shapes.h"

void NormalEnemyB::Init()
{
	for (size_t i = 0; i < state_obj_.TexSize_; i++) {
		state_obj_.Model_[i] = Shapes::CreateSquare({ 0, 0 }, { 64.f, 64.0f }, "dogomu_face.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)i, 0.0f }, {64.0f, 64.0f });
	}
	state_obj_.obj_ = Object3d::UniquePtrCreate(state_obj_.Model_[0]);

	state_obj_.obj_->SetColType(Object3d::CollisionType::Obb);
	state_obj_.obj_->SetObjType((int32_t)Object3d::OBJType::Enemy);
	state_obj_.obj_->SetObbScl({ 1.f,1.f,1.f });
	state_obj_.obj_->SetHitRadius(0.5f);
	state_obj_.obj_->SetScale({ 1.0f, 1.0f, 1.0f });

	Tag_ = "Munni";

}

void NormalEnemyB::Upda(Camera* camera)
{

	state_obj_.obj_->SetRotation(state_obj_.Rot_);
	state_obj_.obj_->SetScale(state_obj_.Scl);
	state_obj_.obj_->SetPosition(state_obj_.Pos_);
	state_obj_.obj_->SetColor(state_obj_.Color_);
	state_obj_.obj_->Update();
}

void NormalEnemyB::Draw()
{
	state_obj_.obj_->Draw();
}

void NormalEnemyB::Jump()
{
	float SubPower = 0.001f;
	//落下の緩急
	constexpr float Distortion = 1.f;
	//地面の高さ
	constexpr float GroundY = -2.5f;
	//ジャンプ高さ
	constexpr float Height = 0.6f;
	/* 変換 */
	constexpr float pi = 3.14f;
	//ドッスン挙動

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

