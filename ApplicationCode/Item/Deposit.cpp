#include "Deposit.h"
#include "Shapes.h"
#include "SafeDelete.h"
#include <random>

Deposit::~Deposit()
{
	safe_delete(model_);
}

void Deposit::Initialize()
{
	model_ = Shapes::CreateSquare({ 0.0f, 0.0f }, { 256.0f, 256.0f }, "frontRock2.png", { 32.0f, 32.0f }, { 0.5f, 0.5f }, {0.0f, 0.0f}, {256.0f, 256.0f});
	deposit_ = Object3d::UniquePtrCreate(model_);
	deposit_->SetColType(Object3d::CollisionType::Obb);
	deposit_->SetObjType((int32_t)Object3d::OBJType::Object);
	deposit_->SetObbScl({ 2.f, 2.f, 2.f });

	hp_ = 5;
	hitCoolTimer_ = hitCoolTime_ = 30;

}

void Deposit::Update()
{
	//���G���ԍX�V
	if (hitCoolTime_ > hitCoolTimer_) {
		hitCoolTimer_++;
	}

	deposit_->Update();

}

void Deposit::Draw()
{
	deposit_->Draw();
}

Vector3 Deposit::OreDropVec()
{
	//�z�΃h���b�v�x�N�g��
	Vector3 vec = { 0.f, 0.f, 0.f };

	//��������
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 10);

	//�z�΃h���b�v�x�N�g�����
	vec.x = (float)rand(mt) * 0.01f;
	vec.z = (float)rand(mt) * 0.01f;

	return vec;
}

bool Deposit::GetIsHit()
{
	if (deposit_->GetIsHit() && hitCoolTimer_ >= hitCoolTime_) {
		hitCoolTimer_ = 0;
		return true;
	}

	return false;
}
