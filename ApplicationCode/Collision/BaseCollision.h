#pragma once
#include "Object3d.h"

class Object3d;

class BaseCollision
{
public: //メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseCollision() {};
	/// <summary>
	/// 当たり判定チェック
	/// </summary>
	/// <param name="obj1">オブジェクト1</param>
	/// <param name="obj2">オブジェクト2</param>
	virtual bool HitTest(Object3d* obj1, Object3d* obj2) = 0;
};

