#pragma once
#include"Collision.h"
#include"Vector3.h"
using namespace DirectX;

class Helper
{
public:
	Helper() {};
	~Helper() {};
private:
	uint16_t m_KnockCoolT = 0;
public:
	//‹——£‚ªdistance‚æ‚è¬‚³‚¢‚©
	static void isDraw(const Vector3& pos1,const Vector3& pos2,Object3d*obj,float dis = 0.f,bool Flag=false);

	static void ColKnock(const Vector3& vec1, const Vector3& vec2, Player* obj, bool flag=false);
};

//–ˆƒV[ƒ“‘‚­‚Ì‚¾‚é‚¢‚Ì‚Å
inline void Helper::isDraw(const Vector3& POS1,const Vector3& POS2,Object3d*obj, float dis,bool Flag)
{
	//•`‰æØ‚éðŒ
	auto isDraw = [dis](Vector3 pos1, Vector3 pos2)->
		bool {if (Collision::GetLength(pos1, pos2) < dis)return true; return false; };

	if (Flag)return;
	if (!isDraw(POS1, POS2))return;

	obj->Draw();
}

inline void Helper::ColKnock(const Vector3& vec1, const Vector3& vec2, Player*obj,bool flag) 
{
	Vector3 vec;
	vec = vec1 - vec2;
	vec.normalize();
	vec.y = 0.0f;
	
	if(flag)
	{
		obj->HitHammerToEnemy(vec,0.5f);
	}

}

