#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <memory>
#include "DirectXSetting.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightGroup
{
public: //エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private: //静的メンバ変数
	static const int DirLightNum = 3;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;
	static const int CircleShadowNum = 1;
	static const int ShadowMapNum = 1;
	static ID3D12Device* device;

public: //サブクラス
	struct ConstBufferData {
		XMFLOAT3 ambientColor;
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		PointLight::ConstBufferData pointLights[PointLightNum];
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		CircleShadow::ConstBufferData circleShadow[CircleShadowNum];
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
	static LightGroup* Create();

	/// <summary>
	/// インスタンス生成(unique_ptr)
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<LightGroup> UniquePtrCreate();

public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rootParameterIndex">指定ルートパラメータ</param>
	void Draw(UINT rootParameterIndex);

	/// <summary>
	/// 定数バッファへデータ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// デフォルトライト設定
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// アンビエント色をセット
	/// </summary>
	/// <param name="ambientColor">アンビエント色</param>
	void SetAmbientColor(const XMFLOAT3& ambientColor);

	/// <summary>
	/// 平行光源有効化
	/// </summary>
	/// <param name="index">平行光源番号</param>
	/// <param name="isActive">平行光源有効化フラグ</param>
	void SetDirLightActive(int index, bool isActive);

	/// <summary>
	/// 平行光源ライト方向をセット
	/// </summary>
	/// <param name="index">平行光源番号</param>
	/// <param name="lightDir">ライト方向</param>
	void SetDirLightDirection(int index, const XMFLOAT3& lightDir);

	/// <summary>
	/// 平行光源ライト色をセット
	/// </summary>
	/// <param name="index">平行光源番号</param>
	/// <param name="lightColor">平行光源色</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// 点光源有効化
	/// </summary>
	/// <param name="index">点光源番号</param>
	/// <param name="active">点光源有効化フラグ</param>
	void SetPointLightActive(int index, bool isActive);

	/// <summary>
	/// 点光源座標をセット
	/// </summary>
	/// <param name="index">点光源番号</param>
	/// <param name="lightPos">点光源座標</param>
	void SetPointLightPos(int index, const XMFLOAT3& lightPos);

	/// <summary>
	/// 点光源色をセット
	/// </summary>
	/// <param name="index">点光源番号</param>
	/// <param name="lightColor">点光源色</param>
	void SetPointLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// 距離減衰係数(点光源用)をセット
	/// </summary>
	/// <param name="index">点光源番号</param>
	/// <param name="lightAtten">距離減衰係数(点光源用)</param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// スポットライト有効化
	/// </summary>
	/// <param name="index">スポットライト番号</param>
	/// <param name="isActive">スポットライト有効化フラグ</param>
	void SetSpotLightActive(int index, bool isActive);

	/// <summary>
	/// スポットライト方向をセット
	/// </summary>
	/// <param name="index">スポットライト番号</param>
	/// <param name="lightDir">スポットライト方向</param>
	void SetSpotLightDirection(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// スポットライト座標をセット
	/// </summary>
	/// <param name="index">スポットライト番号</param>
	/// <param name="lightPos">スポットライト座標</param>
	void SetSpotLightPos(int index, const XMFLOAT3& lightPos);

	/// <summary>
	/// スポットライト色をセット
	/// </summary>
	/// <param name="index">スポットライト番号</param>
	/// <param name="lightColor">スポットライト色</param>
	void SetSpotLightColor(int index, const XMFLOAT3& lightColor);

	/// <summary>
	/// スポットライト距離減衰係数をセット
	/// </summary>
	/// <param name="index">スポットライト番号</param>
	/// <param name="lightAtten">スポットライト距離減衰係数</param>
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// スポットライト減衰角度をセット
	/// </summary>
	/// <param name="index">スポットライト番号</param>
	/// <param name="lightAngle">スポットライト減衰角度</param>
	void SetSpotLightAngle(int index, const XMFLOAT2& lightAngle);

	/// <summary>
	/// 丸影有効化
	/// </summary>
	/// <param name="index">丸影番号</param>
	/// <param name="isActive">有効化フラグ</param>
	void SetCircleShadowActive(int index, bool isActive);

	/// <summary>
	/// 丸影キャスター座標をセット
	/// </summary>
	/// <param name="index">丸影番号</param>
	/// <param name="casterPos">キャスター座標</param>
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos);

	/// <summary>
	/// 丸影方向をセット
	/// </summary>
	/// <param name="index">丸影番号</param>
	/// <param name="lightDir">丸影方向</param>
	void SetCircleShadowDir(int index, const XMVECTOR& lightDir);

	/// <summary>
	/// キャスターとライトの距離をセット(丸影)
	/// </summary>
	/// <param name="index">丸影番号</param>
	/// <param name="distanceCasterLight">キャスターとライトの距離</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);

	/// <summary>
	/// 丸影距離減衰係数をセット
	/// </summary>
	/// <param name="index">丸影番号</param>
	/// <param name="lightAtten">距離減衰係数</param>
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// 丸影角度減衰をセット
	/// </summary>
	/// <param name="index">丸影番号</param>
	/// <param name="angle">角度減衰</param>
	void SetCircleShadowAngle(int index, const XMFLOAT2& angle);

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//環境光
	XMFLOAT3 ambientColor;
	//平行光源配列
	DirectionalLight dirLights[DirLightNum];
	//点光源配列
	PointLight pointLights[PointLightNum];
	//スポットライト配列
	SpotLight spotLights[SpotLightNum];
	//丸影配列
	CircleShadow circleShadows[CircleShadowNum];
	//ダーティフラグ
	bool isDirty;
};

