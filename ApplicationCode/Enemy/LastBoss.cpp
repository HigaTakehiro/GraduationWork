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
	for (size_t i = 0; i < 3; i++) {
		m_Model_Hole[i] = Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "blackHole.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)i, 0.0f }, { 64.0f, 64.0f });
	}
	for (size_t i = 0; i <2; i++) {
		MeteoModel[i] = Shapes::CreateSquare({ 0, 0 }, { 256.0f, 448.0f }, "meteorite.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 256.0f * (float)i, 0.0f }, { 256.0f, 448.0f });
	}
	
	for (size_t i = 0; i < 4; i++)
	{
		m_Model_Frames[i] = Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "normalFire.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)i, 0.0f }, { 64.0f, 64.0f });
		m_FlameTex[i] = Object3d::UniquePtrCreate(m_Model_Frames[0]);

		m_Shadow[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "Area.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)0, 0.0f }, { 64.0f, 64.0f }));
		
	}
	//m_FlameTex[i] = Object3d::UniquePtrCreate(m_Model_Frames[0]);

	for (size_t i = 0; i < 3; i++)
	{
		m_GuardTex[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "boss_shield.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)i, 0.0f }, { 64.0f, 64.0f }));
	}
	m_SpellTex = Object3d::UniquePtrCreate(Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "Area.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)0, 0.0f }, { 64.0f, 64.0f }));
	m_MeteoTex = Object3d::UniquePtrCreate(MeteoModel[0]);

	m_MeteoTex->SetPosition({0,20,0});
	//for (int32_t i = 0; i < m_SpearArray; i++) {
	//}

	
	InitAnimatin();
	m_Body = Object3d::UniquePtrCreate(m_Model_Idle[0]);
	m_Body->SetColType(Object3d::CollisionType::Obb);
	m_Body->SetObjType((int32_t)Object3d::OBJType::Enemy);
	m_Body->SetObbScl({ 9.f,9.f,9.f });
	m_Body->SetHitRadius(0.5f);
	m_Body->SetScale({ 0.015f, 0.050f, 0.040f });

	std::stringstream stream;
	std::string line;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Boss.csv");
	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("LastBoss") == 0) {
			getline(line_stream, word, ' ');
			m_HP =  (int)std::atof(word.c_str());
			getline(line_stream, word, ' ');
			GuardValue = (int)std::atof(word.c_str());
			getline(line_stream, word, ' ');
			AttackValue = (float)std::atof(word.c_str());
			getline(line_stream, word, ' ');
			Exp = (float)std::atof(word.c_str());
			continue;
		}
	}
	m_BodyAlpha = 1.f;
	BossMaxHP = m_HP;
	
	names = "LastBoss";

	HPUiInit();
	Action = new LastBossAct();

	Action->SetHp(BossMaxHP);
	Action->SetGuardPoint(GuardValue);
	Action->SetAttackPoint(AttackValue);
	particle = ParticleManager::UniquePtrCreate(DirectXSetting::GetIns()->GetDev(), camera);

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
	if (m_player->GetPos().z > 12.f)return;
	//プレイヤーインスタンスセット
	Action->SetPlayerIns(m_player);
	//行動遷移
	Action->Transision();

	Pos_ = Action->GetPos();
//各種パラメータセット
	m_HP = Action->GetHp();
	//if (m_HP > 0) {
	for (size_t i = 0; i < 3; i++)
	{
		//if (i < 3) {
		m_Shadow[i]->SetScale(Vector3(0.01f, 0.01f, 0.1f));
		m_Shadow[i]->SetPosition({ Action->GetBarrierPos(i).x,-2.f,Action->GetBarrierPos(i).z+3.f });
		//} else {
	}
	m_Shadow[3]->SetScale(Vector3(0.05f, 0.05f, 0.1f));
	m_Shadow[3]->SetPosition({ Pos_.x,-2.f,Pos_.z+3.f });
	//}
	for (size_t i = 0; i < 4; i++)
	{
		m_Shadow[i]->SetRotation(Vector3(90, 0, 0));
		m_Shadow[i]->SetColor(XMFLOAT4(0, 0, 0, 1));
		m_Shadow[i]->Update();
	}

	//}
	bool isCol = Collision::HitCircle(XMFLOAT2(Pos_.x, Pos_.z + 3.f), 2.f, XMFLOAT2(m_player->GetHammmerPos().x, m_player->GetHammmerPos().z), 1.f);

	//	if (!nowcrush) {
	//Helper::DamageManager(m_HP, 1, DamF, DamCoolTime, 60, judg1 && isCol);
	//Helper::ColKnock(m_player->GetPos(), { Pos_.x,Pos_.y,Pos_.z + 3.f }, m_player, judg1 && isCol);
	//}
	
	const int Inter = 20;
	if(holeanimtime/Inter>1)
	{
		holeanimtime = 0;
	}
	else {
		if (++holeanimtime % Inter == 0) {
			m_HoleTex[0] = Object3d::UniquePtrCreate(m_Model_Hole[holeanimtime / Inter]);
			m_HoleTex[1] = Object3d::UniquePtrCreate(m_Model_Hole[holeanimtime / Inter]);
		}
	}

	if (frameanim / Inter > 2)
	{
		frameanim = 0;
	} else {
		if (++frameanim % Inter == 0) {
			for (size_t i = 0; i < 4; i++)
			{
				m_FlameTex[i] = Object3d::UniquePtrCreate(m_Model_Frames[frameanim / Inter]);
			}
		}
	}

	meteoanim++;
		if (meteoanim < 30 ) {
			m_MeteoTex= Object3d::UniquePtrCreate(MeteoModel[0]);
		}else
		{
			m_MeteoTex = Object3d::UniquePtrCreate(MeteoModel[1]);
		}
	if(meteoanim>60)
	{
		meteoanim = 0;
	}

	if (m_HP <= 0 && !isGetExp) {
		m_player->AddEP(30);
		isGetExp = true;
	}
	//meteoanim = std::clamp(meteoanim, 0, 10);

	bool isCollide = Collision::HitCircle({ Pos_.x,Pos_.z + 3.f }, 1.2f, { m_player->GetPos().x,m_player->GetPos().z }, 1.f);
	if (m_HP > 0) {
		Helper::ColKnock(m_player->GetPos(), { Pos_.x,Pos_.y,Pos_.z + 3.f }, m_player, isCollide, 1.f);
	}
	DamF = Action->GetDamF();
	if (DamF)FlashF = true;
	RecvDamageFlash();
	//
	AnimationSett();
	
	//本体
	//bool isCollsion = m_player->getisHammerActive() && Collision::HitCircle(XMFLOAT2(Pos_.x, Pos_.z + 3.f), 2.f, XMFLOAT2(m_player->GetHammmerPos().x, m_player->GetHammmerPos().z), 1.f);

	//	if (!nowcrush) {
	//Helper::DamageManager(m_HP, 1, DamF, DamCoolTime, 60, isCol);
	//Helper::ColKnock(m_player->GetPos(), { Pos_.x,Pos_.y,Pos_.z + 3.f }, m_player,  isCol,1.f);
	if(m_HP<=0)
	{
		color_rgb.w -= 0.02f;
		if (color_rgb.w <= 0.f)m_ClearF = true;
	}
	else
	{
		if (Action->GetkillDraw())
			color_rgb.w -= 0.05f;
		else
			color_rgb.w += 0.05f;
	}
	
	if (m_ClearF)color_rgb.w = 0.f;
	m_Body->SetScale(Action->GetScl());
	m_Body->SetPosition({Action->GetPos().x,-2.f,Action->GetPos().z});
	m_Body->SetRotation(Vector3(0, 0, 0));
	m_Body->SetColor({color_rgb.x+0.5f,color_rgb.y + 0.5f,color_rgb.z + 0.5f,color_rgb.w});
	m_Body->Update();

	Action->Attack_Spell();
	for(size_t i=0;i<2;i++)
	{
		if (m_HoleTex[i] == nullptr)continue;
		m_HoleRot[i]++;
		m_HoleTex[i]->SetRotation({ 90,0,m_HoleRot[i]});
		m_HoleTex[i]->SetScale({Action->GetHoleSize(i)});
		m_HoleTex[i]->SetPosition(Action->GetHolePos(i));
		m_HoleTex[i]->Update();
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
		m_GuardTex[i]->SetPosition({ Action->GetBarrierPos(i) });
		m_GuardTex[i]->SetScale({ 0.05f,0.05f,0.05f });
		m_GuardTex[i]->SetRotation({ 0,0,0 });
		m_GuardTex[i]->SetColor({ Action->GetBarrierCol(i).x,
			Action->GetBarrierCol(i).y,Action->GetBarrierCol(i).z
			,Action->GetBarrierAlpha(i) });
		m_GuardTex[i]->Update();
	}
	m_SpellTex->SetPosition( {0,0,-2.f});
	m_SpellTex->SetScale(Action->GetRangeScl());
	m_SpellTex->SetRotation({ 90,0,0 });
	m_SpellTex->SetColor({ 1,1,1,0.5f });
	m_SpellTex->Update();

	m_MeteoTex->SetPosition(Action->GetMeteoPos());
	m_MeteoTex->SetScale({0.2f,0.2f,1.f});
	m_MeteoTex->SetRotation({ 0,0,0 });
	m_MeteoTex->SetColor({ 1,1,1,0.9f });
	m_MeteoTex->Update();

	bomf = Action->GetBom();
	if (bomf) {
		ptime++;
		for (size_t i = 0; i < 1; i++) {
			if (ptime > 10)break;
			const float rnd_pos = 0.01f;
			Vector3 ppos = { 0.f,-2.f,0.f };
			ppos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			ppos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			ppos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

			const float rnd_vel = 2.15f;
			Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			Vector3 acc{};
			const float rnd_acc = 0.006f;
			acc.y = -(float)rand() / RAND_MAX * rnd_acc;

			// 追加
			particle->Add(100, ppos, vel, acc, 10.5f, 0.2f, { 1,0.4f,0.3f }, { 0.9f,0.8f,0.8f }, 0.5, 0);

		}
	}

	if (!bomf)ptime = 0;
		particle->Update();
	//m_HP--;
	//UI_Upda();

	
	HPUiUpda();
	color_rgb.w = std::clamp(color_rgb.w, 0.f, 1.f);
}

void LastBoss::Draw()
{

	if (m_player->GetPos().z > 12.f)return;
	if (color_rgb.w > 0.f) {
	for (size_t i = 0; i < m_Shadow.size(); i++)
	{
		m_Shadow[i]->Draw();
	}
		m_Body->Draw();
	}
	if (m_HP > 0) {
		m_SpellTex->Draw();
		for (size_t i = 0; i < 2; i++) {
			if (m_HoleTex[i] == nullptr)continue;
			m_HoleTex[i]->Draw();
		}
		for (size_t i = 0; i < 4; i++)
		{
			m_FlameTex[i]->Draw();
		}
		if (!Action->GetkillDraw()) {
			for (size_t i = 0; i < 3; i++)
			{
				m_GuardTex[i]->Draw();
			}//UI_Draw();
		}
		m_MeteoTex->Draw();
	}
}

void LastBoss::Draw2()
{
	particle->Draw(DirectXSetting::GetIns()->GetCmdList());
}

void LastBoss::SpriteDraw()
{
	HPUiDraw();
}

void LastBoss::Attack()
{

}

void LastBoss::AddIndex(Model** model, int size)
{
	//アニメーション間隔
	int NextIndInter = 20;
	
	//現在のフレーム
	int NowIndex = animeIndex / NextIndInter;

	if (NowIndex > size - 1) {//最後まで再生されたら最初に戻す
		animeIndex = 0;
	} else {
		if (++animeIndex % NextIndInter == 0) {
			m_Body = Object3d::UniquePtrCreate(model[NowIndex]);
		}
	}
}

void LastBoss::AnimationSett()
{
	switch (Action->GetName())
	{
	case LastBossAct::AnimName::IDLE:
		AddIndex(m_Model_Idle, 4);
		break;

	case LastBossAct::AnimName::SPELL:
		AddIndex(m_Model_Spell, 4);
		break;

	case LastBossAct::AnimName::WALK :
		AddIndex(m_Model_Walk, 4);
		break;

	}
}

void LastBoss::InitAnimatin()
{
	//待機
	for (size_t i = 0; i < 4; i++) {
		m_Model_Idle[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 160.0f }, "wizard_idle.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 1.0f }, { 128.0f, 160.0f });
	}
	//歩き
	for (size_t i = 0; i < 4; i++) {
		m_Model_Walk[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 160.0f }, "wizard_move.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 1.0f }, { 128.0f, 160.0f });
		m_Model_Spell[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 160.0f }, "wizard_chanting.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 1.0f }, { 128.0f, 160.0f });
		m_Model_Idle[i] = Shapes::CreateSquare({ 0, 0 }, {  128.0f, 160.0f }, "wizard_idle.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 1.0f }, { 128.0f, 160.0f });
	
	}
	
}

bool LastBoss::Appear()
{
	Pos_ = Vector3(0, 0, -10);


	return true;
}

void LastBoss::Finalize()
{

}



