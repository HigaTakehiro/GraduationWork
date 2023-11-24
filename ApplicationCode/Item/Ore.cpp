#include "Ore.h"
#include "Shapes.h"
#include "SafeDelete.h"

Ore::~Ore() {
	safe_delete(oreModel_);
}

void Ore::Initialize(Vector3 pos, Vector3 vec)
{
	pos_ = pos;
	vec_ = vec.normalize();
	oreModel_ = Shapes::CreateSquare({ 0, 0 }, { 16, 16 }, "Ore.png", { 16, 16 }, {0.0f, 0.0f});
	ore_ = Object3d::UniquePtrCreate(oreModel_);
	ore_->SetScale({ 0.02f, 0.02f, 0.02f });
	ore_->SetObbScl({ 0.2f,0.2f,0.2f });
	ore_->SetColType(Object3d::CollisionType::Obb);
	ore_->SetObjType((int32_t)Object3d::OBJType::Item);
	ore_->SetIsBillboardY(true);
	ore_->SetHitRadius(0.5f);
	speed_ = 1.0f;
}

void Ore::Update()
{
	Vector3 vec = vec_ * speed_;
	pos_ += vec;
	ore_->SetPosition(pos_);
	ore_->Update();
	if (speed_ > 0) {
		speed_ -= 0.1f;
	}
	else {
		speed_ = 0.0f;
	}
}

void Ore::Draw()
{
	ore_->Draw();
}

void Ore::Finalize()
{
	safe_delete(oreModel_);
}

bool Ore::GetIsHit()
{
	if (speed_ <= 0) {
		return true;
	}
	return false;
}
