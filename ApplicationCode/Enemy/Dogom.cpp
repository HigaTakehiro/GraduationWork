#include "Dogom.h"

#include <dinput.h>

#include "Collision.h"
#include"Easing.h"
#include "ImageManager.h"
#include "KeyInput.h"

#include "Shapes.h"

void Dogom::Init()
{
	m_Name = DOGOM;


	//BodyModel_.reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	//BodyModel_->CreateTexture();

	for (int32_t i = 0; i < 2; i++) {
		ArmModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 64.f, 64.0f }, "Hammer.png");
	}

	/*本体モデル設定*/
	m_Body.reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
	m_Body->CreateTexture();
	//m_Body->SetColType(Object3d::CollisionType::Sphere);
	//m_Body->SetObjType((int32_t)Object3d::OBJType::Enemy);
	//m_Body->SetHitRadius(0.5f);
	m_Body->SetAnchorPoint({ 0.5f,1.f });
	m_BodyPos = { 0,2,10 };
	m_BodyRot.x = 180;
	/*腕モデル設定*/
	for (size_t i = 0; i < 2; i++) {
		m_Arm[i].reset(Texture::Create(ImageManager::GetIns()->USA_1, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		m_Arm[i]->CreateTexture();
		m_Arm[i]->SetAnchorPoint({ 0.5f,0.5f });
		m_Arm[i]->SetRotation({ 180,0,0 });
		//m_Arm[i]->SetIsBillboardY(true);
		//m_Arm[i]->SetColType(Object3d::CollisionType::Sphere);
		//m_Arm[i]->SetObjType((int32_t)Object3d::OBJType::Enemy);
		//m_Arm[i]->SetHitRadius(0.5f);
		
	}
	
	m_ArmPos[RIGHT] = Vector3(m_BodyPos.x + 2.f, m_BodyPos.y - 0.5f, m_BodyPos.z);
	m_ArmPos[LEFT] = Vector3(m_BodyPos.x -2.f, m_BodyPos.y -0.5f, m_BodyPos.z);
}

void Dogom::Upda()
{
	m_ActionTimer++;
	//if(m_ActionTimer%120)
	Follow();
	Wince();
	if (!WinceF)WinceEaseT = 0;
	RecvDamage(m_BodyPos);
	m_Body->SetRotation({ m_BodyRot.x,m_BodyRot.y,m_BodyRot.z });
	m_Body->SetScale({ 1.f,1.f,1.f });
	m_Body->SetBillboard(FALSE);
	m_Body->SetPosition(m_BodyPos);
	m_Body->Update(m_Camera);

	ArmAct();
	for (size_t i = 0; i < 2; i++) 
	{
		m_Arm[i]->SetScale({ 1.f,1.0f,1.0f });
		m_Arm[i]->SetPosition(m_ArmPos[i]);
		m_Arm[i]->SetBillboard(TRUE);
		m_Arm[i]->Update(m_Camera);
	}

}

void Dogom::Draw()
{
	
	
	
}
void Dogom::Draw2()
{
	if (m_HP <= 0)return;
	Texture::PreDraw();
m_Body->Draw();
	for (size_t i = 0; i < 2; i++)
		m_Arm[i]->Draw();
Texture::PostDraw();
}


void Dogom::Attack()
{

}

void Dogom::Finalize()
{
	m_Body.reset(nullptr);
	m_Arm[RIGHT].reset(nullptr);
	m_Arm[LEFT].reset(nullptr);
}

#define PI 3.14
void Dogom::ArmAct()
{
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f }, move2 = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot_R = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y-90.f));
		XMMATRIX matRot_L = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y +90.f));

		move = XMVector3TransformNormal(move, matRot_R);
		move2 = XMVector3TransformNormal(move2, matRot_L);

		float Left_X = m_BodyPos.x + move.m128_f32[0] * 20.f, Left_Z = m_BodyPos.z + move.m128_f32[2] *20.f;

		float Right_X = m_BodyPos.x + move2.m128_f32[0] * 20.f, Right_Z = m_BodyPos.z + move2.m128_f32[2] * 20.f;


	//通常
	if (arm_move_ == DEFAULT) {
		constexpr float GroundY = 1.5f;
		//上下周期
		constexpr float MovingInter = 240.f;
		//振れ幅
		constexpr float MovingVel = 0.80f;

		m_ArmMov_Y[LEFT]++;
		m_ArmMov_Y[RIGHT] = m_ArmMov_Y[LEFT] / 1.2f;

		m_ArmPos[LEFT].x =Left_X;
		m_ArmPos[LEFT].z = Left_Z;

		m_ArmPos[RIGHT].x = Right_X;
		m_ArmPos[RIGHT].z = Right_Z;

		for (size_t i = 0; i < 2; i++) {
			m_ArmPos[i].y = GroundY + sinf(PI * 2.f / MovingInter * m_ArmMov_Y[i]) * MovingVel;
			BefoPos[i] = m_ArmPos[i];
		}
		PlayerPos = m_player->GetPos();

		if(KeyInput::GetIns()->TriggerKey(DIK_SPACE))
		{
			/*{
				if (!WinceF) {
					StanCount = 0;
					WinceF = TRUE;
					wince_phase_ = WincePhase::PHASE_1;
				}
			}*/
				SetAttack_Impact();

			arm_move_ = ATTACK_IMPACT;
		}
	}

	//叩きつけ
	else if(arm_move_==ATTACK_IMPACT)
	{
		//追加時間(腕戻す)
		constexpr float MaxTime_1 = 50.f;
		constexpr float MaxTime_2 = 30.f;
		constexpr float MaxTime_3 = 30.f;

		bool next_2=FALSE,next_3=FALSE;

		switch (phase_)
		{
		case Phase_Impact::PHASE_1:
			ResetArmParam();

			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];

			BefoEaseT = m_ArmAttckEaseT[LEFT];

			if (m_ArmAttckEaseT[LEFT] >=MaxTime_1) {
				
				for (size_t i = 0; i < 2; i++) {
					BefoPos[i] = m_ArmPos[i];
				}
				m_ArmAttckEaseT[LEFT] = m_ArmAttckEaseT[RIGHT] = 0;
				next_2 = TRUE;
			}
			else {
				m_ArmPos[LEFT].y = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_1, BefoPos[LEFT].y, m_BodyPos.y + 3.f);
				m_ArmPos[RIGHT].y = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_1, BefoPos[RIGHT].y, m_BodyPos.y + 3.f);
			}

				if(next_2)
					phase_ = Phase_Impact::PHASE_2;
			break;

		case Phase_Impact::PHASE_2:
			//カウンタを右と左でずらす
			if (m_ArmAttckEaseT[LEFT] >= MaxTime_2)
				m_ArmAttckEaseT[RIGHT]++;
			else
				m_ArmAttckEaseT[LEFT]++;


			if (m_ArmAttckEaseT[RIGHT] >= MaxTime_2) {
				BefoEaseT = m_ArmAttckEaseT[LEFT];
				m_ArmAttckEaseT[LEFT] = m_ArmAttckEaseT[RIGHT] = 0;
				next_3 = TRUE;
			}
			else{
				m_ArmPos[LEFT].y = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_2, BefoPos[LEFT].y, 1.f);
				m_ArmPos[RIGHT].y = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_2, BefoPos[RIGHT].y, 1.f);
			}

			if(next_3)
				phase_ = Phase_Impact::PHASE_3;
			break;

		case Phase_Impact::PHASE_3:

			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];
			//腕の位置地面
			BefoPos[0].y = BefoPos[1].y =  1.f;

			//ボスの横に
			m_ArmPos[LEFT].y = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_3, BefoPos[LEFT].y, m_BodyPos.y - 0.5f);
			m_ArmPos[RIGHT].y = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_3, BefoPos[RIGHT].y, m_BodyPos.y - 0.5f);

			if (m_ArmAttckEaseT[LEFT] >= MaxTime_3)
				phase_ = Phase_Impact::END;

			break;

		case Phase_Impact::END:
			m_ArmAttckEaseT[LEFT] = m_ArmAttckEaseT[RIGHT]=0.f;
			next_2 = FALSE, next_3 = FALSE;
			arm_move_ = DEFAULT;
			break;
		}

	}

	else if(arm_move_==ATTACK_CROSS)
	{
		/*XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_player->GetRot().y));

		move = XMVector3TransformNormal(move, matRot);*/
		bool next_1=FALSE, next_2=FALSE;
		float EndX[2] = { PlayerPos.x+ 5.f ,PlayerPos.x - 5.f };
		float EndZ[2] = { PlayerPos.z ,PlayerPos.z };
		switch (phase_cross_)
		{
		case Phase_Cross::PHASE_1:
			ResetArmParam();
			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];

			//腕座標をプレイヤー位置に
			for (size_t i = 0; i < 2; i++) {
				m_ArmPos[i].x = Easing::easeIn(m_ArmAttckEaseT[i], 60.f, BefoPos[i].x, EndX[i]);
				m_ArmPos[i].y = Easing::easeIn(m_ArmAttckEaseT[i], 60.f, BefoPos[i].y, m_BodyPos.y);
				m_ArmPos[i].z = Easing::easeIn(m_ArmAttckEaseT[i], 60.f, BefoPos[i].z, EndZ[i]);
			}

			if (m_ArmAttckEaseT[LEFT] >= 60.f)
			{
				m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT] = 0.f;
				BefoPos[LEFT] = m_ArmPos[LEFT], BefoPos[RIGHT] = m_ArmPos[RIGHT];
				next_1 = TRUE;
			}

			if (next_1)phase_cross_ = Phase_Cross::PHASE_2;
			break;

		case Phase_Cross::PHASE_2:
			
			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];

			for(size_t i=0;i<2;i++)
			m_ArmPos[i].x = Easing::easeIn(m_ArmAttckEaseT[i], 30.f, BefoPos[i].x, PlayerPos.x);

			if (m_ArmAttckEaseT[LEFT] >= 30.f) {
				m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT] = 0.f;
				BefoPos[LEFT] = m_ArmPos[LEFT], BefoPos[RIGHT] = m_ArmPos[RIGHT];
				next_2 = TRUE;
			}
			if (next_2)phase_cross_ = Phase_Cross::PHASE_3;
			break;

		case Phase_Cross::PHASE_3:
			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];
		
			//ボスの横に
			m_ArmPos[LEFT].x = Easing::easeIn(m_ArmAttckEaseT[LEFT], 50.f, BefoPos[LEFT].x, Left_X);
			m_ArmPos[RIGHT].x = Easing::easeIn(m_ArmAttckEaseT[RIGHT], 50.f, BefoPos[RIGHT].x, Right_X);

			m_ArmPos[LEFT].y = Easing::easeIn(m_ArmAttckEaseT[LEFT], 50.f, BefoPos[LEFT].y, m_BodyPos.y - 0.5f);
			m_ArmPos[RIGHT].y = Easing::easeIn(m_ArmAttckEaseT[RIGHT], 50.f, BefoPos[RIGHT].y, m_BodyPos.y - 0.5f);

			m_ArmPos[LEFT].z = Easing::easeIn(m_ArmAttckEaseT[LEFT], 50.f, BefoPos[LEFT].z, Left_Z);
			m_ArmPos[RIGHT].z = Easing::easeIn(m_ArmAttckEaseT[RIGHT], 50.f, BefoPos[RIGHT].z, Right_Z);
			
			if (m_ArmAttckEaseT[LEFT] >= 50.f)phase_cross_ = Phase_Cross::END;
			break;

		case Phase_Cross::END:
			m_ArmAttckEaseT[LEFT] = m_ArmAttckEaseT[RIGHT] = 0.f;
			next_1 = FALSE, next_2 = FALSE;
			arm_move_ = DEFAULT;
			break;
		}
	}

}

void Dogom::Follow()
{
	if (WinceF)return;
	float Len = Collision::GetLength(m_player->GetPos(), m_BodyPos);
	bool notFollow = Len < 1.f;
	
	XMVECTOR posA = { m_player->GetPos().x,m_player->GetPos().y,m_player->GetPos().z };
	XMVECTOR posB = { m_BodyPos.x,m_BodyPos.y,m_BodyPos.z };

	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVec = XMVectorSubtract(posB, posA); // positionA - positionB;

	float toPlayer;
	toPlayer = atan2f(SubVec.m128_f32[0], SubVec.m128_f32[2]);

	m_BodyRot.y = toPlayer * 70.f + 180.f;

	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y + 0.f));

	move = XMVector3TransformNormal(move, matRot);

	if (Len > 2.f) {
		m_BodyPos.x += move.m128_f32[0] * 1.1f;

		m_BodyPos.z += move.m128_f32[2] * 1.1f;

	}
}

void Dogom::Wince()
{
	if (!WinceF)return;
	constexpr float MaxEase = 30.f;
	if (wince_phase_ == WincePhase::PHASE_1) {
		if (++WinceEaseT >= MaxEase) {
			WinceEaseT = 0.f;
			wince_phase_ = WincePhase::PHASE_2;
		} else
			m_BodyRot.x = Easing::easeIn(WinceEaseT, MaxEase, 180.f, 160.f);
	}
	else if(wince_phase_==WincePhase::PHASE_2)
	{
		m_BodyRot.x = Easing::easeIn(WinceEaseT, 30.f, 160.f, 270.f);
		m_BodyPos.y=Easing::easeIn(WinceEaseT, 30.f, 2.f, 0.9f);
		if (++WinceEaseT >=30)
		{
			WinceEaseT = 0.f;
			wince_phase_ = WincePhase::END;
		}
	}
	else if (wince_phase_ == WincePhase::END)
	{
		StanCount++;
		if (StanCount >= 120) {
			if (++WinceEaseT >= 50)
			{
				WinceEaseT = 0.f;
				WinceF = FALSE;
			} else
			{
				m_BodyPos.y = Easing::easeIn(WinceEaseT, 50.f, 0.f, 2.f);
				m_BodyRot.x = Easing::easeIn(WinceEaseT, 50.f, 270.f, 180.f);
			}
		}
	}


}

