#pragma once
#include "Object3d.h"
#include "Vector3.h"
#include "DirectXMath.h"
#include"WinApp.h"
#include"CollisionPrimitive.h"
class Collision final
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	Collision() = default;
	~Collision() = default;
	Collision(const Collision& obj) = delete;
	Collision& operator=(const Collision& obj) = delete;

public: //静的メンバ関数
	/// <summary>
	/// インスタンス取得用関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Collision* GetIns();

public: //メンバ関数
	static float GetLength(XMFLOAT3 position, XMFLOAT3 position2);

	static bool HitCircle(XMFLOAT2 pos1, float radius1, XMFLOAT2 pos2, float radius2);

	static bool HitBox(XMFLOAT2 box, int boxSize, XMFLOAT2 point, int pointSize = 0);
	static bool HitBox(XMFLOAT2 box,int boxSize, XMFLOAT2 point, XMFLOAT2 size);

	static XMFLOAT2 HitCrossWindows(XMFLOAT2 enemyPos,int flag);
	
	static bool CrossLine(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT2 pos3, XMFLOAT2 pos4);

	static XMFLOAT2 CrossLinePoint(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT2 pos3, XMFLOAT2 pos4);
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="object1">判定したいオブジェクト1</param>
	/// <param name="object2">判定したいオブジェクト2</param>
	/// <param name="objScale1">判定したいオブジェクトの半径1</param>
	/// <param name="objScale2">判定したいオブジェクトの半径2</param>
	/// <returns>当たった</returns>
	bool OBJSphereCollision(Object3d* object1, Object3d* object2, float objScale1, float objScale2);

	static bool OBBCollision(OBB& obbA, OBB& obbB);

	static double LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3=0);

private: //メンバ変数
	Vector3 object1Pos = { 0, 0, 0 };
	Vector3 object2Pos = { 0, 0, 0 };

	float objectPosTotal = 0;
	float objectScaleTotal = 0;
};

