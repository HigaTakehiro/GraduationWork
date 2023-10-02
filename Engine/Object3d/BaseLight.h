#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "DirectXSetting.h"

class BaseLight {
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	BaseLight() = default;
	virtual ~BaseLight() {}

public: //サブクラス
	struct ConstBufferData {
		XMVECTOR lightVec; //ライトへの方向を表すベクトル
		XMFLOAT3 lightColor; //ライトの色
		XMFLOAT3 lightPos;
	};

public: //静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static BaseLight* Create() {};

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 定数バッファへデータ転送
	/// </summary>
	virtual void TransferConstBuffer() = 0;

	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="lightDir">ライト方向</param>
	 virtual void SetLightDir(const XMVECTOR& lightDir) = 0;

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightColor">ライト色</param>
	virtual void SetLightColor(const XMFLOAT3& lightColor) = 0;

	/// <summary>
	/// ライト座標をセット
	/// </summary>
	/// <param name="light"></param>
	virtual void SetLightPos(XMFLOAT3 pos) = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rootParameter">指定ルートパラメータ</param>
	virtual void Draw(UINT rootParameter) = 0;

protected: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;

protected: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//ライト光線方向
	XMVECTOR lightDir = { 1, 0, 0, 0 };
	//ライト色
	XMFLOAT3 lightColor = { 1, 1, 1 };
	//ライト座標	
	XMFLOAT3 lightPos = { 0, 0, 0 };
	//ダーティフラグ
	bool isDirty = false;
};