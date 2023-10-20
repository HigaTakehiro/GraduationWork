#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "TextDraw.h"
#include <DirectXMath.h>
#include"ParticleManager.h"

class PlayerEffect
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device, Camera* camera);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Vector3 Pos);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
private:
	/// <summary>
	/// パーティクル
	/// </summary>
	void particleCreate();

	Vector3 particlePos{};
	std::unique_ptr <ParticleManager> particle;
};

