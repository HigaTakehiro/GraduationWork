#include "TogemaruAct.h"

#include <algorithm>

#include "Easing.h"
#define PI 3.14
#define PI_180 180
#define PI_360 360

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
		act_ = Act::ATTACK_SHOTSPEAR;
	}

	//棘が3つ壊れたら
	if(CrushSpear()==TRUE)
	{
		//逃げ惑う
		act_ = Act::RUNAWAY;
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
	constexpr float maxRushEaseT = 30.f;

	if (++rushEaseT >= maxRushEaseT) {
		isShot = TRUE;//突進終わったら針飛ばす
	}
	else {
			//突進
			Pos_.x = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.x, RushStartPos.x + move.m128_f32[0] * 100.f);
			Pos_.z = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.z, RushStartPos.z + move.m128_f32[2] * 100.f);
	}

	//針飛ばす
	//待ち時間経過したら
	bool canShot = ++waitShotCount > 90;
	//発射終了
	bool endShot =ShotRange>20.f;

	if (isShot) {
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
	//棘回復する時間
	constexpr int32_t reproductionMaxTime = 180;

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





