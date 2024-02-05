#include "Togemaru.h"

#include <algorithm>

#include "ExternalFileLoader.h"
#include "Helper.h"
#include "Shapes.h"

#define MapX_Mx 10.f
#define MapX_Mn -10.f

#define MapZ_Mx 8.8f
#define MapZ_Mn -12.f

void Togemaru::Init()
{
	m_SpearsModel = Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "needle.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)0, 0.0f }, { 64.0f, 64.0f });

	for (int32_t i = 0; i < m_SpearArray; i++) {
		m_Spears[i] = Object3d::UniquePtrCreate(m_SpearsModel);
		m_Spears[i]->SetColType(Object3d::CollisionType::Obb);
		m_Spears[i]->SetObjType((int32_t)Object3d::OBJType::Enemy);
		m_Spears[i]->SetObbScl({9.f,9.f,9.f});
		m_Spears[i]->SetHitRadius(0.5f);
		m_Spears[i]->SetScale({0.0f, 0.0f, 0.0f});
	}

	InitAnimatin();
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

		if (word.find("Togemaru") == 0) {
			getline(line_stream, word, ' ');
			m_HP = (int)std::atof(word.c_str());
			getline(line_stream, word, ' ');
			GuardValue = (int)std::atof(word.c_str());
			getline(line_stream, word, ' ');
			AttackValue = (float)std::atof(word.c_str());
			continue;
		}
	}
	BossMaxHP = m_HP;
	names = "Togemaru";
	UI_Init();
	Action = new TogemaruAct();
}

void Togemaru::UI_Init()
{ 
	for (size_t i = 0; i < m_ScaleArray; i++) {
		m_ScaleSizeUI[i] = Object3d::UniquePtrCreate(Shapes::CreateSquare({0, 0}, {64.0f, 64.0f}, "white1x1.png", {64.0f, 64.0f}, {0.f, 0.f}, {64.0f, 0.0f}, {64.0f, 64.0f}));

		ScaleColor[i] = { 1,0,1,1 };
	}
	HPUiInit();
}

void Togemaru::UI_Upda()
{
	//
	//画像サイズ
	constexpr float sizeX = 0.015f,sizeY=0.005f;
	//UI座標X
	float uiposX[] = { Pos_.x + 1.f,Pos_.x,Pos_.x -1.f };
	//補正値
	float corrValX = +0.3f;

	//鉱石の色消すときの画像番号指定用
	int index = m_ScaleArray-Action->GetCrushSpearNum();

	if(Action->GetCrushSpearNum()==0)
	{
		for (size_t i = 0; i < m_ScaleArray; i++) {
			ScaleColor[i].w += 0.05f;
		}
	}
	else
	{
		for (size_t i = index; i < m_ScaleArray; i++) {
			if (Action->GetCrushSpearNum() == 0)break;
			ScaleColor[i].w -= 0.02f;
		}
	}

	for(size_t i=0;i<m_ScaleArray;i++){
		ScaleUI_Pos[i] = { uiposX[i]+corrValX,Pos_.y + 2.5f,Pos_.z };
		ScaleUI_Scl[i] = { sizeX,sizeY,0.1f };

		m_ScaleSizeUI[i]->SetColor(ScaleColor[i]);
		m_ScaleSizeUI[i]->SetPosition(ScaleUI_Pos[i]);
		m_ScaleSizeUI[i]->SetScale(ScaleUI_Scl[i]);
		m_ScaleSizeUI[i]->Update();

		ScaleColor[i].w = std::clamp(ScaleColor[i].w, 0.f, 1.f);
	}
}

void Togemaru::UI_Draw()
{
	if (TogemaruAct::TogemaruDeathF)return;
	for (size_t i = 0; i < m_ScaleArray; i++) {
		m_ScaleSizeUI[i]->Draw();
	}
}


void Togemaru::Upda()
{
	//プレイヤーインスタンスセット
	Action->SetPlayerIns(m_player);
	//行動遷移
	Action->Transition();
	Action->SetHp(m_HP);
	

	Pos_ = Action->GetPos();
	bool judg1 =  m_player->getisHammerActive() && !Action->GetRole();

	
	bool isCol =m_HP>0&& Collision::HitCircle(XMFLOAT2(Pos_.x, Pos_.z + 3.f), 2.f, XMFLOAT2(m_player->GetHammmerPos().x, m_player->GetHammmerPos().z), 1.f);

//	if (!nowcrush) {
		Helper::DamageManager(m_HP, 1, DamF, DamCoolTime, 60, judg1&& isCol);
		Helper::ColKnock(m_player->GetPos(), { Pos_.x,Pos_.y,Pos_.z+3.f }, m_player, judg1 && isCol);
	//}

	if (DamF)FlashF = true;
	RecvDamageFlash();
	//
	AnimationSett();
	//各種パラメータセット
	for (size_t i = 0; i < m_SpearArray; i++) {
		m_Spears[i]->SetRotation(Vector3(90, 0, Action->GetSpearsRot(i)));
		m_Spears[i]->SetScale(Vector3(0.02f, 0.02f, 1.f));
		m_Spears[i]->SetPosition(Action->GetSpearPos(i));//Actクラスから引っ張る
		m_Spears[i]->SetColor(XMFLOAT4(1, 1, 1, Action->GetSpearAlpha(i)));
		m_Spears[i]->Update();
	}

	//本体
	
	m_Body->SetScale(Action->GetScl());
	
	m_Body->SetPosition(Action->GetPos());
	m_Body->SetRotation(Vector3(0, 0, 0));
	m_Body->SetColor(color_rgb);
	m_Body->Update();

	UI_Upda();
	HPUiUpda();

	if(m_HP<=0)
	{
		m_ClearF = true;
	}
}


void Togemaru::Draw()
{
	//針描画
	//if (Action->GetName() != TogemaruAct::AnimeName::ROLE) {
		for (size_t i = 0; i < m_SpearArray; i++)
		{
			if (Action->GetSpearPos(i).x<MapX_Mx && Action->GetSpearPos(i).x > MapX_Mn &&
				Action->GetSpearPos(i).z < MapZ_Mx && Action->GetSpearPos(i).z > MapZ_Mn) {
				m_Spears[i]->Draw();
			}
		//}
	}
	m_Body->Draw();

	UI_Draw();
}

void Togemaru::Draw2()
{
	
}

void Togemaru::SpriteDraw()
{
	HPUiDraw();
}

void Togemaru::Attack()
{
	
}

void Togemaru::AddIndex(Model** model, int size)
{
	//アニメーション間隔
	int NextIndInter = 30;
	if (Action->GetName() == TogemaruAct::AnimeName::ROLE){
		NextIndInter = 5;
	}
	else{
		NextIndInter = 30;
	}
	//現在のフレーム
	int NowIndex = animeIndex / NextIndInter;
	
	if (NowIndex > size-1){//最後まで再生されたら最初に戻す
		animeIndex = 0;
	}
	else {
		if (++animeIndex % NextIndInter == 0){
			m_Body = Object3d::UniquePtrCreate(model[NowIndex]);
		}
	}
}

void Togemaru::AnimationSett()
{
	switch (Action->GetName())
	{
	case TogemaruAct::AnimeName::IdlE:
		AddIndex(m_Model_Idle, 4);
		break;

	case TogemaruAct::AnimeName::WALK_FRONT:
		AddIndex(m_Model_Walk, 4);
		break;

	case TogemaruAct::AnimeName::WALK_RIGHT:
		AddIndex(m_Model_Walk_Right, 4);
		break;

	case TogemaruAct::AnimeName::WALK_LEFT:
		AddIndex(m_Model_Walk_Left, 4);
		break;

	case TogemaruAct::AnimeName::WALK_BACK:
		AddIndex(m_Model_Walk_Back, 4);
		break;

		case TogemaruAct::AnimeName::ROLE:
		AddIndex(m_Model_Role, 2);
		break;

	case TogemaruAct::AnimeName::CRUSH_FRONT:
		AddIndex(m_Model_Crush, 4);
		break;

	case TogemaruAct::AnimeName::CRUSH_RIGHT:
		AddIndex(m_Model_Crush_Right, 4);
		break;

	case TogemaruAct::AnimeName::CRUSH_LEFT:
			AddIndex(m_Model_Crush_Left, 4);
			break;

	case TogemaruAct::AnimeName::CRUSH_BACK:
				AddIndex(m_Model_Crush_Back, 4);
				break;
	}
}

void Togemaru::InitAnimatin()
{
	//待機
	for (size_t i = 0; i < 4; i++) {
		m_Model_Idle[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_idle.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
	//歩き
	for (size_t i = 0; i < 4; i++) {
		m_Model_Walk[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_move.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
		m_Model_Walk_Right[i]= Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "togemaru_moveRL.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
		m_Model_Walk_Left[i]= Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "togemaru_moveLR.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
		m_Model_Walk_Back[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 160.0f }, "togemaru_moveBack.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 160.0f });

	}
	//突進
	for (size_t i = 0; i < 2; i++) {
		m_Model_Role[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_rot.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
	for(size_t i=0;i<4;i++){
		m_Model_Crush[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_weekMove.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
		m_Model_Crush_Right[i] = Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "togemaru_weekMoveLR.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
		m_Model_Crush_Left[i] = Shapes::CreateSquare({ 10, 0 }, { 192.0f, 128.0f }, "togemaru_weekMoveRL.png", { 192.0f, 64.0f }, { 0.5f, 0.5f }, { 192.0f * (float)i, 10.0f }, { 192.0f, 128.0f });
		m_Model_Crush_Back[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_weekMove.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });

	}
}

bool Togemaru::Appear()
{
	Pos_ = Vector3(0,0,-10);


	return true;
}

void Togemaru::Finalize()
{
	
}



