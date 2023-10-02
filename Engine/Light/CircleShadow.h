#pragma once
#include <DirectXMath.h>

class CircleShadow {
private: //エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	struct ConstBufferData {
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 cosAngle;
		unsigned int isActive;
		float pad4;
	};

public: //メンバ関数
	/// <summary>
	/// 方向をセット
	/// </summary>
	/// <param name="dir">方向</param>
	void SetDir(const XMVECTOR& dir) { this->dir = DirectX::XMVector3Normalize(dir); }

	/// <summary>
	/// キャスター座標をセット
	/// </summary>
	/// <param name="casterPos">障害物</param>
	void SetCasterPos(const XMFLOAT3& casterPos) { this->casterPos = casterPos; }

	/// <summary>
	/// キャスターとライトの距離
	/// </summary>
	/// <param name="distanceCasterLight"></param>
	void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }

	/// <summary>
	/// 距離減衰係数をセット
	/// </summary>
	/// <param name="atten">距離減衰係数</param>
	void SetAtten(const XMFLOAT3& atten) { this->atten = atten; }

	/// <summary>
	/// 減衰角度をセット
	/// </summary>
	/// <param name="cosAngle">減衰角度</param>
	void SetAngle(const XMFLOAT2& cosAngle) {
		this->cosAngle.x = cosf(DirectX::XMConvertToRadians(cosAngle.x));
		this->cosAngle.y = cosf(DirectX::XMConvertToRadians(cosAngle.y));
	}

	/// <summary>
	/// 丸影有効化
	/// </summary>
	/// <param name="isActive">有効化フラグ</param>
	void SetIsActive(bool isActive) { this->isActive = isActive; }

	/// <summary>
	/// 有効化フラグを取得
	/// </summary>
	/// <returns>有効化フラグ</returns>
	bool GetIsActive() { return isActive; }

	/// <summary>
	/// 方向を取得
	/// </summary>
	/// <returns>方向</returns>
	const XMVECTOR& GetDir() { return dir; }

	/// <summary>
	/// キャスター座標を取得
	/// </summary>
	/// <returns>キャスター座標</returns>
	const XMFLOAT3& GetCasterPos() { return casterPos; }

	/// <summary>
	/// キャスターとライトの距離を取得
	/// </summary>
	/// <returns>キャスターとライトの距離</returns>
	float GetDistanceCasterLight() { return distanceCasterLight; }

	/// <summary>
	/// 距離減衰係数を取得
	/// </summary>
	/// <returns>距離減衰係数</returns>
	const XMFLOAT3& GetAtten() { return atten; }

	/// <summary>
	/// 減衰角度を取得
	/// </summary>
	/// <returns>減衰角度</returns>
	const XMFLOAT2& GetAngle() { return cosAngle; }

private: //メンバ変数
	XMVECTOR dir = { 1, 0, 0, 0 };
	float distanceCasterLight = 100.0f;
	XMFLOAT3 casterPos = { 0, 0, 0 };
	XMFLOAT3 atten = { 0.5f, 0.6f, 0.0f };
	XMFLOAT2 cosAngle = { 0.2f, 0.5f };
	bool isActive = false;
};