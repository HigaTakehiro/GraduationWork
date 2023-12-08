#include "MunniAction.h"

#include "Easing.h"

void MunniAction::Move()
{
	//向いた方に移動する
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rot_.y));

	move = XMVector3TransformNormal(move, matRot);

	//回転のY軸は関数の戻り値(angle)で判定
	Rot_.y = isFollow ? Follow() : Walk();

	if(isFollow)
	{
		oldRotY = Rot_.y;
		randAddRotVal = rand() % 60 + 60;
		BeginWalkCount = 0;
		rotEaseT = 0.f;
	}

	if (BeginWalkCount > 0) {
		//座標反映
		Pos_ = {
			Pos_.x += move.m128_f32[0] * movSpeed,
			Pos_.y,
			Pos_.z += move.m128_f32[2] * movSpeed,
		};
	}
}

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

void MunniAction::Attack()
{
	
}

void MunniAction::Death()
{
	
}



