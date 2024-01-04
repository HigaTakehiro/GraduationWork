#include "AttackEffect.h"

void AttackEffect::Initialize(ID3D12Device* device, Camera* camera)
{
	particle = ParticleManager::UniquePtrCreate(device, camera, true);
}

void AttackEffect::Update(Vector3 Pos)
{
	particlePos = Pos;
	//if (flag == true) {
		particleCreate();
	//}
	particle->Update();
}

void AttackEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	particle->Draw(cmdList);
}

void AttackEffect::particleCreate()
{
	for (int i = 0; i < 1; i++) {
		// X,Y,Z‘S‚Ä[-5.0f,+5.0f]‚Åƒ‰ƒ“ƒ_ƒ€‚É•ª•z
		const float rnd_pos = 0.0005f;
		Vector3 ppos = particlePos;
		ppos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		ppos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		ppos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.01f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// ’Ç‰Á
		particle->Add(60, ppos, vel, acc, 0.2f, 0.2f, { 1,0.5f,0 }, { 1,0.5f,0 }, 1, 0);
	}
}
