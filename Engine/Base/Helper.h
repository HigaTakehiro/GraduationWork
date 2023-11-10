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
	//距離がdistanceより小さいか
	static bool GetLengthisSmallerDist(Vector3 pos1, Vector3 pos2, float dis = 0.f);

};

//毎シーン書くのだるいので
inline bool Helper::GetLengthisSmallerDist(Vector3 POS1, Vector3 POS2, float dis)
{
	//描画切る条件
	auto isDraw = [dis](Vector3 pos1, Vector3 pos2)->
		bool {if (Collision::GetLength(pos1, pos2) < dis)return true; return false; };

	return isDraw(POS1, POS2);
}
