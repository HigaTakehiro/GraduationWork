#include "LastBoss.h"
#include "LastBoss.h"

#include <algorithm>

#include "ExternalFileLoader.h"
#include "Helper.h"
#include "LastBossAct.h"
#include "Shapes.h"

#define MapX_Mx 10.f
#define MapX_Mn -10.f

#define MapZ_Mx 8.8f
#define MapZ_Mn -12.f

void LastBoss::Init()
{//待機
	for (size_t i = 0; i < 4; i++) {
		m_Model_Idle[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_idle.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
	//歩き
	for (size_t i = 0; i < 4; i++) {
		m_Model_Walk[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_move.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
		m_Model_Walk_Right[i] = Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "togemaru_moveRL.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
		m_Model_Walk_Left[i] = Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "togemaru_moveLR.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
		m_Model_Walk_Back[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 160.0f }, "togemaru_moveBack.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 160.0f });

	}
	//m_SpearsModel = Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "needle.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)0, 0.0f }, { 64.0f, 64.0f });
	m_HoleTex[0] = Object3d::UniquePtrCreate(Shapes::CreateSquare({0, 0}, {64.0f, 64.0f}, "Area.png", {64.0f, 64.0f}, {0.5f, 0.5f}, {64.0f * (float)0, 0.0f}, {64.0f, 64.0f}));
	m_HoleTex[1] = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "Area.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)0, 0.0f }, { 64.0f, 64.0f }));

	for (size_t i = 0; i < 4; i++)
	{
		m_FlameTex[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "Area.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)0, 0.0f }, { 64.0f, 64.0f }));
	}
	for (size_t i = 0; i < 3; i++)
	{
		m_GuardTex[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "Area.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)i, 0.0f }, { 64.0f, 64.0f }));
	}
	
	//for (int32_t i = 0; i < m_SpearArray; i++) {
	//}

	//InitAnimatin();
	m_Body = Object3d::UniquePtrCreate(m_Model_Idle[0]);
	m_Body->SetColType(Object3d::CollisionType::Obb);
	m_Body->SetObjType((int32_t)Object3d::OBJType::Enemy);
	m_Body->SetObbScl({ 9.f,9.f,9.f });
	m_Body->SetHitRadius(0.5f);
	m_Body->SetScale({ 0.020f, 0.050f, 0.040f });

	std::stringstream stream;
	std::string line;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Boss.csv");
	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("LastBoss") == 0) {
			getline(line_stream, word, ' ');
			m_HP = (int)std::atof(word.c_str());
			getline(line_stream, word, ' ');
			GuardValue = (int)std::atof(word.c_str());
			getline(line_stream, word, ' ');
			AttackValue = (float)std::atof(word.c_str());
			continue;
		}
	}
	m_BodyAlpha = 1.f;
	BossMaxHP = m_HP;
	names = "LastBoss";

	HPUiInit();
	Action = new LastBossAct();
}


//void LastBoss::UI_Draw()
//{
//	if (LastBossAct::LastBossDeathF)return;
//	for (size_t i = 0; i < m_ScaleArray; i++) {
//		m_ScaleSizeUI[i]->Draw();
//	}
//}


void LastBoss::Upda()
{
	//プレイヤーインスタンスセット
	Action->SetPlayerIns(m_player);
	//行動遷移
	Action->Transision();
	Action->SetHp(m_HP);


	Pos_ = Action->GetPos();

	bool isCol = Collision::HitCircle(XMFLOAT2(Pos_.x, Pos_.z + 3.f), 2.f, XMFLOAT2(m_player->GetHammmerPos().x, m_player->GetHammmerPos().z), 1.f);

	//	if (!nowcrush) {
	//Helper::DamageManager(m_HP, 1, DamF, DamCoolTime, 60, judg1 && isCol);
	//Helper::ColKnock(m_player->GetPos(), { Pos_.x,Pos_.y,Pos_.z + 3.f }, m_player, judg1 && isCol);
	//}

	//if (DamF)FlashF = true;
	RecvDamageFlash();
	//
	//AnimationSett();
	//各種パラメータセット


	//本体

	m_Body->SetScale(Action->GetScl());
	m_Body->SetPosition(Action->GetPos());
	m_Body->SetRotation(Vector3(0, 0, 0));
	m_Body->SetColor(color_rgb);
	m_Body->Update();

	for(size_t i=0;i<2;i++)
	{
		m_HoleTex[i]->SetRotation({ 90,0,0 });
		m_HoleTex[i]->SetScale({Action->GetHoleSize(i)});
		m_HoleTex[i]->SetPosition(Action->GetHolePos(i));
	}
	for (size_t i = 0; i < 4; i++)
	{
		m_FlameTex[i]->SetPosition(Action->GetFlamePos(i));
		m_FlameTex[i]->SetScale({ Action->GetFlameScl(i) });
		m_FlameTex[i]->SetColor(Action->GetFlameColor(i));

		m_FlameTex[i]->Update();
	}
	for (size_t i = 0; i < 3; i++)
	{
		m_GuardTex[i]->SetPosition(Action->GetBarrierPos(i));
		m_GuardTex[i]->SetScale({ 0.05f,0.05f,0.05f });
		m_GuardTex[i]->SetRotation({ 0,0,0 });
		m_GuardTex[i]->SetColor({ 1,1,1,Action->GetBarrierAlpha(i) });
		m_GuardTex[i]->Update();
	}

	m_HoleTex[0]->Update();
	m_HoleTex[1]->Update();

	//UI_Upda();
	HPUiUpda();
}

void LastBoss::Draw()
{
	if (m_player->GetPos().z > 12.f)return;
	m_Body->Draw();

	m_HoleTex[0]->Draw();
	m_HoleTex[1]->Draw();
	for(size_t i=0;i<4;i++)
	{
		m_FlameTex[i]->Draw();
	}
	for (size_t i = 0; i < 3; i++)
	{
		m_GuardTex[i]->Draw();
	}//UI_Draw();
}

void LastBoss::Draw2()
{

}

void LastBoss::SpriteDraw()
{
	HPUiDraw();
}

void LastBoss::Attack()
{

}

//void LastBoss::AddIndex(Model** model, int size)
//{
//	//アニメーション間隔
//	int NextIndInter = 30;
//	if (Action->GetName() == LastBossAct::AnimeName::ROLE) {
//		NextIndInter = 5;
//	} else {
//		NextIndInter = 30;
//	}
//	//現在のフレーム
//	int NowIndex = animeIndex / NextIndInter;
//
//	if (NowIndex > size - 1) {//最後まで再生されたら最初に戻す
//		animeIndex = 0;
//	} else {
//		if (++animeIndex % NextIndInter == 0) {
//			m_Body = Object3d::UniquePtrCreate(model[NowIndex]);
//		}
//	}
//}
//
//void LastBoss::AnimationSett()
//{
//	switch (Action->GetName())
//	{
//	case LastBossAct::AnimeName::IdlE:
//		AddIndex(m_Model_Idle, 4);
//		break;
//
//	case LastBossAct::AnimeName::WALK_FRONT:
//		AddIndex(m_Model_Walk, 4);
//		break;
//
//	case LastBossAct::AnimeName::WALK_RIGHT:
//		AddIndex(m_Model_Walk_Right, 4);
//		break;
//
//	case LastBossAct::AnimeName::WALK_LEFT:
//		AddIndex(m_Model_Walk_Left, 4);
//		break;
//
//	case LastBossAct::AnimeName::WALK_BACK:
//		AddIndex(m_Model_Walk_Back, 4);
//		break;
//
//	case LastBossAct::AnimeName::ROLE:
//		AddIndex(m_Model_Role, 2);
//		break;
//
//	case LastBossAct::AnimeName::CRUSH_FRONT:
//		AddIndex(m_Model_Crush, 4);
//		break;
//
//	case LastBossAct::AnimeName::CRUSH_RIGHT:
//		AddIndex(m_Model_Crush_Right, 4);
//		break;
//
//	case LastBossAct::AnimeName::CRUSH_LEFT:
//		AddIndex(m_Model_Crush_Left, 4);
//		break;
//
//	case LastBossAct::AnimeName::CRUSH_BACK:
//		AddIndex(m_Model_Crush_Back, 4);
//		break;
//	}
//}

//void LastBoss::InitAnimatin()
//{
	////待機
	//for (size_t i = 0; i < 4; i++) {
	//	m_Model_Idle[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "LastBoss_idle.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	//}
	////歩き
	//for (size_t i = 0; i < 4; i++) {
	//	m_Model_Walk[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "LastBoss_move.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	//	m_Model_Walk_Right[i] = Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "LastBoss_moveRL.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
	//	m_Model_Walk_Left[i] = Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "LastBoss_moveLR.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
	//	m_Model_Walk_Back[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 160.0f }, "LastBoss_moveBack.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 160.0f });

	//}
	////突進
	//for (size_t i = 0; i < 2; i++) {
	//	m_Model_Role[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "LastBoss_rot.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	//}
	//for (size_t i = 0; i < 4; i++) {
	//	m_Model_Crush[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "LastBoss_weekMove.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	//	m_Model_Crush_Right[i] = Shapes::CreateSquare({ 0, 0 }, { 192.0f, 128.0f }, "LastBoss_weekMoveLR.png", { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
	//	m_Model_Crush_Left[i] = Shapes::CreateSquare({ 0, 0 }, { 192.0f,128.0f }, "LastBoss_weekMoveRL.png", { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
	//	m_Model_Crush_Back[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 160.0f }, "LastBoss_moveBack.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 160.0f });

	//}
//}

bool LastBoss::Appear()
{
	Pos_ = Vector3(0, 0, -10);


	return true;
}

void LastBoss::Finalize()
{

}



