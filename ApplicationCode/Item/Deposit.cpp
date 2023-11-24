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
	//無敵時間更新
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
	//鉱石ドロップベクトル
	Vector3 vec = { 0.f, 0.f, 0.f };

	//乱数生成
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 10);

	//鉱石ドロップベクトル代入
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
