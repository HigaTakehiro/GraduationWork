#include "Deposit.h"
#include "Shapes.h"
#include "SafeDelete.h"
#include <random>

Deposit::~Deposit()
{
	safe_delete(model_);
}

void Deposit::Initialize(Vector3 pos,bool isDestroy,Camera*camera)
{
	model_ = Shapes::CreateSquare({ 0.0f, 0.0f }, { 64.0f, 64.0f }, "ironOre.png", { 2.0f, 2.0f }, { 0.5f, 0.5f }, {0.0f, 2.0f}, {64.0f, 64.0f});
	deposit_ = Object3d::UniquePtrCreate(model_);
	deposit_->SetColType(Object3d::CollisionType::Obb);
	deposit_->SetObjType((int32_t)Object3d::OBJType::Object);
	deposit_->SetObbScl({ 2.f, 2.f, 2.f });
	deposit_->SetPosition(pos);

	hp_ = 5;
	hitCoolTimer_ = hitCoolTime_ = 30;

	if (isDestroy) {
		this->isDestroy = isDestroy;
		color_ = { 1,1,1,1 };
		if(!BossDestroy)
		parPos_ = pos;//生成座標
		desParticle_ = ParticleManager::UniquePtrCreate(DirectXSetting::GetIns()->GetDev(), camera);
	}
}

void Deposit::Update(const Vector3& playerPos)
{
	//AlphaTest(playerPos);

	//無敵時間更新
	if (hitCoolTime_ > hitCoolTimer_) {
		hitCoolTimer_++;
	}


	if(this->isDestroy)
	{
		//エフェクト生成
		if (destroyF_) {
			DestroyEffect();
			//鉱石透明に
			color_.z -= 0.01f;
		}
		
		desParticle_->Update();

		deposit_->SetColor(color_);
	}

	deposit_->Update();

}

void Deposit::ParticleDraw()
{
	if (!this->isDestroy)return;

	desParticle_->Draw(DirectXSetting::GetIns()->GetCmdList());
}


void Deposit::DestroyEffect()
{
	for (size_t i=0;i<1; i++) {

		if (color_.z <= 0.8f)break;
		const float rnd_pos = 0.01f;
		Vector3 ppos = parPos_;
		ppos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		ppos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		ppos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.09f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		desParticle_->Add(60, ppos, vel, acc, 1.5f, 0.2f, { 0,0.f,0 }, { 0.2f,0.2f,0.2f }, 1, 0);
	}
}

void Deposit::Draw()
{
	deposit_->Draw();
}

Vector3 Deposit::OreDropVec()
{
	//鉱石ドロップベクトル
	Vector3 vec = { 0.f, 0.f, 0.f };
	//
	//乱数生成
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 10);

	//鉱石ドロップベクトル代入
	vec.x = (float)rand(mt) * 0.01f - 0.05f;
	vec.z = (float)rand(mt) * 0.01f - 0.05f;

	return vec;
}

bool Deposit::GetIsHit(bool isHammerSwing)
{
	if (deposit_->GetIsHit() && hitCoolTimer_ >= hitCoolTime_ && isHammerSwing) {
		hp_--;
		hitCoolTimer_ = 0;
		return true;
	}

	return false;
}

void Deposit::AlphaTest(const Vector3& playerPos)
{
	float depositSize = 2.f;
	Vector3 depositPos = deposit_->GetPosition();
	if (depositPos.x + depositSize >= playerPos.x && depositPos.x - depositSize <= playerPos.x && depositPos.z >= playerPos.z) {
		deposit_->SetColor({ 1.f, 1.f, 1.f, 0.5f });
	}
	else {
		deposit_->SetColor({ 1.f, 1.f, 1.f, 1.f });
	}
}
