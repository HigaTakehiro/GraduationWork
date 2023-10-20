#include "IwaEffect.h"
#include "KeyInput.h"
using namespace DirectX;

void IwaEffect::Initialize(ID3D12Device* device, Camera* camera)
{
	particle = ParticleManager::UniquePtrCreate(device, camera, true);
	particlePos = { 0,0 ,0 };
}

void IwaEffect::Update(Vector2 particle2dPos, float fade)
{
	ConvertParticlePos(particle2dPos);
	particleCreate(fade);
	particle->Update();
}

void IwaEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	particle->Draw(cmdList);
}

void IwaEffect::ConvertParticlePos(Vector2 particle2dPos)
{
	//ビューポート行列
	XMMATRIX mvp = XMMatrixIdentity();
	mvp.r[0].m128_f32[0] = 1280.0f / 2.0f;
	mvp.r[1].m128_f32[1] = -720.0f / 2.0f;
	mvp.r[3].m128_f32[0] = 1280.0f / 2.0f;
	mvp.r[3].m128_f32[1] = 720.0f / 2.0f;
	//ビュープロジェクションビューポート合成行列
	XMMATRIX mvpv = particle->GetMat() * mvp;
	//上記の行列の逆行列
	XMMATRIX mvpvInv = XMMatrixInverse(nullptr, mvpv);
	//スクリーン座標
	Vector3 posNear = Vector3(particle2dPos.x, particle2dPos.y, 0);
	Vector3 posFar = Vector3(particle2dPos.x, particle2dPos.y, 1);
	XMVECTOR posNearV = XMLoadFloat3(&posNear);
	XMVECTOR posFarV = XMLoadFloat3(&posFar);
	//スクリーン座標系からワールド座標系へ
	posNearV = XMVector3TransformCoord(posNearV, mvpvInv);//座標に行列をかけてwを除算
	posFarV = XMVector3TransformCoord(posFarV, mvpvInv);
	//レイの方向
	XMVECTOR direction = posFarV - posNearV;
	//ベクトルの正規化
	direction = XMVector3Normalize(direction);
	const float distance = 0.0f;

	particlePos.x = posNearV.m128_f32[0] - direction.m128_f32[0] * distance;
	particlePos.y = posNearV.m128_f32[1] - direction.m128_f32[1] * distance;
	particlePos.z = posNearV.m128_f32[2] - direction.m128_f32[2] * distance;
}

void IwaEffect::particleCreate(float fade)
{
	for (int i = 0; i < 1; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
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

		// 追加
		particle->Add(60, ppos, vel, acc, 0.002f, 0.0f, { 0,0,0 }, { 0,0,0 }, fade, 0);
	}
}
