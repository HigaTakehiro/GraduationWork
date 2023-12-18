#include "TogemaruAct.h"

#include <algorithm>
#include <random>

#include "Easing.h"
#include "Helper.h"
#define PI 3.14
#define PI_180 180
#define PI_360 360

Vector3 TogemaruAct::depositPos = { 10, -2.5f, 0.f };
bool TogemaruAct::depositDelF = false;

void TogemaruAct::Transition()
{
	constexpr float inBossroomZ = 12.f;
	if (Player_->GetPos().z < inBossroomZ)
	{
		beginBattle = TRUE;
	}

	//戦闘開始したら動くように
	if (beginBattle) {
		(this->*ActionList[act_])();
	}

	CollideDeposit();
	//棘が3つ壊れたら
	if (CrushSpear() == TRUE){
		//逃げ惑う
		act_ = Act::RUNAWAY;
	}

	//座標の範囲指定
	Pos_.x = std::clamp(Pos_.x, -10.f, 10.f);
	Pos_.z = std::clamp(Pos_.z, -12.f, 8.8f);
}

//
//
//
void TogemaruAct::ResetParam_Spear()
{
	//
	rushEaseT =0.f;
	ShotRange = 0.f;
	spearsAlpha = 0.f;
	waitShotCount = 0;
	reproductionTime = 0;
	//座標リセット
	for (size_t i = 0; i < spearSize; i++) {
		SpearPos_[i] = Pos_;
	}
}

void TogemaruAct::Move()
{
	anime_name_ = AnimeName::WALK;
	animationWaitTime = 0;

	movSpeed = 0.1f;
	//向いた方に移動する
	move = { 0.f,0.f, 0.1f, 0.0f };
	matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//常に追従
	isFollow = TRUE;

	Rot_.y = isFollow ? Follow() : Walk();

	//座標反映(向いた方に)
	Pos_ = {
		Pos_.x += move.m128_f32[0] * movSpeed,
		Pos_.y,
		Pos_.z += move.m128_f32[2] * movSpeed,
	};

	ResetParam_Spear();

	constexpr uint32_t ActionInter = 60;
	//攻撃に移行
	if(++actionCount%ActionInter==0)
	{
		RushStartPos = Pos_;
		spearsAlpha = 1.f;
		anime_name_ = AnimeName::ROLE;
		act_ = Act::ATTACK_SHOTSPEAR;
	}


}

float TogemaruAct::Walk()
{
	return 0;
}

float TogemaruAct::Follow()
{
	//
	constexpr float AngleCorrVal = 70.f;
	float pi_180 = 180.f;

	//プレイヤー座標
	XMVECTOR posP = { Player_->GetPos().x,Player_->GetPos().y,Player_->GetPos().z };
	//敵座標
	XMVECTOR posE = { Pos_.x,Pos_.y,Pos_.z };
	//敵からプレイヤーへのベクトル
	XMVECTOR Vec = XMVectorSubtract(posE, posP);
	//上のベクトルから回転角求める
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	//追従角度返す
	return angle * AngleCorrVal + pi_180;
}

void TogemaruAct::Attack_Rush()
{
	
}

void TogemaruAct::Attack_ShotSpear()
{
	if (++animationWaitTime < 60)return;

	constexpr float maxRushEaseT = 30.f;

	if (++rushEaseT >= maxRushEaseT) {
		depositCollideF = FALSE;
		isShot = TRUE;//突進終わったら針飛ばす
	}
	else {
		//突進
		Pos_.x = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.x, RushStartPos.x + move.m128_f32[0] * 100.f);
		Pos_.z = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.z, RushStartPos.z + move.m128_f32[2] * 100.f);

		//移動中に鉱石に当たったら
		depositCollideF = TRUE;
	}

	//針飛ばす
	//待ち時間経過したら
	bool canShot = ++waitShotCount > 90;
	//発射終了
	bool endShot =ShotRange>20.f;

	if (isShot) {
		anime_name_ = AnimeName::IdlE;
		if (canShot) {
			spearsAlpha -= 0.02f;//だんだん薄く
			ShotRange += 0.2f;//範囲広げてく
		}
		//発射終了
		if (endShot) {
			isShot = FALSE;//
			act_ = MOVE;//状態を移動に
		}
	}
	//針配置(円状)
	for (size_t i = 0; i < spearSize; i++) {
		//弾け飛ぶ際の回転角
		SpearAngle_[i] = static_cast<float>(i) * (PI_360 / (float)spearSize) + PI_180;

		//座標移動
		SpearPos_[i].x = Pos_.x + sinf(SpearAngle_[i] * (PI / PI_180)) * ShotRange;
		SpearPos_[i].z = Pos_.z + cosf(SpearAngle_[i] * (PI / PI_180)) * ShotRange;
	}

	//アクションのインターバルリセット
	actionCount = 1;

}

bool TogemaruAct::CrushSpear()
{
	//プレイヤー座標
	XMVECTOR posP = { Player_->GetPos().x,Player_->GetPos().y,Player_->GetPos().z };
	//敵座標
	XMVECTOR posE = { Pos_.x,Pos_.y,Pos_.z };
	//敵からプレイヤーへのベクトル
	XMVECTOR Vec = XMVectorSubtract(posE, posP);
	//上のベクトルから回転角求める
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	if(crushSpearNum>=3){
		//プレイヤーの逆向く
		Rot_.y=angle * 70.f;

		return true;
	}

	return false;
}

void TogemaruAct::RunAway()
{
	anime_name_ = AnimeName::CRUSH;

	//棘回復する時間
	constexpr int32_t reproductionMaxTime = 240;

	//向いた方に移動する
	move = { 0.f,0.f, 0.1f, 0.0f };
	matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//座標反映(向いた方に)
	Pos_ = {
		Pos_.x += move.m128_f32[0] * movSpeed,
		Pos_.y,
		Pos_.z += move.m128_f32[2] * movSpeed,
	};

	actionCount = 0;
	//棘復活しきったら移動開始
	if(++reproductionTime>reproductionMaxTime)
	{
		crushSpearNum = 0;
		act_ = Act::MOVE;
	}
}

void TogemaruAct::CollideDeposit()
{
	//判定用の半径 player - boss
	constexpr float r1 = 1.f, r2 = 2.f;

	//鉱石復活するまでは3秒
	constexpr int32_t ReproductionTimeMax = 180;
	if(depositDelF)
	{
		if(++depositDelTime>ReproductionTimeMax)
		{
			//出現
			depositPos = DepositReproduction();
			depositDelF = FALSE;
		}
	}
	else
	{
		//鉱石と衝突したら
		if (depositCollideF&&Helper::GetCircleCollide(depositPos, Pos_, r1, r2)) {
			crushSpearNum=3;//針の数1減らす(壊れた針の数＋１)
			anime_name_ = AnimeName::CRUSH;
			depositDelF = TRUE;
		}
		depositDelTime = 0;
	}
}

Vector3 TogemaruAct::DepositReproduction()
{
	//出す座標は4点 上・下・右・左
	Vector3 posList[] = { Vector3(0,-2.5f,-10),Vector3(0,-2.5f,10) ,Vector3(10,-2.5f,0) ,Vector3(-10,-2.5f,0) };

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0,3);

	//出現する座標はPosListの中からランダム
	return posList[rand(mt)];
}


void TogemaruAct::Death()
{
	
}

void (TogemaruAct::*TogemaruAct::ActionList[])() =
{
	&TogemaruAct::Move,
	&TogemaruAct::Attack_Rush,
	&TogemaruAct::Attack_ShotSpear,
	&TogemaruAct::RunAway,
	&TogemaruAct::Death
};





