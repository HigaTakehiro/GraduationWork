#include "Vector2.h"

using namespace DirectX;

//コンストラクタ
Vector2::Vector2() : Vector2(0, 0)
{

}

Vector2::Vector2(float x, float y) : XMFLOAT2{ x, y }
{

}

Vector2::Vector2(const XMFLOAT2& vec) : XMFLOAT2{ x, y } {
	this->x = vec.x;
	this->y = vec.y;
}

Vector2::Vector2(const XMVECTOR& vec) : XMFLOAT2{ x, y } {
	XMStoreFloat2(this, vec);
}

//長さを求める
float Vector2::length() const
{
	return (float)sqrtf(x * x + y * y);
}

//正規化
Vector2& Vector2::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

//内積を求める
float Vector2::dot(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

//単項演算子オーバーロード
Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

//代入演算子オーバーロード
Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(const Vector2& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

Vector2& Vector2::operator/=(const Vector2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

//2項演算子オーバーロード
const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v)
{
	return v * s;
}

const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}

//線形補間
const Vector2 lerp(const Vector2& start, const Vector2& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

const Vector2 easeIn(const Vector2& start, const Vector2& end, const float t)
{
	float y = t * t;
	return start * (1.0f - y) + end * y;
}

const Vector2 easeOut(const Vector2& start, const Vector2& end, const float t)
{
	float y = t * (2 - t);
	return start * (1.0f - y) + end * y;
}