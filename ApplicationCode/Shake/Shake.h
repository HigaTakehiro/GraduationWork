#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "TextDraw.h"
#include "IwaEffect.h"
class Shake
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* device, Camera* camera);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	void ShakeSet();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// シェイク時の揺れる数値の取得
	/// </summary>
	/// <returns></returns>
	float GetShakePos() { return shakePos; }
	float GetShakeTimer() { return shakeTimer; }
	bool GetShakeFlag() { return shakeFlag; }
private:
	//シェイク時の最大時間
	float shakeMaxTimer;

	float shakeTimer;
	bool shakeFlag;
	bool iwaFlag;
	float iwaCount;
	Vector2 iwaPos[2];
	//シェイクの揺れる数値(カメラのeyeに代入する)
	float shakePos;
	Vector3 pos{};
	float timer{};
	IwaEffect* iwa[2];
};

