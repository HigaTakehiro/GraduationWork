#include "Collision.h"

Collision* Collision::GetIns()
{
	static Collision instance;
	return &instance;
}

float Collision::GetLength(XMFLOAT3 position, XMFLOAT3 position2)
{
	float len;
	len = sqrtf(
		(position.x - position2.x) * (position.x - position2.x) + (position.y - position2.y) * (position.y - position2.
			y) + (position.z - position2.z) * (position.z - position2.z));
	return len;
}

bool Collision::HitCircle(XMFLOAT2 pos1, float radius1, XMFLOAT2 pos2, float radius2)
{
	float temp = sqrtf(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));

	return temp < radius1 + radius2;
}

bool Collision::HitBox(XMFLOAT2 box, int boxSize, XMFLOAT2 point, int pointSize)
{
	bool flag = 0;
	if (box.x - boxSize < point.x + pointSize&&
		box.x + boxSize > point.x - pointSize)
	{
		if (box.y - boxSize < point.y + pointSize &&
			box.y + boxSize > point.y - pointSize)
		{
			flag = true;
		}
	}


	return flag;
}

bool Collision::HitBox(XMFLOAT2 box, int boxSize, XMFLOAT2 point, XMFLOAT2 size)
{
	bool flag = 0;
	if (box.x - boxSize < point.x + size.x &&
		box.x + boxSize > point.x - size.x)
	{
		if (box.y - boxSize < point.y + size.y &&
			box.y + boxSize > point.y - size.y)
		{
			flag = true;
		}
	}


	return flag;
}


DirectX::XMFLOAT2 Collision::HitCrossWindows(XMFLOAT2 enemyPos, int flag)
{
	
	XMFLOAT2 XminYmin = { 0,0 };
	XMFLOAT2 XmaxYmin = { WinApp::window_width,0 };
	XMFLOAT2 XminYmax = { 0, WinApp::window_height };
	XMFLOAT2 XmaxYmax = { WinApp::window_width,WinApp::window_height };

	XMFLOAT2 center = { XmaxYmax.x / 2,XmaxYmax.y / 2 };

	if (flag == 0)
	{
		//XminYmin&&XmaxYminの場合
		//XminYmin		(x2,y2)
		//XmaxYmin		(x4,y4)
		//center		(x1,y1)
		//enemyPos		(x3,y3)
		bool isHit = CrossLine(center, XminYmin, enemyPos, XmaxYmin);

		if (isHit)
		{
			XMFLOAT2 temp = CrossLinePoint(center, XminYmin,
				enemyPos, XmaxYmin);
			return temp;
		}

	}
	else if (flag == 1)
	{
		//XmaxYmin&&XmaxYmaxの場合
		//XmaxYmin		(x2,y2)
		//XmaxYmax		(x4,y4)
		//center		(x1,y1)
		//enemyPos		(x3,y3)
		bool isHit = CrossLine(center, XmaxYmin, enemyPos, XmaxYmax);

		if (isHit)
		{
			XMFLOAT2 temp = CrossLinePoint(center, XmaxYmin,
				enemyPos, XmaxYmax);
			return temp;
		}
	}
	else if (flag == 2)
	{
		//XmaxYmax&&XminYmaxの場合
		//XmaxYmax		(x2,y2)
		//XminYmax		(x4,y4)
		//center		(x1,y1)
		//enemyPos		(x3,y3)
		bool isHit = CrossLine(center, XmaxYmax, enemyPos, XminYmax);

		if (isHit)
		{
			XMFLOAT2 temp = CrossLinePoint(center, XmaxYmax,
				enemyPos, XminYmax);
			return temp;
		}
	}
	else if (flag == 3)
	{
		//XminYmax&&XminYminの場合
		//XminYmax		(x2,y2)
		//XminYmin		(x4,y4)
		//center		(x1,y1)
		//enemyPos		(x3,y3)
		bool isHit = CrossLine(center, XminYmax, enemyPos, XminYmin);

		if (isHit)
		{
			XMFLOAT2 temp = CrossLinePoint(center, XminYmax,
				enemyPos, XminYmin);
			return temp;
		}
	}
	else
	{
		
		return { 5000.f, 5000.f };
	}
}

bool Collision::CrossLine(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT2 pos3, XMFLOAT2 pos4)
{
	//頂点A(pos1) - 頂点B(pos3)
	//頂点C(pos2) - 頂点D(pos4)

	bool ABFlag;
	bool cdFlag;
	float s;
	float t;

	s = ((pos3.x - pos1.x) * (pos2.y - pos1.y))
		- ((pos2.x - pos1.x) * (pos3.y - pos1.y));
	t = ((pos3.x - pos1.x) * (pos4.y - pos1.y))
		- ((pos4.x - pos1.x) * (pos3.y - pos1.y));
	if (s * t > 0)
	{
		ABFlag = false;
	}
	else
	{
		ABFlag = true;
	}
	s = ((pos2.x - pos4.x) * (pos1.y - pos2.y))
		- ((pos2.y - pos4.y) * (pos1.x - pos2.x));
	t = ((pos2.x - pos4.x) * (pos3.y - pos2.y))
		- ((pos2.y - pos4.y) * (pos3.x - pos2.x));
	if (s * t > 0)
	{
		cdFlag = false;
	}
	else
	{
		cdFlag = true;
	}

	if (ABFlag == true && cdFlag == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

DirectX::XMFLOAT2 Collision::CrossLinePoint(XMFLOAT2 pos1, XMFLOAT2 pos2, XMFLOAT2 pos3, XMFLOAT2 pos4)
{
	//頂点A(pos1) - 頂点B(pos3)
	//頂点C(pos2) - 頂点D(pos4)
	//とした時、交点(x, y)は、
	//s₁ = { (x₄ - x₂) * (y₁ - y₂) - (y₄ - y₂) * (x₁ - x₂) } / 2.0
	//s₂ = { (x₄ - x₂) * (y₂ - y₃) - (y₄ - y₂) * (x₂ - x₃) } / 2.0
	//x = x₁ + (x₃ - x₁) * s₁ / (s₁ + s₂)
	//y = y₁ + (y₃ - y₁) * s₁ / (s₁ + s₂)

	float s1 = (
		(pos4.x - pos2.x) * (pos1.y - pos2.y) -
		(pos4.y - pos2.y) * (pos1.x - pos2.x)) / 2.f;

	float s2 = (
		(pos4.x - pos2.x) * (pos3.y - pos2.y) -
		(pos4.y - pos2.y) * (pos3.x - pos2.x)) / 2.f;

	XMFLOAT2 temp{};
	temp.x = pos1.x + (pos3.x - pos1.x) * s1 / (s1 + s2);
	temp.y = pos1.y + (pos3.y - pos1.y) * s1 / (s1 + s2);

	return temp;
}

bool Collision::OBJSphereCollision(Object3d* object1, Object3d* object2, float objScale1, float objScale2) {
	object1Pos = object1->GetMatWorld().r[3];

	object2Pos = object2->GetMatWorld().r[3];

	objectPosTotal = (object2Pos.x - object1Pos.x) * (object2Pos.x - object1Pos.x) + (object2Pos.y - object1Pos.y) * (object2Pos.y - object1Pos.y) + (object2Pos.z - object1Pos.z) * (object2Pos.z - object1Pos.z);
	objectScaleTotal = (objScale1 + objScale2) * (objScale1 + objScale2);

	if (!(objectPosTotal <= objectScaleTotal)) return false;

	return true;
}

//OBBの当たり判定
bool Collision::OBBCollision(OBB& obbA, OBB& obbB) {
	//中身がないやつ
	XMVECTOR not = { 0,0,0 };
	// 各方向ベクトルの確保
  // （N***:標準化方向ベクトル）
	XMVECTOR NAe1 = obbA.GetDirect(0), Ae1 = NAe1 * obbA.GetLen_W(0);
	XMVECTOR NAe2 = obbA.GetDirect(1), Ae2 = NAe2 * obbA.GetLen_W(1);
	XMVECTOR NAe3 = obbA.GetDirect(2), Ae3 = NAe3 * obbA.GetLen_W(2);
	XMVECTOR NBe1 = obbB.GetDirect(0), Be1 = NBe1 * obbB.GetLen_W(0);
	XMVECTOR NBe2 = obbB.GetDirect(1), Be2 = NBe2 * obbB.GetLen_W(1);
	XMVECTOR NBe3 = obbB.GetDirect(2), Be3 = NBe3 * obbB.GetLen_W(2);
	XMVECTOR Interval = obbA.GetPos_W() - obbB.GetPos_W();

	// 分離軸 : Ae1
	double rA = sqrt(
		(Ae1.m128_f32[0] * Ae1.m128_f32[0]) + (Ae1.m128_f32[1] * Ae1.m128_f32[1]) + (Ae1.m128_f32[2] * Ae1.m128_f32[
			2]));
	double rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	double fL = fabs(
		(Interval.m128_f32[0] * NAe1.m128_f32[0]) + (Interval.m128_f32[1] * NAe1.m128_f32[1]) + (Interval.m128_f32[2] *
			NAe1.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : Ae2
	rA = sqrt(
		(Ae2.m128_f32[0] * Ae2.m128_f32[0]) + (Ae2.m128_f32[1] * Ae2.m128_f32[1]) + (Ae2.m128_f32[2] * Ae2.m128_f32[
			2]));
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * NAe2.m128_f32[0]) + (Interval.m128_f32[1] * NAe2.m128_f32[1]) + (Interval.m128_f32[2] *
			NAe2.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	rA = sqrt(
		(Ae3.m128_f32[0] * Ae3.m128_f32[0]) + (Ae3.m128_f32[1] * Ae3.m128_f32[1]) + (Ae3.m128_f32[2] * Ae3.m128_f32[
			2]));
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * NAe3.m128_f32[0]) + (Interval.m128_f32[1] * NAe3.m128_f32[1]) + (Interval.m128_f32[2] *
			NAe3.m128_f32[2]));

	// 分離軸 : Ae3
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = sqrt(
		(Be1.m128_f32[0] * Be1.m128_f32[0]) + (Be1.m128_f32[1] * Be1.m128_f32[1]) + (Be1.m128_f32[2] * Be1.m128_f32[
			2]));
	fL = fabs(
		(Interval.m128_f32[0] * NBe1.m128_f32[0]) + (Interval.m128_f32[1] * NBe1.m128_f32[1]) + (Interval.m128_f32[2] *
			NBe1.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = sqrt(
		(Be2.m128_f32[0] * Be2.m128_f32[0]) + (Be2.m128_f32[1] * Be2.m128_f32[1]) + (Be2.m128_f32[2] * Be2.m128_f32[
			2]));
	fL = fabs(
		(Interval.m128_f32[0] * NBe2.m128_f32[0]) + (Interval.m128_f32[1] * NBe2.m128_f32[1]) + (Interval.m128_f32[2] *
			NBe2.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = sqrt(
		(Be3.m128_f32[0] * Be3.m128_f32[0]) + (Be3.m128_f32[1] * Be3.m128_f32[1]) + (Be3.m128_f32[2] * Be3.m128_f32[
			2]));
	fL = fabs(
		(Interval.m128_f32[0] * NBe3.m128_f32[0]) + (Interval.m128_f32[1] * NBe3.m128_f32[1]) + (Interval.m128_f32[2] *
			NBe3.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C11
	XMVECTOR Cross;
	Cross = XMVector3Cross(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C12
	Cross = XMVector3Cross(NAe1, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C13
	Cross = XMVector3Cross(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C21
	Cross = XMVector3Cross(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C22
	Cross = XMVector3Cross(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C23
	Cross = XMVector3Cross(NAe2, NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C31
	Cross = XMVector3Cross(NAe3, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C32
	Cross = XMVector3Cross(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	//L = XMVector3Dot(Interval, Cross);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// 分離軸 : C33
	Cross = XMVector3Cross(NAe3, NBe3);

	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	//L = XMVector3Dot(Interval, Cross);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}

	// 分離平面が存在しないので「衝突している」
	return true;
}

// 分離軸に投影された軸成分から投影線分長を算出
double Collision::LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	double fr1 = fabs(
		(Sep->m128_f32[0] * e1->m128_f32[0]) + (Sep->m128_f32[1] * e1->m128_f32[1]) + (Sep->m128_f32[2] * e1->m128_f32[
			2]));
	double fr2 = fabs(
		(Sep->m128_f32[0] * e2->m128_f32[0]) + (Sep->m128_f32[1] * e2->m128_f32[1]) + (Sep->m128_f32[2] * e2->m128_f32[
			2]));
	double fr3 = e3
		? (fabs(
			(Sep->m128_f32[0] * e3->m128_f32[0]) + (Sep->m128_f32[1] * e3->m128_f32[1]) + (Sep->m128_f32[2]
				* e3->m128_f32[2])))
		: 0;
	return fr1 + fr2 + fr3;
}


