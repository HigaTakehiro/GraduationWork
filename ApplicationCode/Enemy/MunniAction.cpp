#include "MunniAction.h"

#include <algorithm>
#include"Collision.h"
#include "Easing.h"
#include "Shapes.h"
#include"Helper.h"

MunniAction::MunniAction()
{
	ImpTexInit();
}

MunniAction* MunniAction::GetIns()
{
	static MunniAction ins;
	return &ins;
}

//
// テクスチャ初期化
//
void MunniAction::ImpTexInit()
{
	Model*model= Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "impact.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 0.f, 2.0f }, { 64.0f, 64.0f });
	Model*model2= Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "Area.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 0.f, 2.0f }, { 64.0f, 64.0f });

	ImpactTex = Object3d::UniquePtrCreate(model);
	ImpactTex->SetColType(Object3d::CollisionType::Obb);
	ImpactTex->SetObjType((int32_t)Object3d::OBJType::Enemy);
	ImpactTex->SetObbScl({ 1.f,1.f,1.f });
	ImpactTex->SetHitRadius(0.5f);

	ImpactAreaTex = Object3d::UniquePtrCreate(model2);
	ImpactAreaTex->SetColType(Object3d::CollisionType::Obb);
	ImpactAreaTex->SetObjType((int32_t)Object3d::OBJType::Enemy);
	ImpactAreaTex->SetObbScl({ 1.f,1.f,1.f });
	ImpactAreaTex->SetHitRadius(0.5f);

	impTexScl = Vector3(0, 0, 0);
}


//
// 移動
//
void MunniAction::Move()
{
	XMFLOAT2 Pos_P = { m_Player_->GetPos().x,m_Player_->GetPos().z };//Player
	XMFLOAT2 Pos_E = { Pos_.x,Pos_.z+3.f };//Enemy

	if(Collision::HitCircle(Pos_E,1.f,Pos_P,3.f)&&!isSearch)
	{
		isFollow = !isFollow;//フラグ切り替え
		isSearch = TRUE;//追跡しま
	}

	constexpr float pr = 1.f, er = 1.2f;
	bool isCollide = Helper::GetCircleCollide(m_Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z+3.f}, pr, er);
	Helper::ColKnock(m_Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z}, m_Player_, isCollide, 0.8f);

	//追跡時間
	FollowCount = isSearch ? ++FollowCount : 0;

	//追跡終了の条件
	bool FolSuspension = FollowCount > 180;
	if(FolSuspension)
	{
		isSearch = FALSE;
		isFollow = isSearch;
	}

	//向いた方に移動する
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//回転のY軸は関数の戻り値(angle)で判定
	Rot_.y = isFollow ? Follow() : Walk();

	if(isFollow)
	{
		//一定範囲内＋30フレーム近くいたら攻撃する
		constexpr uint32_t nearPlayerCountMax = 30;
		if (Collision::HitCircle(Pos_E, 1.f, Pos_P, 2.f)) {
			if (++atckCoolTime >nearPlayerCountMax ){
				isAtck = TRUE;
				list_ = List::ATTACK;
			}
		}

		//各パラメータリセットなど
		oldRotY = Rot_.y;//Y軸のRot保持
		randAddRotVal = rand() % 60 + 60;//上の回転値に加える値
		BeginWalkCount = 0;//あるき中のカウントリセット
		rotEaseT = 0.f;//イージングカウント
	}

	//歩ける条件満たした＋追跡状態の時
	if (BeginWalkCount > 0||isFollow) {
		//座標反映
		Pos_ = {
			Pos_.x += move.m128_f32[0] * movSpeed,
			Pos_.y,
			Pos_.z += move.m128_f32[2] * movSpeed,
		};
	}

	// 攻撃に使うパラメータリセット
	m_JumpInter_ = 0;
	atckTimer_ = 0;
}

//
// 移動(徘徊)
//
float MunniAction::Walk()
{
	//カウンタ上限
	constexpr float CountMax = 90;
	constexpr uint32_t WalkEndTime=120;

	//待機終了 あるき始める
	if(rotEaseT>=CountMax)
	{
		oldRotY = Rot_.y;
		randAddRotVal = rand() % 60+60;

		if(++BeginWalkCount>WalkEndTime)
		{
			rotEaseT = 0.f;
		}
	}
	//待機中 向きかえる
	else
	{
		BeginWalkCount = 0;
		rotEaseT++;
		return Easing::easeIn(rotEaseT, 60.f, oldRotY, oldRotY + (float)randAddRotVal);
	}
	return Rot_.y;
}

//
// 移動(追従)
//
float MunniAction::Follow()
{
	constexpr float AngleCorrVal = 70.f;
	float pi_180 = 180.f;
	//角度の取得 
	XMVECTOR PositionA = { m_Player_->GetPos().x,m_Player_->GetPos().y,m_Player_->GetPos().z };
	XMVECTOR PositionB = { Pos_.x, Pos_.y, Pos_.z };

	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(PositionB, PositionA); // positionA - positionB;

	float Angle;
	Angle = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	return Angle * AngleCorrVal + pi_180;
}

//
// 攻撃
//
void MunniAction::Attack()
{
	//待ち時間
	constexpr uint32_t waitTime = 90;
	//ジャンプ
	constexpr uint32_t next_Phase = 30;
	//ジャンプの着地時
	bool onGround = m_JumpInter_ >= 1.f;

	atckTimer_ = isAtck ? ++atckTimer_ : 0;

	if (atckTimer_ > waitTime) {//少し待ったら
		Jump();//飛ぶ
		if(onGround) {//地面着地後は
			isTexScling = TRUE;//衝撃波出す
			ImpactTex->SetPosition(Vector3(Pos_.x, InitPos.y+0.1f, Pos_.z));//テクスチャ座標(InitPosは地面用)
			isAtck = false;//攻撃フラグ切る
			list_ = List::MOVE;//移動に戻る
		}
		else
		{
			ImpactAreaTex->SetPosition(Vector3(Pos_.x, InitPos.y + 0.1f, Pos_.z));
			impAreaalpha += 0.02f;
		}
		shakeCount = 0.f;
	}
	//ぷるぷる震える
	else
	{
		shakeCount++;
	}
	//震わすのはsin波
	Pos_.x = Pos_.x + (sinf(shakeCount) / 30.f);
	//
	isFollow = isSearch = FALSE;
	atckCoolTime = 0;
}

//
// 死に
//
void MunniAction::Death()
{
	
}

//
// 攻撃(Jump)
//
void MunniAction::Jump()
{
	float SubPower = 0.001f;
	//落下の緩急
	constexpr float Distortion = 3.f;
	//地面の高さ
	float GroundY = InitPos.y;
	//ジャンプ高さ
	constexpr float Height = 4.6f;
	/* 変換 */
	constexpr float pi = 3.14f;
	//ドッスン挙動

	m_JumpInter_ += 1.f / 60.f;
	Pos_.y = GroundY + (1.0f - pow(1.0f - sin(pi * m_JumpInter_), Distortion)) * Height;

	m_JumpInter_ = std::clamp(m_JumpInter_, 0.f, 1.f);
}

//
// テクスチャ更新
//
//
void MunniAction::ImpTexUpda()
{
	//加算値,減算値
	constexpr float SubTexAlphaVal = 0.05f;//alpha
	constexpr float AddTexScl = 0.005f;//scl

	if (isTexScling)
	{
		constexpr float pr = 1.f, er = 2.5f;

		bool isCollide = Helper::GetCircleCollide(m_Player_->GetPos(), Pos_, pr, er);

		Helper::ColKnock(m_Player_->GetPos(), { Pos_.x,Pos_.y,Pos_.z }, m_Player_, isCollide, 1.f);
		if (isCollide) {
			m_Player_->SubHP(1);
		}
		// 拡大
		impTexScl = Vector3(impTexScl.x + AddTexScl,
			impTexScl.y + AddTexScl,
			impTexScl.z
		);
		// アルファ値減算
		impTexAlpha -= SubTexAlphaVal;

		if (impTexAlpha <= 0.f) {
			isTexScling = FALSE;
		}
		impAreaalpha -= 0.05f;
	} else
	{
		impTexScl = Vector3(0, 0, 0);
		impTexAlpha = 1.f;
	}

	//ジャンプの着地時に出る衝撃波
	ImpactTex->SetScale(impTexScl);
	ImpactTex->SetRotation(Vector3(90, 0, 0));
	ImpactTex->SetColor({ 1,1,1,impTexAlpha });
	ImpactTex->Update();

	//範囲
	ImpactAreaTex->SetScale({0.05f,0.05f,1.f});
	ImpactAreaTex->SetRotation(Vector3(90, 0, 0));
	ImpactAreaTex->SetColor({ 1,1,1,impAreaalpha });
	ImpactAreaTex->Update();

	impAreaalpha = std::clamp(impAreaalpha, 0.f, 0.3f);
	impTexAlpha = std::clamp(impTexAlpha, 0.f, 1.f);
}

// 行動遷移
//
void MunniAction::AttackTransition()
{
	(this->*ActionList[list_])();

	ImpTexUpda();
}

//
// 関数ポインタ(行動用の)
//
void (MunniAction::* MunniAction::ActionList[])() = {
	&MunniAction::Move,
	&MunniAction::Attack,
	&MunniAction::Death
};

//
// 衝撃波のテクスチャ描画
//
void MunniAction::ImpTexDraw()
{
	ImpactAreaTex->Draw();
	ImpactTex->Draw();
}

void MunniAction::IdleShake(bool shakestop)
{
	shakeCount++;

	Pos_.x = Pos_.x + (sinf(shakeCount)/30.f);
}
