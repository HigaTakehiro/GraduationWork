#pragma once
#include"Collision.h"
#include"Vector3.h"
#include <algorithm>
using namespace DirectX;

class Helper
{
public:
	Helper() {};
	~Helper() {};
private:
	uint16_t m_KnockCoolT = 0;
public:
	//距離がdistanceより小さいか
	static void isDraw(const Vector3& pos1,const Vector3& pos2,Object3d*obj,float dis = 0.f,bool Flag=false);
	//ノックバックとか
	static void ColKnock(const Vector3& vec1, const Vector3& vec2, Player* obj, bool flag=false);
	//
	static float SmoothStep_Deb(float edge0, float edge1, float x);
	/// <summary>
	/// ダメージ計算
	/// </summary>
	/// <param name="Hp">体力</param>
	/// <param name="subHp">減らす体力</param>
	/// <param name="damF">ダメージフラグ</param>
	/// <param name="coolT">被ダメージのクールタイム</param>
	/// <param name="maxcool">クールタイム最大値</param>
	/// <param name="DamJudg">被ダメージ発生条件</param>
	static void DamageManager(int& Hp, int subHp, BOOL& damF,int& coolT,int maxcool,bool DamJudg=false);
};

//毎シーン書くのだるいので
inline void Helper::isDraw(const Vector3& POS1,const Vector3& POS2,Object3d*obj, float dis,bool Flag)
{
	//描画切る条件
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

inline float Helper::SmoothStep_Deb(float edge0, float edge1, float x)
{
	//edge0からedge1への線形補間
	float t = std::clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
	return t * t * (3.f - 2.f * t);
}

inline void Helper::DamageManager(int& Hp, int subHp, BOOL& damF,int& coolT,int maxcool,bool DamageJudg)
{
	if (Hp <= 0)return;

	//フラグが立ってる時は加算
	if (!damF) {
		//攻撃判定とったｔか
		if (DamageJudg) {
			Hp -= subHp;
			damF = TRUE;
		}
	}

	if (coolT > maxcool)
		damF = FALSE;

	//クールタイム加算するか？
	coolT = damF ? ++coolT : 0;

}