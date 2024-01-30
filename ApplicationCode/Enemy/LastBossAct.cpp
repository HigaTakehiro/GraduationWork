#include "LastBossAct.h"

#include <random>

#include "Collision.h"
#include "Helper.h"
#include "LastBoss.h"

void LastBossAct::Move()
{
	if (act_ == Act::DEATH)return;
	//向いた方に移動する
	matRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(Rot_.y));

	move = { 0.f,0.f, 0.1f, 0.0f };
	move = DirectX::XMVector3TransformNormal(move, matRot);


	Rot_.y =Follow() ;

	//WalkAnimation();
	//座標反映(向いた方に)
	Pos_ = {
		Pos_.x += move.m128_f32[0] * 0.8f,
		Pos_.y,
		Pos_.z += move.m128_f32[2] * 0.8f,
	};
	for (size_t i = 0; i < flameSize; i++)
	{
		FlameColor[i] = { 1,1,1,0 };
	}
	std::random_device rnd;
	std::mt19937 mt(rnd());

	constexpr uint32_t ActionInter = 90;
	//攻撃に移行
	if (++actionCount % ActionInter == 0)
	{
		std::uniform_int_distribution<> randact(0, 1);
		if(randact(mt) == 0)
		{

			Vector3 posList1[] = { Vector3(0,-2.5f,-10),Vector3(0,-2.5f,6) };

			Vector3 posList2[] = { Vector3(0,-2.5f,0) ,Vector3(10,-2.5f,0) ,Vector3(-10,-2.5f,0) };

			std::random_device rnd;
			std::mt19937 mt(rnd());
			std::uniform_int_distribution<> rand1(0, 1);
			std::uniform_int_distribution<> rand2(0, 2);

			HolePos[0] = posList1[rand1(mt)];
			HolePos[1] = posList2[rand2(mt)];

			act_ = Act::ATTACK_Hole;
		}
		else
		{
			for (size_t i = 0; i < flameSize; i++)
			{
				FlameScl[i] = { 0.01f,0.01f,0.01f };
				FlameColor[i] = { 1,1,1,1 };
			}
			std::random_device rnd;
			std::mt19937 mt(rnd());
			std::uniform_int_distribution<> rand1(0, 3);
			actionval = rand1(mt);
			act_ = Act::ATTACK_Flame;
		}
		//anime_name_ = AnimeName::ROLE;
	}
	HoleSize[0] = { 0,0,0 };
	HoleSize[1] = { 0,0,0 };

	HoleSmalling = false;
	HoleActivCount = 0;

	for(size_t i=0;i<4;i++)
	

	flameP = INITFLAME;
}

float LastBossAct::Follow()
{
	float pi_180 = 180.f;
	//
	constexpr float AngleCorrVal = 180 /3.14;

	//プレイヤー座標
	DirectX::XMVECTOR posP = { Player_->GetPos().x,Player_->GetPos().y,Player_->GetPos().z };
	//敵座標
	DirectX::XMVECTOR posE = { Pos_.x,Pos_.y,Pos_.z };
	//敵からプレイヤーへのベクトル
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(posP, posE);
	//上のベクトルから回転角求める
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	//追従角度返す
	return angle * AngleCorrVal;
}

void LastBossAct::Attack_Hole()
{
	constexpr float maxSize = 0.1f;
	if (HoleActivCount == 0) {
		for (size_t i = 0; i < holeSize; i++)
		{
			HoleSize[i].x += 0.001f;
			HoleSize[i].y += 0.001f;
			if (HoleSize[i].x >= maxSize)continue;
		}
	}

	if(HoleSize[0].x>=maxSize&& HoleSize[1].x >= maxSize)
	{
		HoleSmalling = true;
	}
	if (HoleSmalling) {
		HoleActivCount++;
		if (HoleActivCount > 120)
		{
			for (size_t i = 0; i < holeSize; i++)
			{
				HoleSize[i].x -= 0.01f;
				HoleSize[i].y -= 0.01f;
				if (HoleSize[i].x <= 0.f)continue;
			}
		}
		if (HoleSmalling&&HoleSize[0].x <= 0.f && HoleSize[1].x <= 0.f)
		{
			act_ = Act::MOVE;
		}
	}

	// Hole当たり判定
	for (size_t i = 0; i < holeSize; i++) {
		if (HoleSize[i].x <= 0.f)continue;
		HoleRange[i] = HoleSize[i].x *20.f ;
		bool judg_Ham = Player_->getisHammerActive() && Collision::HitCircle({ HolePos[i].x, HolePos[i].z + 3.f }, HoleRange[i],
			{ Player_->GetHammmerPos().x,Player_->GetHammmerPos().z }, 1.f);
		bool judg_Player = Collision::HitCircle({ HolePos[i].x, HolePos[i].z + 3.f }, HoleRange[i],
			{ Player_->GetPos().x,Player_->GetPos().z }, 1.f);

		if(judg_Player)
		{
			Player_->SubHP(1);
		}
	}
}

void LastBossAct::Attack_Flame()
{
	
	std::string Vec[] = { "FRONT","BACK","LEFT","RIGHT" };
	Vector3 initPos[] = { Vector3(0,-2.5f,-10),Vector3(0,-2.5f,6) ,Vector3(10,-2.5f,0) ,Vector3(-10,-2.5f,0) };

	if(flameP==INITFLAME){
	if (Vec[actionval] == "FRONT") {
		for (size_t i = 0; i < flameSize; i++) {
			FlameInitPos[i] = { 8.f-(i*4),-2.5f,10.f };
		}
	} else if (Vec[actionval] == "BACK") {
		for (size_t i = 0; i < flameSize; i++) {
			FlameInitPos[i] = { 8.f - (i * 4),-2.5f,-19.f };;
		}
	} else if (Vec[actionval] == "LEFT") {
		for (size_t i = 0; i < flameSize; i++) {
			FlameInitPos[i] = { 10,-2.5f,2.f - (i * 4)};;
		}
	} else if (Vec[actionval] == "RIGHT") {
		for (size_t i = 0; i < flameSize; i++) {
			FlameInitPos[i] = { -10,-2.5f,2.f - (i * 4) };;
		}
	}
	for (size_t i = 0; i < flameSize; i++) {
		FlamePos[i] = FlameInitPos[i];
	}
	ss = Vec[actionval];
	flameP = SHOTFLAME;
}

	if(flameP==SHOTFLAME)
	{
		if (++ShotWaitTime > 50) {
			constexpr float accel = 0.2f;
			if (ss == "FRONT"){
				for (size_t i = 0; i < flameSize; i++){
					FlamePos[i].z -= accel;
				}
			}

			if (ss == "BACK"){
				for (size_t i = 0; i < flameSize; i++){
					FlamePos[i].z += accel;
				}
			}

			if (ss == "LEFT"){
				for (size_t i = 0; i < flameSize; i++){
					FlamePos[i].x -= accel;
				}
			}

			if (ss == "RIGHT"){
				for (size_t i = 0; i < flameSize; i++){
					FlamePos[i].x += accel;
				}
			}
		}
		constexpr float add = 0.0005f;
		for (size_t i = 0; i < flameSize; i++)
		{
			FlameScl[i].x += add;
			FlameScl[i].y += add;
			FlameColor[i].w -= 0.005f;

			FlameScl[i].x = std::clamp(FlameScl[i].x, 0.f, 0.1f);
			FlameScl[i].y = std::clamp(FlameScl[i].y, 0.f, 0.1f);

			bool judg = Collision::HitCircle({ Player_->GetPos().x,Player_->GetPos().z }, 1.f, { FlamePos[i].x,FlamePos[i].z + 3.f }, FlameScl[i].x * 20.f);
			if (judg)
			{
				Player_->SubHP(1);
			}
		}
	}
	if(FlameColor[0].w<=0.f)
	{
		ShotWaitTime = 0;
		act_ = Act::MOVE;
	}

}

void LastBossAct::Attack_Spell()
{
	if (!SpellCancel) {
		RangeScale.x += 0.02f;
		RangeScale.y += 0.02f;
	}
}

void LastBossAct::Transision()
{
	constexpr float inBossroomZ = 12.f;
	if (Rot_.y >= 360.f || Rot_.y <= -360.f)Rot_.y = 0.f;

	if (beginBattle) {//戦闘
		beforeBattle = FALSE;
		(this->*ActionList[act_])();
		//針とプレイヤー衝突
	}
	else
	{
		if(Player_->GetPos().z < inBossroomZ)
		{
			beginBattle = true;
		}
	}
	Act_Barrier();
}
void LastBossAct::Act_Barrier()
{
	for (size_t i = 0; i < barrierSize; i++)
	{
		BarrierAngle[i] += XMConvertToRadians(1.f);
		BarrierPos[i].x = Pos_.x + cosf(BarrierAngle[i] + (i * 90)) * 2.f;
		BarrierPos[i].z = Pos_.z + sinf(BarrierAngle[i] + (i * 90)) * 2.f;
		BarrierPos[i].y = Pos_.y;

		bool judg =BarrierHp[i]>0&&Player_->getisHammerActive() && Collision::HitCircle({ BarrierPos[i].x, BarrierPos[i].z + 3.f }, 1.f,
			{ Player_->GetHammmerPos().x,Player_->GetHammmerPos().z }, 1.f);
		{
			Helper::DamageManager(BarrierHp[i], 1, BarrierDamF[i], BarrierDamCool[i], 60, judg);
			Helper::ColKnock(Player_->GetPos(), { BarrierPos[i].x,BarrierPos[i].y, BarrierPos[i].z + 3.f }, Player_, judg, 1.f);
			if (judg) { Player_->SetIsHammerReflect(true); }
		}
		if(BarrierHp[i]<=0)
		{
			BarrierAlpha[i] -= 0.1f;
		}
		else
		{
			BarrierAlpha[i] += 0.1f;
		}
		BarrierAlpha[i] = std::clamp(BarrierAlpha[i], 0.f, 1.f);
	}

	if(BarrierHp[0]<=0&& BarrierHp[1] <= 0 && BarrierHp[2] <= 0 )
	{
		AllBarrierDestroyF = true;
	}
	if(AllBarrierDestroyF)
	{
		constexpr int tenseconf = 600;
		if(++RefBarrierTime>tenseconf){
			for(size_t i=0;i<barrierSize;i++){
				BarrierHp[i] = 3;
				AllBarrierDestroyF = false;
			}
		}
	}
	else
	{
		RefBarrierTime = 0;
	}
}

void LastBossAct::Death()
{
	
}



void (LastBossAct::* LastBossAct::ActionList[])() =
{
	&LastBossAct::Move,
	&LastBossAct::Attack_Hole,
	&LastBossAct::Attack_Flame,
	&LastBossAct::Attack_Spell,
	& LastBossAct::Act_Barrier,
	& LastBossAct::Death
};



