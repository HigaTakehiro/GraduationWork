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