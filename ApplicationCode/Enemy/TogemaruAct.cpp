#include "TogemaruAct.h"
#include "TogemaruAct.h"

#include <algorithm>
#include <random>

#include "Easing.h"
#include "Helper.h"
#include "PadInput.h"
#include "SoundManager.h"
#include "DamageMath.h"
#define PI 3.14
#define PI_180 180
#define PI_360 360

Vector3 TogemaruAct::depositPos = { 0, -2.5f, -8.f };
bool TogemaruAct::depositDelF = false;

Vector3 TogemaruAct::depositPos2 = { 5, -2.5f, -8.f };
bool TogemaruAct::depositDelF2 = false;
Vector3 TogemaruAct::cameraPos = {};
Vector3 TogemaruAct::oldCameraPos = {};
Vector3 TogemaruAct::DefaultPos= {};

bool TogemaruAct::TogemaruDeathF = false;
TogemaruAct::AppearState TogemaruAct::StateArray[TogemaruAct::StateName::P_Num] =
{
	{TogemaruAct::P1,&TogemaruAct::Phase1},
	{TogemaruAct::P2,&TogemaruAct::Phase2},
	{TogemaruAct::P3,&TogemaruAct::Phase3}
};

void TogemaruAct::StateExecute(int state)
{
	int index = 0;

	//stateに応じた関数の実行
	for(int i=0;i<StateName::P_Num;i++){
		if(StateArray[i].state==state)
		{
			(this->*StateArray[i].func)();
			return;
		}
	}
}

void TogemaruAct::Phase1()
{
	depositPos = { 0, -2.5f, -8.f };
	depositDelF = false;
	depositPos2 = { 5, -2.5f, -8.f };
	depositDelF2 = false;
	

	TogemaruDeathF = false;
	//イージング用{カウンタ上限,Y座標(上),Y座標(下)}
	float EaseState_Pos[3] = {40.f, 20.f, -2.5f};

	anime_name_ = AnimeName::ROLE;
	depositCollideF = TRUE;
	//落ちてくる
	PosEaseT++;
	Pos_.y=Easing::easeIn(PosEaseT, EaseState_Pos[0], EaseState_Pos[1], EaseState_Pos[2]);

	Pos_.x = 0.f;
	Pos_.z = -8.f;

	DeathSmallF = false;
	for (size_t i = 0; i < spearSize; i++) {
		spearsAlpha[i] = 0.f;//だんだん薄く
	}
	shakeT = 0.f;
	//地面ついたら次のフェーズ
	if(PosEaseT>=EaseState_Pos[0]){
		PosEaseT = 0.f;
		cameraStateIndex = StateName::P2;
	}
}

void TogemaruAct::Phase2()
{
	constexpr float MaxWaitTime = 90.f;
	//
	PosEaseT++;
	if(PosEaseT>MaxWaitTime)
	{
		PosEaseT = 0.f;
		anime_name_ = AnimeName::IdlE;
		cameraStateIndex = StateName::P3;
	}

	//anime_name_ = AnimeName::IdlE;
}

void TogemaruAct::Phase3()
{
	//anime_name_ = AnimeName::IdlE;
	//Pos_ = {};
}

void TogemaruAct::Transition()
{
	constexpr float inBossroomZ = 12.f;
	if (Rot_.y >= 360.f || Rot_.y <= -360.f)Rot_.y = 0.f;

	if (!beginBattle&&Player_->GetPos().z < inBossroomZ) {
		beforeBattle = TRUE;
	}

	if(beforeBattle)
	{
		//プレイヤーの動き止める
		Player_->SetStopF(true);
		//待機モーション実行
		//StateExecute(cameraStateIndex);
		(this->*StateArray[cameraStateIndex].func)();

		if( cameraStateIndex==StateName::P3){
			//動けるように　戦闘開始
			Player_->SetStopF(false);
			beginBattle = TRUE;
		}
	}
	else
	{
		if(!beginBattle)
		Pos_.y = 20.f;
	}


	//戦闘開始したら動くように
	if (beginBattle) {//戦闘
		beforeBattle = FALSE;
		(this->*ActionList[act_])();
		//針とプレイヤー衝突
		if (CollideSpear()) {
			if (!Player_->GetIsHammerRelease() && PadInput::GetIns()->PushButton(PadInput::Button_B))
				Player_->SetIsHammerRelease(true);
			Player_->SubHP(DamageMath::ReturnDamage(atk, Player_->GetDef()));
		}

		//棘が3つ壊れたら
		if (CrushSpear() == TRUE) {
			//逃げ惑う
			act_ = Act::RUNAWAY;
		}
		if(Hp<=0)
		{
			act_ = Act::DEATH;
		}
		else {
			//衝突半径
			constexpr float pr = 1.f, er = 1.5f;
			//衝突時のプレイヤーのけぞり
			bool AccelJudg = (rushEaseT >= 0.2f && rushEaseT <= 0.9f) && (act_ == Act::ATTACK_SHOTSPEAR);
			float KnockDis = AccelJudg ? 0.3f : 0.6f;//のけぞり値

			//判定
			bool animenotwalk = anime_name_ != AnimeName::WALK_FRONT && anime_name_ != AnimeName::WALK_BACK &&
				anime_name_ != AnimeName::WALK_LEFT && anime_name_ != AnimeName::WALK_RIGHT;
			bool isCollide =crushSpearNum<3&&Hp>0&& animenotwalk && Helper::GetCircleCollide(Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z + 3.f }, pr, er);
			if (!damf && isCollide) { Player_->SubHP(DamageMath::ReturnDamage(atk, Player_->GetDef())); damf = true; }
			if (damf) { damcool++; if (damcool > 90)damf = false; } else { damcool = 0; }
			Helper::ColKnock(Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z + 3.f }, Player_, isCollide, KnockDis);
		}
	}

	if(anime_name_==ROLE )
	{
		RoleF = true;
	}
	else { RoleF = false; }
	//鉱石当たり判定
	CollideDeposit();
	CollideDeposit2();
	//鉱石衝突時の画面の揺れ
	ViewShake();


	//座標の範囲指定
	Pos_.x = std::clamp(Pos_.x, -10.f, 10.f);
	Pos_.z = std::clamp(Pos_.z, -10.f, 6.8f);
}

//
//
//
void TogemaruAct::ResetParam_Spear()
{
	rushEaseT = 0.f;
	ShotRange = 0.f;
	spearsAlpha = { 0.f };
	waitShotCount = 0;
	reproductionTime = 0;
	//座標リセット
	for (size_t i = 0; i < spearSize; i++) {
		SpearPos_[i] = Pos_;
	}
}

void TogemaruAct::WalkAnimation()
{
	float NowRotAnime = (Rot_.y);

	if(NowRotAnime<-35)
	{
		anime_name_ = AnimeName::WALK_LEFT;
	}
	else if (NowRotAnime < 45)
	{
		anime_name_ = AnimeName::WALK_FRONT;
	} else if (NowRotAnime < 135)
	{
		anime_name_ = AnimeName::WALK_RIGHT;
	} else if (NowRotAnime < 225)
	{
		anime_name_ = AnimeName::WALK_BACK;
	} else if (NowRotAnime < 315)
	{
		anime_name_ = AnimeName::WALK_LEFT;
	} else
	{
		anime_name_ = AnimeName::WALK_FRONT;
	}
}


void TogemaruAct::CrushAnimation()
{
	float NowRotAnime = (Rot_.y);

	if (NowRotAnime < -35)
	{
		anime_name_ = AnimeName::CRUSH_LEFT;
	} else if (NowRotAnime < 45)
	{
		anime_name_ = AnimeName::CRUSH_FRONT;
	} else if (NowRotAnime < 135)
	{
		anime_name_ = AnimeName::CRUSH_RIGHT;
	} else if (NowRotAnime < 225)
	{
		anime_name_ = AnimeName::CRUSH_BACK;
	} else if (NowRotAnime < 315)
	{
		anime_name_ = AnimeName::CRUSH_LEFT;
	} else
	{
		anime_name_ = AnimeName::CRUSH_FRONT;
	}
}
void TogemaruAct::Move()
{

	if (act_ == Act::DEATH)return;
	animationWaitTime = 0;

	movSpeed = 0.6f;
	//向いた方に移動する
	move = { 0.f,0.f, 0.1f, 0.0f };
	matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	for (size_t i = 0; i < spearSize; i++) {
		spearsAlpha[i] = 0.f;//だんだん薄く
	}
	//常に追従
	isFollow = TRUE;

	Rot_.y = isFollow ? Follow() : Walk();

	WalkAnimation();
	//座標反映(向いた方に)
	Pos_ = {
		Pos_.x += move.m128_f32[0] * movSpeed,
		Pos_.y,
		Pos_.z += move.m128_f32[2] * movSpeed,
	};

	ResetParam_Spear();
	std::random_device rnd;
	std::mt19937 mt(rnd());

	constexpr uint32_t ActionInter = 90;
	//攻撃に移行
	if (++actionCount % ActionInter == 0)
	{
		RushStartPos = Pos_;
	
		std::uniform_int_distribution<> randact(0, 1);
		if(randact(mt)==0)
		{
			act_ = Act::ATTACK_SHOTSPEAR;
		}
		else
		{
			std::uniform_int_distribution<> rand1(0, 20);
			std::uniform_int_distribution<> rand2(0, 10);

			constexpr int PosSize = 6;


			for (size_t i = 0; i < PosSize; i++)
			{
				std::uniform_int_distribution<> randInt(1, 2);
				if (randInt(mt) == 1)RandM_P = -1.f;
				else RandM_P = 1.f;

				CurreRandVal[i] = RandM_P;
			}
			Vector3 posList[] = {
				Vector3((rand1(mt)) * RandM_P,-2.5f,rand2(mt) * RandM_P),
				Vector3(rand1(mt) * -RandM_P,-2.5f,rand2(mt) * -RandM_P) ,
				Vector3((rand1(mt)) * RandM_P,-2.5f,rand2(mt) * RandM_P) ,
				Vector3(rand1(mt) * -RandM_P,-2.5f,rand2(mt) * -0.2f),
				Vector3((rand1(mt)) * -0.2,-2.5f,rand2(mt) * -RandM_P),
				Vector3(rand1(mt) * RandM_P,-2.5f,rand2(mt) * RandM_P)
			};

			if (spline == nullptr) {
				for (size_t i = 0; i < _countof(posList); i++) {
					SplinePosList.emplace_back(posList[i]);
				}
				spline = new Spline();
				spline->Init(SplinePosList, static_cast<int>(SplinePosList.size()));
			}
			act_ = Act::ATTACK_RUSH;
		}

		anime_name_ = AnimeName::ROLE;
		
	}
	splineT = 0;
	BefoSplinePos = Pos_;
	SplineAfterPos = {};


}

float TogemaruAct::Walk()
{
	return Rot_.y;
}

float TogemaruAct::Follow()
{
	float pi_180 = 180.f;
	//
	constexpr float AngleCorrVal = PI_180 / PI;

	//プレイヤー座標
	XMVECTOR posP = { Player_->GetPos().x,Player_->GetPos().y,Player_->GetPos().z };
	//敵座標
	XMVECTOR posE = { Pos_.x,Pos_.y,Pos_.z };
	//敵からプレイヤーへのベクトル
	XMVECTOR Vec = XMVectorSubtract(posP, posE);
	//上のベクトルから回転角求める
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	//追従角度返す
	return angle * AngleCorrVal;
}

void TogemaruAct::Attack_Rush()
{
	if (act_ == Act::DEATH)return;


	splineT++;
	
	if (splineT > 60) {
		if (spline->GetIndex() >= SplinePosList.size()-2)
		{
			//RoleF = false;
			anime_name_ = AnimeName::IdlE;
			act_ = Act::MOVE;
			if (spline)
				delete spline;
			spline = nullptr;
			SplinePosList.clear();
		} else
		{
			//RoleF = true;
			anime_name_ = AnimeName::ROLE;
			spline->Upda(Pos_);
		}
	}
	else
	{
		
		spline->Upda(SplineAfterPos);
		Pos_.x = Easing::easeIn(splineT, 60, BefoSplinePos.x, SplineAfterPos.x);
		Pos_.z = Easing::easeIn(splineT, 60, BefoSplinePos.z, SplineAfterPos.z);
		
	}
}

void TogemaruAct::Attack_ShotSpear()
{
	if (act_ == Act::DEATH)return;
	if (++animationWaitTime < 60)return;

	//RoleF = true;

	constexpr float maxRushEaseT = 30.f;

	if (++rushEaseT >= maxRushEaseT) {
		depositCollideF = FALSE;
		depositCollideF2 = FALSE;
		for (size_t i = 0; i < spearSize; i++) {
			spearsRot[i] = (i * (360 / spearSize));
			if(i%2!=0)
			{
				spearsRot[i] += 90;
			}
			if(i==0||i==7)
			{
				spearsRot[i] += 180;
			}
			if(i==4||i==3)
			{
				spearsRot[i] += 180;;
			}
			spearsAlpha[i] = 1.f;
		}
	//	spearsRot[1] = (i * (360 / spearSize));
		//spearsRot[3] = (i * (360 / spearSize));
		isShot = TRUE;//突進終わったら針飛ばす
	} else {
		//突進
		Pos_.x = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.x, RushStartPos.x + move.m128_f32[0] * 100.f);
		Pos_.z = Easing::easeIn(rushEaseT, maxRushEaseT, RushStartPos.z, RushStartPos.z + move.m128_f32[2] * 100.f);

		//移動中に鉱石に当たったら
		depositCollideF = TRUE;

		depositCollideF2 = TRUE;
	}

	//針飛ばす
	//待ち時間経過したら
	bool canShot = ++waitShotCount > 90;
	//発射終了
	bool endShot = ShotRange > 20.f;

	if (isShot) {
		anime_name_ = AnimeName::IdlE;
		if (canShot) {
			for (size_t i = 0; i < spearSize; i++) {
			//	spearsAlpha[i] -= 0.02f;//だんだん薄く
			}
			ShotRange += 0.2f;//範囲広げてく
			if(ShotRange<=0.2f)
			{
				SoundManager::GetIns()->PlaySE(SoundManager::SEKey::nidle, 0.2f);
			}
		}
		//発射終了
		if (endShot) {
			isShot = FALSE;//
			//RoleF = false;
			for (size_t i = 0; i < spearSize; i++) {
				spearsAlpha[i] = 0.f;//だんだん薄く
			}
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
	XMVECTOR Vec = XMVectorSubtract(posP, posE);
	//上のベクトルから回転角求める
	float angle = atan2f(Vec.m128_f32[0], Vec.m128_f32[2]);

	if (crushSpearNum >= 3) {
		//プレイヤーの逆向く
		Rot_.y = angle * 60.f + 180.f;

		return true;
	}

	return false;
}

void TogemaruAct::RunAway()
{

	if (act_ == Act::DEATH)return;
	CrushAnimation();
	//棘回復する時間
	constexpr int32_t reproductionMaxTime = 360;

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
	if (++reproductionTime > reproductionMaxTime)
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
	if (depositDelF)
	{
		if (++depositDelTime > ReproductionTimeMax)
		{
			//出現
			depositPos = DepositReproduction();
			depositDelF = FALSE;
		}
	} else
	{
		constexpr float GroundY = 2.f;
		bool col = Helper::GetCircleCollide({ depositPos.x,depositPos.y,depositPos.z + 3.f }, { Pos_.x,Pos_.y,Pos_.z + 3.f }, r1, r2);
		//鉱石と衝突したら
		if (depositCollideF &&(Pos_.y<GroundY&&col)) {
			shakeF = TRUE;//画面揺らす
			//針の数1減らす(壊れた針の数＋１)
			if (beginBattle) {
				RoleF = false;
				crushSpearNum=3;
			}
			CrushAnimation();
			depositDelF = TRUE;
		}
		depositDelTime = 0;
	}
}


void TogemaruAct::CollideDeposit2()
{
	//判定用の半径 player - boss
	constexpr float r1 = 1.f, r2 = 2.f;

	//鉱石復活するまでは3秒
	constexpr int32_t ReproductionTimeMax = 180;
	if (depositDelF2)
	{
		if (++depositDelTime2 > ReproductionTimeMax)
		{
			//出現
			depositPos2= DepositReproduction();
			depositDelF2 = FALSE;
		}
	} else
	{
		constexpr float GroundY = 2.f;
		bool col = Helper::GetCircleCollide({ depositPos2.x,depositPos2.y,depositPos2.z + 3.f }, { Pos_.x,Pos_.y,Pos_.z + 3.f }, r1, r2);
		//鉱石と衝突したら
		if (depositCollideF2 && (Pos_.y < GroundY && col)) {
			shakeF = TRUE;//画面揺らす
			//針の数1減らす(壊れた針の数＋１)
			if (beginBattle) {
			//	RoleF = false;
				crushSpearNum=3;
			}
			CrushAnimation();
			depositDelF2 = TRUE;
		}
		depositDelTime2 = 0;
	}
}

bool TogemaruAct::CollideSpear()
{
	//発射時以外判定取らない
	if (ShotRange <= 0)return false;

	for (size_t i = 0; i < spearSize; i++)
	{
		if (spearsAlpha[i] <= 0.f)continue;
		if (Helper::GetCircleCollide(Player_->GetPos(), { SpearPos_[i].x,SpearPos_[i].y,SpearPos_[i].z + 3.f }
			, 1.f, 0.5f))
		{
			spearsAlpha[i] = 0.f;//当たったら描画消す
			return true;
		}
	}
	return false;
}

Vector3 TogemaruAct::DepositReproduction()
{
	//出す座標は4点 上・下・右・左
	Vector3 posList[] = { Vector3(0,-2.5f,-10),Vector3(0,-2.5f,6) ,Vector3(10,-2.5f,0) ,Vector3(-10,-2.5f,0) };

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 3);

	//出現する座標はPosListの中からランダム
	return posList[rand(mt)];
}

Vector3 TogemaruAct::DepositReproduction2()
{
	//出す座標は4点 上・下・右・左
	Vector3 posList[] = { Vector3(5,-2.5f,-10),Vector3(-5,-2.5f,6) ,Vector3(10,-2.5f,5) ,Vector3(-10,-2.5f,-5) };

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand(0, 3);

	//出現する座標はPosListの中からランダム
	return posList[rand(mt)];
}

void TogemaruAct::ViewShake()
{
	//シェイク周期
	constexpr float movInter = 2.f;
	//シェイク幅
	constexpr float shakeVibration = 10.f;

	if (shakeF) {
		shakeT++;
		shakeXVal = sinf(PI * 2 / movInter * shakeT) * shakeVibration;
		shakeYVal = sinf(PI * 2 / movInter * shakeT) * shakeVibration;
		if(shakeT>20.f){
			shakeXVal = DefaultPos.x - oldCameraPos.x;
			shakeYVal = DefaultPos.y - oldCameraPos.y;
			shakeF = FALSE;
		}
		//シェイク値を加算
		//o12  d14 -2  //n10 d15  +5
	}
	else{
		shakeT = 0.f;
		shakeXVal = shakeYVal = 0.f;
	}
	cameraPos.x = shakeXVal;
	cameraPos.y = shakeYVal;
}

bool TogemaruAct::Appear()
{
	//Pos_ = Vector3(0, 0, -10);
	return true;
}



void TogemaruAct::Death()
{
	constexpr float shakeVibration = 3.f;
	anime_name_ = AnimeName::ROLE;
	dshakeT++;
	dshakeXVal = sinf(PI * 2 / 2 * dshakeT) * shakeVibration;

	if (dshakeT > 90) {
		dshakeXVal = 0;
		dshakeYVal = 0;
		DeathSmallF = true;
	}
	else
	{
		constexpr float radians = PI / PI_180;
		constexpr float shakeInter = 2.f;

		//Pos_.x += sin(dshakeT * radians) / shakeInter;
	}
	if (DeathSmallF) {
		Scl_.x -= 0.001f;
		Scl_.y -= 0.001f;

		dshakeT = 0;

	}
	if (Scl_.x <= 0.f) {
		TogemaruDeathF = true;
	}
	Scl_.x = std::clamp(Scl_.x, 0.f, 1.f);
	Scl_.y = std::clamp(Scl_.y, 0.f, 1.f);
}

void (TogemaruAct::* TogemaruAct::ActionList[])() =
{
	
	&TogemaruAct::Move,
	&TogemaruAct::Attack_Rush,
	&TogemaruAct::Attack_ShotSpear,
	&TogemaruAct::RunAway,
	&TogemaruAct::Death
};






void TogemaruAct::RushGround360()
{
	spline->Upda(Pos_);
	
	anime_name_ = AnimeName::ROLE;
	if(spline->GetIndex()>=4)
	{
		spline->Init(SplinePosList, SplinePosList.size());
		act_ = Act::MOVE;
	}
}
