#include "Dogom.h"
#include <algorithm>
#include <dinput.h>
#include <random>

#include "Collision.h"
#include"Easing.h"
#include "ImageManager.h"
#include "KeyInput.h"
#include "Shapes.h"

#include"Helper.h"
#include "PadInput.h"
#define BOSSMAP_C 0.f
#define BOSSMAP_H 12.f
#define BOSSMAP_W 15.f

void Dogom::Init()
{
	m_Name = DOGOM;
	
	for (int32_t i = 0; i < 8; i++) {
		ArmModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "dogomu_hand.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
	for (int32_t i = 0; i < 8; i++) {
		BodyModel_[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "dogomu_face.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
	m_HpTex=Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 0, 0 });
	m_FeedTex = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 0, 0 });
	m_FeedTex->SetSize(XMFLOAT2(1280, 720));
	m_FeedTex->SetColor(XMFLOAT3(0, 0, 0));
	m_FeedTex->SetAlpha(m_FeedAlpha);
	m_FeedAlpha = 0.f;
	m_Body = Object3d::UniquePtrCreate(BodyModel_[0]);
	//m_Body->SetIsBillboardY(true);
	m_Body->SetColType(Object3d::CollisionType::Obb);
	m_Body->SetObjType((int32_t)Object3d::OBJType::Enemy);
	m_Body->SetObbScl({ 9.f,9.f,9.f });
	m_Body->SetHitRadius(0.5f);
	m_Body->SetScale({ 0.0f, 0.0f, 0.0f });

	arm_move_ = DEFAULT;
	/*本体モデル設定*/
	//m_BodyPos = { 0,-3,30 };
	//m_BodyRot.x = 180;
	/*腕モデル設定*/
	for (size_t i = 0; i < 2; i++) {
		m_ArmAlpha[i] = 1.f;
		m_Arm[i] = Object3d::UniquePtrCreate(ArmModel_[0]);
		//m_Body->SetIsBillboardY(true);
		m_Arm[i]->SetColType(Object3d::CollisionType::Obb);
		m_Arm[i]->SetObjType((int32_t)Object3d::OBJType::Enemy);
		m_Arm[i]->SetObbScl({ 1.5f,4.f,1.5f });
		m_Arm[i]->SetHitRadius(0.5f);
		m_Arm[i]->SetScale({ 0.10f, 0.20f, 0.0f });

		m_ShadowTex[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "Shadow.png", { 48, 48 }, { 0.5f, 0.5f }, { 0, 0 }, { 64, 64 }));
		m_ShadowTex[i]->SetRotation({ 90.f,0.f,0.f });

		m_ImpactTex[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "dogomu_hand.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f }));
		m_ImpactTex[i]->SetRotation({ 90,0,0 });

		m_ArmHpTex[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({0, 0}, {128.0f, 128.0f}, "white1x1.png", {128.0f, 64.0f}, {0.f, 0.f}, {128.0f * (float)i, 0.0f}, {128.0f, 128.0f}));
		
	}

	CrossAreaTex = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64, 64 }, "CrossArea.png", { 64, 64 }, { 0.5f, 0.5f }, { 0, 0 }, { 128, 128 }));
	CrossAreaTex->SetRotation(Vector3(90, 0, 0));
	CrossAreaTex->SetScale(Vector3(0.2f, 0.05f, 2.f));

	MovingAngle = 180.f;
	m_ArmPos[RIGHT] = Vector3(m_BodyPos.x + 8.f, m_BodyPos.y , m_BodyPos.z-30.f);
	m_ArmPos[LEFT] = Vector3(m_BodyPos.x - 8.f, m_BodyPos.y , m_BodyPos.z-30.f);

	float pi_ = 3.14f;
	m_BodyPos.x = sinf(MovingAngle * (pi_ / 180.0f)) * 16.0f;
	m_BodyPos.z = -4.f + cosf(MovingAngle * (pi_ / 180.0f)) * 16.0f;

	DeathAct = &Dogom::Death_Non;

	BossMaxHP = m_HP;
}

void Dogom::Upda()
{
	isAttack = (KeyInput::GetIns()->HoldKey(DIK_SPACE) || PadInput::GetIns()->PushButton(PadInput::Button_B));
	
	constexpr float pi_ = 3.14f;

	Follow();
	Wince();
	if (!WinceF)WinceEaseT = 0;
	//RecvDamage(m_BodyPos);
	
	auto isHit = [](Vector3 pos1, Vector3 pos2, float radi1 = 1.f, float radi2 = 1.f) ->
		bool { if (Collision::GetIns()->HitCircle({ pos1.x, pos1.z }, radi1,
			{ pos2.x, pos2.z }, radi2))return true;
	return false;
	};

	
	//登場終わったら行動
	if (Appear() == TRUE) {
		////////
		m_player->SetStopF(FALSE);
		if(m_player->GetPos().z<3.f)
		StartWaitT++;
		////////
		
			if (!WinceF) {
				MoveBody();
				m_BodyPos.x = sinf(MovingAngle * (pi_ / 180.0f)) * 16.0f;
				m_BodyPos.z = -4.f + cosf(MovingAngle * (pi_ / 180.0f)) * 16.0f;
			} else {
				constexpr int RecvCoolMax = 120;
				const int DamageVal = 1;
				bool judg = isAttack && isHit(m_BodyPos, m_player->GetHammer()->GetMatWorld().r[3], 3.f, 1.f);

				Helper::DamageManager(m_HP, DamageVal, BodyRecvDam, BodyDamCool, RecvCoolMax, judg);
			}
			
			ImpactKnock();
			ArmAct();
		
	}
	else
	{
		m_player->SetStopF(TRUE);
		m_camera->SetTarget(m_Target);
		m_camera->SetEye(m_CameraPos);
	}
	if (isHit(m_player->GetPos(), m_BodyPos,1.f,3.f))
	{
		Vector3 vec;
		vec = PlayerPos - m_BodyPos;
		vec.normalize();
		vec.y = 0.0f;

		m_player->HitHammerToEnemy(vec);
	}

	CoollisionArm();
	CoollisionFace();
	ImpactTexScling();
	RotationFace(120);

	constexpr int AnimationInter = 10;
	constexpr size_t TexNum = 8;
	static int animeCount = 0;

	if (++animeCount >= AnimationInter*TexNum) {
		animeCount = 0;
	}

	if (animeCount % AnimationInter == 0) {
		m_Body->SetModel(BodyModel_[animeCount / AnimationInter]);
		m_Body->Initialize();

		for (size_t i = 0; i < m_Arm.size(); i++)
		{
			if (ColF[0] && DamCool[i] < 10) {
				m_Arm[i]->SetModel(ArmModel_[animeCount / AnimationInter]);
				m_Arm[i]->Initialize();
			} else
			{
				m_Arm[i]->SetModel(ArmModel_[0]);
				m_Arm[i]->Initialize();
			}
		}
	}
	for (size_t i = 0; i < 2; i++) {
		m_ArmPos[i].x = std::clamp(m_ArmPos[i].x,-BOSSMAP_W, BOSSMAP_W);
		m_ArmPos[i].z = std::clamp(m_ArmPos[i].z, -BOSSMAP_H, BOSSMAP_H);
	}
	//m_BodyPos = std::clamp(m_BodyPos, Vector3(-BOSSMAP_W, -5, -BOSSMAP_H), Vector3(BOSSMAP_W, -5, BOSSMAP_H));

	if(m_HP<=0)
	DeathMotion();
	m_Body->SetRotation({ m_BodyRot.x,m_BodyRot.y,m_BodyRot.z });
	m_Body->SetScale({ BodyScl });

	Feed();

	m_FeedTex->SetAlpha(m_FeedAlpha);

	m_Arm[RIGHT]->SetRotation(Vector3(0, 0, 180));
	m_Body->SetPosition(m_BodyPos);
	m_Body->SetColor({ 1,1,1,m_BodyAlpha });
	m_Body->Update();

	constexpr float alphaval = 0.05f;

	float HpBarSclX[2] = {};
	for (size_t i = 0; i < 2; i++)
	{
		if (m_ArmHp[i] <= 0)m_ArmAlpha[i] -= alphaval;
		else m_ArmAlpha[i] += alphaval;
		m_ShadowTex[i]->SetPosition({ m_ArmPos[i].x,-3.5f,m_ArmPos[i].z });
		m_ShadowTex[i]->SetScale(ShadowScl(m_ArmPos[i].y));
		m_ShadowTex[i]->SetRotation(m_ArmRot[i]);
		m_ShadowTex[i]->SetColor({ 1,1,1,0.6f });
		m_ShadowTex[i]->Update();

		m_Arm[i]->SetScale({ 0.050f,0.070f,2.0f });
		m_Arm[i]->SetPosition(m_ArmPos[i]);
		m_Arm[i]->SetColor({ 1,1,1,m_ArmAlpha[i] });
		m_ArmPos[i].x = std::clamp(m_ArmPos[i].x, -9.f, 9.f);
		m_ArmPos[i].z = std::clamp(m_ArmPos[i].z, -9.f, 6.f);
		m_ArmAlpha[i]=std::clamp(m_ArmAlpha[i], 0.f, 1.f);
		m_Arm[i]->Update();

		m_ArmHpTex[i]->SetPosition(Vector3(m_ArmPos[i].x+2.f, m_Arm[i]->GetPosition().y + 2.5f, m_ArmPos[i].z));
		
		constexpr float magniVal = 0.04f;
		const float maxSx = 0.05f;

		//1までの値にスケールの倍率あわせる
		HpBarSclX[i]=Helper::SmoothStep_Deb(0.f,m_ArmHp_Max[i], (float)(m_ArmHp[i]))*magniVal;

		m_ArmHpTex[i]->SetColor(XMFLOAT4(1, 0, 0, 1));
		m_ArmHpTex[i]->SetScale(Vector3(HpBarSclX[i], 0.005f, 1.f));
		m_ArmHpTex[i]->Update();

	
		m_ImpactTex[i]->SetScale(m_ImpactTexScl[i]);
		m_ImpactTex[i]->SetPosition(m_ImpactTexPos[i]);
		m_ImpactTex[i]->SetColor({ 1, 1, 1, m_ImpactTexAlpha[i] });
		m_ImpactTex[i]->Update();
	}

	CrossAreaTex->SetColor({ 1,1,1,m_CrossAreaAlpha });
	CrossAreaTex->Update();

	if (m_player->GetPos().z > BOSSMAP_H &&AppearFlag)
		isLeaveBoss = TRUE;
	else
		isLeaveBoss = FALSE;

	str = std::to_wstring(m_player->GetPos().z);
}

void Dogom::Draw()
{
	if (isLeaveBoss)return;
	//if()
	//if (m_HP <= 0)return;
	CrossAreaTex->Draw();
	m_Body->Draw();
	
}
void Dogom::Draw2()
{
	if (isLeaveBoss)return;
	//if (m_HP <= 0)return;
	constexpr float BossDraw_maxlen = 125.f;
	for (size_t i = 0; i < 2; i++) {
		Helper::isDraw(m_player->GetPos(), m_ImpactTexPos[i], m_ShadowTex[i].get(),
			BossDraw_maxlen, (!ShadowHpTexisDraw ||m_HP <= 0 || m_ArmHp[i] <= 0));
	}
	for (size_t i = 0; i < 2; i++) {
		Helper::isDraw(m_player->GetPos(), m_ImpactTexPos[i], m_ImpactTex[i].get(),
			BossDraw_maxlen, (m_HP <= 0 || m_ArmHp[i] <= 0));

		Helper::isDraw(m_player->GetPos(), m_ArmPos[i],
			m_Arm[i].get(), BossDraw_maxlen, (m_HP <= 0 || m_ArmHp[i] <= 0));

	}
	Helper::isDraw(m_player->GetPos(), m_ArmPos[0],
		m_ArmHpTex[0].get(), BossDraw_maxlen, (!ShadowHpTexisDraw || m_HP <= 0 || m_ArmHp[0] <= 0));
	Helper::isDraw(m_player->GetPos(), m_ArmPos[1],
		m_ArmHpTex[1].get(), BossDraw_maxlen, (!ShadowHpTexisDraw || m_HP <= 0 || m_ArmHp[1] <= 0));

}


void Dogom::Attack()
{

}

Vector3 Dogom::ShadowScl(float YPos)
{
	Vector3 scale;
	constexpr float Ground = -3.5f;
	float armYpos_Ground[2] = { abs(m_ArmPos[RIGHT].y - Ground),abs(m_ArmPos[LEFT].y - Ground) };
	

	constexpr float maxX = 0.14f,maxY=0.07f;
	constexpr float minX = 0.04f,minY=0.03f;

	//線形補間で影の大きさ変える
	scale.x = Helper::SmoothStep_Deb(-Ground, Ground, YPos)*maxX;
	scale.y = Helper::SmoothStep_Deb(-Ground, Ground, YPos) * maxY;
	scale.z = 1.f;

	scale.x = std::clamp(scale.x, minX, maxX);
	scale.y = std::clamp(scale.y, minY, maxY);

	return scale;
}

void Dogom::Finalize()
{
	m_Body.reset(nullptr);
	m_Arm[RIGHT].reset(nullptr);
	m_Arm[LEFT].reset(nullptr);
}

void Dogom::ShakeArm(Vector3& Defopos,float &time)
{
	int randX = rand() % 10 - 5;
	int randY = rand() % 10 - 5;
	
	time += 90.f;
	for (size_t i = 0; i < 2; i++) {
		Defopos.x += sin(time * 3.14f / 180.f)/6.f;
	}
}

#define PI 3.14
void Dogom::ArmAct()
{
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f }, move2 = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX matRot_R = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y - 65.f));
	XMMATRIX matRot_L = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y + 65.f));

	move = XMVector3TransformNormal(move, matRot_R);
	move2 = XMVector3TransformNormal(move2, matRot_L);

	const float dis = 80.f;
	constexpr float CenterX = 0.f, CenterZ = -4.f;
	float Left_X =CenterX+ move.m128_f32[0] * dis,
	Left_Z =CenterZ+ move.m128_f32[2] * dis;

	float Right_X = CenterX+ move2.m128_f32[0] * dis,
	Right_Z =  CenterZ+move2.m128_f32[2] * dis;

	Vector3 OldRushPaunch[2];

	Vector3 oldarmpos[2];
	//通常
	if (arm_move_ == DEFAULT) {
		constexpr float GroundY = -1.5f;
		//上下周期
		constexpr float MovingInter = 240.f;
		//振れ幅
		constexpr float MovingVel = 0.40f;

		m_ArmMov_Y[LEFT]++;
		m_ArmMov_Y[RIGHT] = m_ArmMov_Y[LEFT] / 1.2f;

		m_ArmPos[LEFT].x = Left_X;
		m_ArmPos[LEFT].z = Left_Z;

		m_ArmPos[RIGHT].x = Right_X;
		m_ArmPos[RIGHT].z = Right_Z;

		for (size_t i = 0; i < 2; i++) {
			m_ArmPos[i].y = GroundY + sinf(PI * 2.f / MovingInter * m_ArmMov_Y[i]) * MovingVel;
			BefoPos[i] = m_ArmPos[i];
		}
		OldRushPaunch[LEFT] = m_ArmPos[LEFT];

		OldRushPaunch[RIGHT] = m_ArmPos[RIGHT];

		bool isNextActTim = m_ActionTimer != 0 && m_ActionTimer % 160 == 0;

		if (StartWaitT>90&&!isLeaveBoss&&!WinceF&&isNextActTim) {
			ActionRandom = rand() % 100;
			if (ActionRandom > 0) {
				SetAttack_Impact();
				arm_move_ = ATTACK_IMPACT;
			} else
			{
				SetAttack_Cross();
				arm_move_ = ATTACK_CROSS;
			}
		}
		if (!movF) {
			BossBodyMovingT++;
			m_ActionTimer++;
		}
		
		MovingAngle = std::clamp(MovingAngle, 90.f, 270.f);
	}

	//叩きつけ
	else if (arm_move_ == ATTACK_IMPACT)
	{;
		//追加時間(腕戻す)
		float MaxTime_1 = 20.f;
		constexpr float MaxTime_2 = 10.f;
		constexpr float MaxTime_3 = 10.f;

		bool next_2 = FALSE, next_3 = FALSE;

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f }, move2 = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot_R = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y - 90.f));
		XMMATRIX matRot_L = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y + 90.f));

		move = XMVector3TransformNormal(move, matRot_R);
		move2 = XMVector3TransformNormal(move2, matRot_L);
		
		switch (phase_)
		{
		case Phase_Impact::PHASE_1:
			m_ActionTimer = 0; ActionRandom = 0;
			ResetArmParam();
			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];

			BefoEaseT = m_ArmAttckEaseT[LEFT];
			
			if (m_ImpactCout > 0) {
				MaxTime_1 = 30;
				if(abs(270 - MovingAngle) <= 10.f)
				{
					m_ArmPos[LEFT].x = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_1, BefoPos[LEFT].x, BefoPos[LEFT].x+ 2.5f);
					m_ArmPos[RIGHT].x = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_1, BefoPos[RIGHT].x, BefoPos[RIGHT].x + 2.5f);
				}
				else if (abs(180 - MovingAngle) <= 10.f)
				{
					m_ArmPos[LEFT].z = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_1, BefoPos[LEFT].z, BefoPos[LEFT].z + 2.5f);
					m_ArmPos[RIGHT].z = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_1, BefoPos[RIGHT].z, BefoPos[RIGHT].z + 2.5f);
				}
				else if (abs(90 - MovingAngle) <= 10.f)
				{
					m_ArmPos[LEFT].x = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_1, BefoPos[LEFT].x, BefoPos[LEFT].x - 2.5f);
					m_ArmPos[RIGHT].x = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_1, BefoPos[RIGHT].x, BefoPos[RIGHT].x -2.5f);
				}
				
			}
			else {
				MaxTime_1 = 90.f;
			}
			if (m_ArmAttckEaseT[LEFT] >= MaxTime_1) {

				if(m_ImpactCout==0)
				{
					//m_ArmPos[LEFT] = OldRushPaunch[LEFT];
				//	m_ArmPos[RIGHT] = OldRushPaunch[RIGHT];
					
				}
				for (size_t i = 0; i < 2; i++) {
					BefoPos[i] = m_ArmPos[i];
				}
				m_ArmAttckEaseT[LEFT] = m_ArmAttckEaseT[RIGHT] = 0;
				next_2 = TRUE;
			} else {
				if (m_ImpactCout == 0) {
					move = { 0.0f, 0.0f, 0.1f, 0.0f }, move2 = { 0.0f, 0.0f, 0.1f, 0.0f };
					matRot_R = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y - 95.f));
					matRot_L = XMMatrixRotationY(XMConvertToRadians(m_BodyRot.y + 95.f));

					move = XMVector3TransformNormal(move, matRot_R);
					move2 = XMVector3TransformNormal(move2, matRot_L);

					m_EaseRemBody++;
					m_ArmPos[LEFT].x = Easing::easeIn(m_EaseRemBody, MaxTime_1, BefoPos[LEFT].x, m_BodyPos.x + move.m128_f32[0] * 50.f);
					m_ArmPos[RIGHT].x = Easing::easeIn(m_EaseRemBody, MaxTime_1, BefoPos[RIGHT].x, m_BodyPos.x + move2.m128_f32[0] * 50.f);

					m_ArmPos[LEFT].z = Easing::easeIn(m_EaseRemBody, MaxTime_1, BefoPos[LEFT].z, m_BodyPos.z + move.m128_f32[2] * 50.f);
					m_ArmPos[RIGHT].z = Easing::easeIn(m_EaseRemBody, MaxTime_1, BefoPos[RIGHT].z, m_BodyPos.z + move2.m128_f32[2] * 50.f);
					ShakeArm(m_ArmPos[RIGHT], t);
					ShakeArm(m_ArmPos[LEFT], t);
				}
				m_ArmPos[LEFT].y = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_1, BefoPos[LEFT].y, m_BodyPos.y + 3.f);
				m_ArmPos[RIGHT].y = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_1, BefoPos[RIGHT].y, m_BodyPos.y + 3.f);
			}

			if (next_2)
				phase_ = Phase_Impact::PHASE_2;
			break;

		case Phase_Impact::PHASE_2:
			//カウンタを右と左でずらす
			if (m_ArmAttckEaseT[LEFT] >= MaxTime_2)
				m_ArmAttckEaseT[RIGHT]++;
			else
				m_ArmAttckEaseT[LEFT]++;

			m_EaseRemBody = 0;
			if (m_ArmAttckEaseT[RIGHT] >= MaxTime_2) {
				BefoEaseT = m_ArmAttckEaseT[LEFT];
				m_ArmAttckEaseT[LEFT] = m_ArmAttckEaseT[RIGHT] = 0;
				next_3 = TRUE;
			} else {
				m_ArmPos[LEFT].y = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_2, BefoPos[LEFT].y, -3.5f);
				m_ArmPos[RIGHT].y = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_2, BefoPos[RIGHT].y, -3.5f);
			}
			BefoPos[LEFT].z = m_ArmPos[LEFT].z;
			BefoPos[RIGHT].z = m_ArmPos[RIGHT].z;


			if (next_3)
				phase_ = Phase_Impact::PHASE_3;
			break;

		case Phase_Impact::PHASE_3:

			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];
			//腕の位置地面
			BefoPos[0].y = BefoPos[1].y = -3.5f;

			//ボスの横に
			m_ArmPos[LEFT].y = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_3, BefoPos[LEFT].y, m_BodyPos.y );
			m_ArmPos[RIGHT].y = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_3, BefoPos[RIGHT].y, m_BodyPos.y );

			if(m_ImpactCout==3)
			{
				m_ArmPos[LEFT].x = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_3, BefoPos[LEFT].x,Left_X);
				m_ArmPos[RIGHT].x = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_3, BefoPos[RIGHT].x, Right_X);

				m_ArmPos[LEFT].z = Easing::easeIn(m_ArmAttckEaseT[LEFT], MaxTime_3, BefoPos[LEFT].z, Left_Z);
				m_ArmPos[RIGHT].z = Easing::easeIn(m_ArmAttckEaseT[RIGHT], MaxTime_3, BefoPos[RIGHT].z,Right_Z);
			}
			if (m_ArmAttckEaseT[LEFT] >= MaxTime_3)
				if (m_ImpactCout == 3) {
					phase_ = Phase_Impact::END;
				} else
				{
					ResetArmParam();
					m_ImpactCout++;

					for (size_t i = 0; i < 2; i++) {
						BefoPos[i] = m_ArmPos[i];
						m_ArmAttckEaseT[i] = 0;
					}
					phase_ = Phase_Impact::PHASE_1;
				}
				break;

		case Phase_Impact::END:
			m_ImpactCout = 0;
			m_ArmAttckEaseT[LEFT] = m_ArmAttckEaseT[RIGHT] = 0.f;
			next_2 = FALSE, next_3 = FALSE;
			arm_move_ = DEFAULT;
			break;
		}
		m_EaseRemBody = std::clamp(m_EaseRemBody, 0.f, MaxTime_1);

	}

	else if (arm_move_ == ATTACK_CROSS)
	{
		/*XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_player->GetRot().y));
		
		move = XMVector3TransformNormal(move, matRot);*/
		bool next_1 = FALSE, next_2 = FALSE;
		PlayerPos = m_player->GetPos();
		float ClushPos=0;
		float EndX[2] = { PlayerPos.x + 8.f ,PlayerPos.x - 8.f };
		float EndZ[2] = { PlayerPos.z ,PlayerPos.z };
		switch (phase_cross_)
		{
		case Phase_Cross::PHASE_1:
			m_ActionTimer = 0; ActionRandom = 0;
			ResetArmParam();
			

			//腕座標をプレイヤー位置に
			
m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];
			if (m_ArmAttckEaseT[LEFT] >= 30.f)
			{
				m_CrossAreaAlpha += 0.5f;
				if (m_CrossAreaAlpha<=0.5f) {
					CrossAreaTex->SetPosition({ m_ArmPos[RIGHT].x + ((m_ArmPos[LEFT].x - m_ArmPos[RIGHT].x) / 2),
						-3.5f,m_ArmPos[0].z });
				}
					for (size_t i = 0; i < 2; i++)
						ShakeArm(m_ArmPos[i], t3);

				if (++CrossWaitCount > 90.f) {
					m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT] = 0.f;
					BefoPos[LEFT] = m_ArmPos[LEFT], BefoPos[RIGHT] = m_ArmPos[RIGHT];
					
					next_1 = TRUE;
				}
			}
			else
			{
				if(m_ArmAttckEaseT[0]==0.f)
				ClushPos = PlayerPos.x;

					for (size_t i = 0; i < 2; i++) {
				m_ArmPos[i].x = Easing::easeIn(m_ArmAttckEaseT[i], 30.f, BefoPos[i].x, EndX[i]);
				m_ArmPos[i].y = Easing::easeIn(m_ArmAttckEaseT[i], 30.f, BefoPos[i].y,-2.5f);
				m_ArmPos[i].z = Easing::easeIn(m_ArmAttckEaseT[i], 30.f, BefoPos[i].z, EndZ[i]);
			}
			}

			if (next_1)phase_cross_ = Phase_Cross::PHASE_2;
			break;

		case Phase_Cross::PHASE_2:
			CrossWaitCount = 0.f;
			m_ArmAttckEaseT[LEFT]++;
			m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT];
			
			//for (size_t i = 0; i < 2; i++)
				m_ArmPos[RIGHT].x = Easing::easeIn(m_ArmAttckEaseT[RIGHT], 30.f, BefoPos[RIGHT].x,ClushPos+1.f);
				m_ArmPos[LEFT].x = Easing::easeIn(m_ArmAttckEaseT[LEFT], 30.f, BefoPos[LEFT].x, ClushPos-2.f);

			if (m_ArmAttckEaseT[LEFT] >= 30.f) {
				m_ArmAttckEaseT[RIGHT] = m_ArmAttckEaseT[LEFT] = 0.f;
				BefoPos[LEFT] = m_ArmPos[LEFT], BefoPos[RIGHT] = m_ArmPos[RIGHT];
				next_2 = TRUE;
			}
			if (next_2)phase_cross_ = Phase_Cross::PHASE_3;
			break;

		case Phase_Cross::PHASE_3:
			m_CrossAreaAlpha -= 0.02f;
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
	m_CrossAreaAlpha = std::clamp(m_CrossAreaAlpha, 0.f, 0.6f);
}

void Dogom::Follow()
{
	float Len = Collision::GetLength(m_player->GetPos(), m_BodyPos);
	bool notFollow = Len < 1.f;

	XMVECTOR posA = { m_player->GetPos().x,m_player->GetPos().y,m_player->GetPos().z };
	XMVECTOR posB = { m_BodyPos.x,m_BodyPos.y,m_BodyPos.z };

	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVec = XMVectorSubtract(posB, posA); // positionA - positionB;

	float toPlayer;
	toPlayer = atan2f(SubVec.m128_f32[0], SubVec.m128_f32[2]);

	if (!WinceF)
		m_BodyRot.y = toPlayer * 60.f + 180.f;

	XMVECTOR move = { 0.f,0.1f, 0.f, 0.0f };
	XMMATRIX matRot = XMMatrixRotationX(XMConvertToRadians(m_BodyRot.x + 270.f));

	move = XMVector3TransformNormal(move, matRot);

	if (WinceF&& wince_phase_ == WincePhase::PHASE_2) {
	//	if (Len > 2.f) {
			m_BodyPos.y += move.m128_f32[1] * 2.1f;

			m_BodyPos.z += move.m128_f32[2] * 2.1f;
		//}
	}
}

void Dogom::Wince()
{
	if (m_HP <= 0)return;

	WinceIdle();

	if (!WinceF)return;
	constexpr float MaxEase = 30.f;
	if (wince_phase_ == WincePhase::PHASE_1) {
		oldBodyPos = m_BodyPos;
		if (++WinceEaseT >= MaxEase) {
			WinceEaseT = 0.f;
			wince_phase_ = WincePhase::PHASE_2;
		} else
			m_BodyRot.x = Easing::easeIn(WinceEaseT, MaxEase, 0.f, -20.f);
	} else if (wince_phase_ == WincePhase::PHASE_2)
	{
		m_BodyRot.x = Easing::easeIn(WinceEaseT, 30.f, -20.f, 90.f);
		m_BodyPos.x= Easing::easeIn(WinceEaseT, 30.f, oldBodyPos.x, 0.f);
		m_BodyPos.z = Easing::easeIn(WinceEaseT, 30.f, oldBodyPos.z, 0.f);
		m_BodyPos.y = Easing::easeIn(WinceEaseT, 30.f, 2.f, -2.f);
		if (++WinceEaseT >= 30)
		{
			WinceEaseT = 0.f;
			wince_phase_ = WincePhase::END;
		}
	} else if (wince_phase_ == WincePhase::END)
	{
		int randX = rand() % 10 - 5;
		int randY = rand() % 10 - 5;

		
		StanCount++;
		if (StanCount >= 1320) {
			if (++WinceEaseT >= 50)
			{
				m_ArmHp[LEFT] = m_ArmHp[RIGHT] = ArmHP();
				WinceF = FALSE;
				WinceEaseT = 0.f;
				
			} else
			{
				m_BodyPos.x = Easing::easeIn(WinceEaseT, 50.f,0.f, oldBodyPos.x);
				m_BodyPos.z = Easing::easeIn(WinceEaseT, 50.f, 0.f,oldBodyPos.z);
				m_BodyPos.y = Easing::easeIn(WinceEaseT, 50.f, 0.f, 2.f);
				m_BodyRot.x = Easing::easeIn(WinceEaseT, 50.f, 90.f, 00.f);
			}
		}
		else
		{
			if (StanCount > 1360) {
				t2 += 90.f;
				m_BodyPos.x += sin(t2 * 3.14f / 180.f) / 6.f;
			}
		}
	}
}

void Dogom::MoveBody()
{
	if (StartWaitT <= 90)return;
	//if (arm_move_ != DEFAULT)return;
	if (movF && arm_move_ == DEFAULT) {
		BodyMoveEase++;
		if (OldMovAngle == 180) {
			BodyScl.x = Easing::easeIn(BodyMoveEase, 90.f, 0.1f,0.06f);
			BodyScl.y = Easing::easeIn(BodyMoveEase, 90.f,0.2f, 0.12f);
			
		}
		else
		{
			BodyScl.x = Easing::easeIn(BodyMoveEase, 90.f, 0.06f, 0.1f);
			BodyScl.y= Easing::easeIn(BodyMoveEase, 90.f, 0.12f, 0.2f);
		}
			MovingAngle = Easing::easeIn(BodyMoveEase, 90.f, OldMovAngle, nextAngle);
		if (BodyMoveEase >= 90) {
			BossBodyMovingT = 0;
			BodyMoveEase = 0.f;
			movF = false;
		}
	}
	else{
		BossBodyMovingT++;
		OldMovAngle = MovingAngle;

		if (BossBodyMovingT % 360 == 0&& arm_move_ == DEFAULT) {
		if (abs(270-OldMovAngle) <= 10.f)
			nextAngle = 180.f;
		else if (abs(90-OldMovAngle) <= 10.f)
			nextAngle = 180.f;
		else if (abs(180 - OldMovAngle) <= 10.f)
		{
			randAct = rand() % 10;
			if (randAct <5 )nextAngle = 90.f;
			else nextAngle = 270.f;
		}
			movF = true;
		}
	}
	BodyScl.y = std::clamp(BodyScl.y, 0.15f, 0.2f);

	BodyScl.x = std::clamp(BodyScl.x, 0.05f, 0.1f);
}

void Dogom::ImpactTexScling()
{
	constexpr float GroundY = -3.5f;

	if (phase_ == Phase_Impact::PHASE_2) {
		for (size_t i = 0; i < 2; i++) {
			if (m_ImpactF[i])continue;
			//if (m_ArmPos[i].y > 1.f)continue;
			m_ImpactTexPos[i] = Vector3(m_ArmPos[i].x, GroundY, m_ArmPos[i].z);
			m_ImpactTexScl[i] = Vector3(0, 0, 0);
			m_ImpactTexAlpha[i] = 1.f;
			m_ImpactF[i] = TRUE;
		}
	}
	for(size_t i=0;i<2;i++)
	{
		if (!m_ImpactF[i])continue;
		m_ImpactTexScl[i].x += 0.005f;
		m_ImpactTexScl[i].y += 0.005f;

		m_ImpactTexAlpha[i] -= 0.03f;

		if (m_ImpactTexAlpha[i] <= 0.f)
			m_ImpactF[i] = FALSE;
	}

}

void Dogom::CoollisionFace()
{
	bool isColJudg[] = { phase_ == Phase_Impact::PHASE_2 ,phase_cross_ == Phase_Cross::PHASE_2 };

	if (!isColJudg[0] &&!isColJudg[1])return;
	if (WinceF)return;

	for (size_t i = 0; i < 2; i++) {
		if (ColF[0])continue;
		if (m_ArmHp[i] <= 0)continue;
		DamCool[i] = 0;
		float magniVal = 0.7f;

		if (Collision::GetLength(m_ArmPos[i], m_player->GetPos())<5.f)
		{	
			vec[i] = PlayerPos - m_ArmPos[i];
			vec[i].normalize();
			vec[i].y = 0.0f;

			m_player->HitHammerToEnemy(vec[i], magniVal);
			m_Knock = TRUE;

			m_player->SubHP(1);
			ColF[0] = TRUE;
		}
	}

	constexpr int cool = 30;

	for (size_t i = 0; i < 2; i++) {
		if (!ColF[0])continue;
		if (++DamCool[i] > cool)
		{
			ColF[0] = FALSE;
		}
	}

}


void Dogom::CoollisionArm()
{

	bool canCol = arm_move_ == DEFAULT && m_player->getisHammerActive();

	if (canCol) {
		for (size_t i = 0; i < 2; i++) {
		 	//
			if (m_ArmHp[i] <= 0)continue;
			if (m_Arm[i]->GetIsHit()) {
				m_player->SetIsHammerReflect(true);

				vec[i] = PlayerPos - m_ArmPos[i];
				vec[i].normalize();
				vec[i].y = 0.0f;

				m_player->HitHammerToEnemy(vec[i], 1.f);
			}
			constexpr int damval = 1;
			Helper::DamageManager(m_ArmHp[i], damval, m_ArmDamF[i], m_ArmDamCool[i], 60, m_Arm[i]->GetIsHit());
		}
	}

	//両腕の体力が消えたら
	if ((m_ArmHp[LEFT] <=0&& m_ArmHp[RIGHT]<= 0))
	{
		if (!WinceF) {
			StanCount = 0;
			WinceF = TRUE;
			wince_phase_ = WincePhase::IDLE;
		}
	}

}

int Dogom::ArmHP()
{
	//後で変える
	return m_ArmHp_Max[0];
}

void Dogom::RotationFace(const uint16_t& interval)
{
	if (m_HP <= 0)return;

	float oldFaceRot=0.f;

	//正負を返す
	struct ReturnSign {
		static float GetSignVal(uint16_t randVal)
		{
			if (randVal % 2 == 0)
				return 1.f;
			else
				return -1.f;
		}
	};

	if((++isRotateTim) % interval == 0)
		m_FaceRotaF = TRUE;

	if(m_FaceRotaF)
	{
		constexpr float maxEaseT = 60.f;

		//顔を左右どちらかに回転

		m_BodyRot.z = Easing::easeIn(m_FaceRotEaseT, maxEaseT, oldFaceRot, oldFaceRot + (MorP_Sign * 360.f));
		if (++m_FaceRotEaseT >= maxEaseT)
		{
			isRotateTim = 1;
			m_FaceRotaF = FALSE;
		}
	}
	else
	{
		
		m_FaceRotEaseT = 0.f;

		std::mt19937 mt{ std::random_device{}() };
		std::uniform_int_distribution<uint16_t> randval(0, 10);
		
		oldFaceRot = m_BodyRot.z;
		MorP_Sign = ReturnSign::GetSignVal(randval(mt));
	}
}


void Dogom::ImpactKnock()
{

	//if (phase_ != Phase_Impact::PHASE_2)return;
	constexpr float HandsUnderGround = -3.f;
	constexpr float KnockValMagni = 0.3f;/*ノック倍率*/

	bool KnockJudg = m_Knock == FALSE && phase_ == Phase_Impact::PHASE_2;
	if (KnockJudg) {
	
	}

	//フラグ値チェック
	m_KnockInterTime = m_Knock ? ++m_KnockInterTime : 0;

	constexpr UINT maxInter = 90;

	if (m_KnockInterTime > maxInter)
		m_Knock = FALSE;

}

void Dogom::FaceCol()
{
	
}

void Dogom::WinceIdle()
{
	if (!WinceF)return;
	//if (wince_phase_ != WincePhase::IDLE)return;

	constexpr uint16_t maxET = 120;

	if (wince_phase_ == WincePhase::IDLE || wince_phase_ == WincePhase::PHASE_1) {
		ShakeArm(m_BodyPos, t4);
	}

	if (++l_t>maxET&& wince_phase_ == WincePhase::IDLE)
	{
		m_BodyRot = Vector3(0, 0, 0);
		wince_phase_ = WincePhase::PHASE_1;
	}
}

void Dogom::SpriteDraw()
{
	float px = 880.f, py = 30.f;
	float sx,sy;
	//0~400の間での補完取る
	sx = Helper::SmoothStep_Deb(0, BossMaxHP, m_HP) * 400.f;
	sy = 50.f;

	m_HpTex->SetColor(XMFLOAT3(1, 0, 0));
	m_HpTex->SetPosition(XMFLOAT2(px,py));
	m_HpTex->SetSize(XMFLOAT2(sx, sy));
	
	m_HpTex->Draw();

	m_FeedTex->Draw();
}


bool Dogom::Appear()
{
	constexpr float maxAppTime[] = { 160,100,100 };

	if (_phase_appear == PHASE1) {
		//本体処理
		m_Target = Vector3(m_BodyPos.x, m_BodyPos.y + 2.5f, m_BodyPos.z);

		if (++appeaset < 100)
		{
			m_CameraPos.z = Easing::easeIn(appeaset, 100.f, m_player->GetPos().z, m_BodyPos.z + 20.f);
		}
		if (appeaset > maxAppTime[0]) {
			appeaset = 0;
			_phase_appear = PHASE2;
		}
	}
	else if (_phase_appear == PHASE2) {
		//カメラ処理
		if (++appeaset < maxAppTime[1])
		{
			m_CameraPos.z = Easing::easeIn(appeaset, 100.f, m_BodyPos.z + 20.f, m_CameraStartPos.z);
		} else {
			appeaset = 0;
			_phase_appear = PHASE3;
		}

		bool feed = appeaset > 80;
		if (feed)m_FeedF = true;
	}

	//こいつラスト行くまで更新きる
	else if (_phase_appear == PHASE3) {
		if (++appeaset > maxAppTime[2]) {
			if(m_HP>0)
			m_FeedF = false;
			ShadowHpTexisDraw = TRUE;
			AppearFlag = true;
			return true;
		}
	}

	return false;
}

void Dogom::Feed()
{
	float addval = 0.02f;

	//参照でもいい //暗転上がるときだけ早く
	auto judgfeed = [addval](bool f)->
		float {return f ? +addval : -(addval*2.f); };

	m_FeedAlpha += judgfeed(m_FeedF);

	//制限
	m_FeedAlpha = std::clamp(m_FeedAlpha, 0.f, 1.f);
}

void Dogom::HandImp()
{
	
}


void Dogom::DeathMotion()
{
	//if (m_HP > 1)return;
	//m_FeedF = true;
	float max = 50.f;
	auto isNext = [max](float time)->
		bool {return time > max; };

	//1P
	if (Dmotion_phase == DeathAct::CameraSet)
	{
		m_player->SetStopF(TRUE);
		m_FeedF = true;
		//motion//
		if (m_FeedAlpha >= 1.f) {
			m_FeedF = false;
			DeathAct = &Dogom::Death_Idle;
			Dmotion_phase = DeathAct::FeedShake;
		}
	}
	//2P
	else if(Dmotion_phase==DeathAct::FeedShake)
	{
		//motion//
		DeathAct = &Dogom::Death_Shake;
		//nextPhase//
		if (m_FeedCount>2 && isNext(m_DeathT)) {
			m_DeathT = 0.f;//reset
			Dmotion_phase = DeathAct::End;
		}
	}
	else
	{
		max = 30.f;
	
		//motion//
		DeathAct = &Dogom::Death_End;
	
		if(m_FeedAlpha>=1.f && isNext(m_DeathT))
		{
			m_ClearF = TRUE;
		}
		m_player->SetStopF(FALSE);
		m_FeedF = !m_ClearF;
	}

	(this->*DeathAct)();
}

void Dogom::Death_Idle()
{
	//ボス座標
	Vector3 bSet = Vector3(m_BodyPos.x, m_BodyPos.y + 1.5f, m_BodyPos.z + 5.f);

	m_FeedF = TRUE;
	MovingAngle = 180.f;

	float pi_ = 3.14f;
	m_BodyPos.x = sinf(MovingAngle * (pi_ / 180.0f)) * 16.0f;
	m_BodyPos.z = -4.f + cosf(MovingAngle * (pi_ / 180.0f)) * 16.0f;

	m_BodyRot = Vector3(0, 0, 0);
	m_Body->SetRotation(m_BodyRot);
	//CameraSetting//
	m_Camera->SetTarget(m_BodyPos);
	m_Camera->SetEye(bSet);

}

void Dogom::Death_Shake()
{
	constexpr float subalpha = 0.005f;

	BodyScl = { 0.3f,0.3f,1.f };
	m_FeedTex->SetColor({ 1,1,1 });

	if(m_FeedAlpha>=1.f)
	{
		++m_FeedCount;
		m_FeedF = FALSE;
	}
	else if(m_FeedAlpha<=0.f)
	{
		if(m_FeedCount <3)
		m_FeedF=TRUE;
	}
	//ShakeArm(m_BodyRot, m_DeathT);

	if (m_FeedAlpha <= 0.1f) {
		m_BodyAlpha -= subalpha;
		for (size_t i = 0; i < 2; i++)
			m_ArmAlpha[i] -= subalpha;
	}
	
}

void Dogom::Death_End()
{
	
}

void Dogom::Death_Non()
{
	return;
}
