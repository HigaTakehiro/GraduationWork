#include "PlayerEffect.h"

void PlayerEffect::Initialize(ID3D12Device* device, Camera* camera)
{
	particle = ParticleManager::UniquePtrCreate(device, camera, false);
}

void PlayerEffect::Update(Vector3 Pos)
{
	Pos = particlePos;
	particleCreate();
	particle->Update();
}

void PlayerEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	particle->Draw(cmdList);
}

void PlayerEffect::particleCreate()
{
	for (int i = 0; i < 1; i++) {
		// X,Y,Z‘S‚Ä[-5.0f,+5.0f]‚Åƒ‰ƒ“ƒ_ƒ€‚É•ª•z
		const float rnd_pos = 0.05f;
		Vector3 ppos = particlePos;
		ppos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		ppos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		ppos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.0001f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.0001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// ’Ç‰Á
		particle->Add(60, ppos, vel, acc, 0.002f, 0.0f, { 1,0,0 }, { 1,0,0 }, 1, 0);
	}
}
