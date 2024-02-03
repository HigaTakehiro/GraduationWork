#include "Grass.h"
#include"Collision.h"
#include "Shapes.h"
#include "SafeDelete.h"

Grass::~Grass()
{
	for (int32_t i = 0; i < 3; i++) {
		safe_delete(model_[i]);
	}
	safe_delete(grass_);
}

void Grass::Initialize(const Vector3& pos, int I)
{
	for (int32_t i = 0; i < 3; i++) {
		if (I % 2 == 0) {
			model_[i] = Shapes::CreateSquare({ 0.f, 0.f }, { 64.f, 32.f }, "grass.png", { 1.8f, 0.9f }, { 0.5f, 0.5f }, { 64.f * (float)i, 2.f }, { 64.f, 32.f });
		}
		else {
			model_[i] = Shapes::CreateSquare({ 0.f, 0.f }, { 64.f, 64.f }, "herb.png", { 1.8f, 0.9f }, { 0.5f, 0.5f }, { 64.f * (float)i, 2.f }, { 64.f, 64.f });
		}
	}
	grass_ = Object3d::Create(model_[0]);
	grass_->Initialize();
	grass_->SetPosition(pos);
	grass_->SetScale({ 0.4f,0.9f,1.0f });
	animeTimer_ = 0;
	animeTime_ = 30;
	animeCount_ = 0;
}

void Grass::Update(const Vector3& playerPos) {
	AlphaTest(playerPos);

	if (++animeTimer_ >= animeTime_) {
		if (++animeCount_ >= 3) animeCount_ = 0;
		animeTimer_ = 0;

		grass_->SetModel(model_[animeCount_]);
		grass_->Initialize();
	}

	grass_->Update();
}

void Grass::Draw()
{
	grass_->Draw();
}

void Grass::AlphaTest(const Vector3& playerPos)
{
	float grassSize = 2.f;
	Vector3 grassPos = grass_->GetPosition();
	if (Collision::GetIns()->HitCircle({ grassPos.x, grassPos.z }, 1.0f, { playerPos.x, playerPos.z }, 2.0f)) {
		grass_->SetColor({ 1.f, 1.f, 1.f, 0.5f });
	}
	else {
		grass_->SetColor({ 1.f, 1.f, 1.f, 1.f });
	}
}
