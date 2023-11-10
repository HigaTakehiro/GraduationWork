#pragma once
#include"Collision.h"
#include"Vector3.h"
using namespace DirectX;

class Helper
{
public:
	Helper() {};
	~Helper() {};
public:
	//‹——£‚ªdistance‚æ‚è¬‚³‚¢‚©
	static void isDraw(const Vector3& pos1,const Vector3& pos2,Object3d*obj,float dis = 0.f,bool Flag=false);

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
