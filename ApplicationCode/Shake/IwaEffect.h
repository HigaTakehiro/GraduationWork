#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "TextDraw.h"
#include <DirectXMath.h>
#include"ParticleManager.h"
class IwaEffect
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/// <summary>
	/// èâä˙âª
	/// </summary>
	void Initialize(ID3D12Device* device, Camera* camera);
	/// <summary>
	/// çXêVèàóù
	/// </summary>
	void Update(Vector2 particle2dPos);
	void Draw(ID3D12GraphicsCommandList* cmdList);
private:
	void ConvertParticlePos(Vector2 particle2dPos);

	void particleCreate();
	Vector3 particlePos{};
	std::unique_ptr <ParticleManager> particle;
};

