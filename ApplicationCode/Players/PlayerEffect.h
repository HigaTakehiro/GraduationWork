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
	/// ������
	/// </summary>
	void Initialize(ID3D12Device* device, Camera* camera);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Vector3 Pos);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
private:
	/// <summary>
	/// �p�[�e�B�N��
	/// </summary>
	void particleCreate();

	Vector3 particlePos{};
	std::unique_ptr <ParticleManager> particle;
};

