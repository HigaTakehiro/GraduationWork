#pragma once
#include "Vector3.h"
#include "TextDraw.h"
class Shake
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	void ShakeSet();
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
	//シェイクの揺れる数値(カメラのeyeに代入する)
	float shakePos;
	Vector3 pos{};
	float timer{};
};

