#pragma once
#include <DirectXMath.h>
#include <cmath>

class Vector2 : public DirectX::XMFLOAT2
{
public:
	//コンストラクタ
	Vector2();
	Vector2(float x, float y);
	Vector2(const DirectX::XMFLOAT2& vec);
	Vector2(const DirectX::XMVECTOR& vec);

	//メンバ関数
	float length() const;
	Vector2& normalize();
	float dot(const Vector2& v) const;

	//単項演算子オーバーロード
	Vector2 operator+() const;
	Vector2 operator-() const;

	//代入演算子オーバーロード
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator/=(const Vector2& v);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);
};

//2項演算子オーバーロード
const Vector2 operator+(const Vector2& v1, const Vector2& v2);
const Vector2 operator-(const Vector2& v1, const Vector2& v2);
const Vector2 operator*(const Vector2& v, float s);
const Vector2 operator*(float s, const Vector2& v);
const Vector2 operator/(const Vector2& v, float s);

//補間関数
//線形補間(1次関数補間)
const Vector2 lerp(const Vector2& start, const Vector2& end, const float t);

//線形補間(イーズイン)
const Vector2 easeIn(const Vector2& start, const Vector2& end, const float t);

//線形補間(イーズアウト)
const Vector2 easeOut(const Vector2& start, const Vector2& end, const float t);