#include "Togemaru.h"

#include "Shapes.h"

void Togemaru::Init()
{
	m_SpearsModel = Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "impact.png", { 64.0f, 64.0f }, { 0.5f, 0.5f }, { 64.0f * (float)0, 0.0f }, { 64.0f, 64.0f });

	for (int32_t i = 0; i < m_SpearArray; i++) {
		m_Spears[i] = Object3d::UniquePtrCreate(m_SpearsModel);
		m_Spears[i]->SetColType(Object3d::CollisionType::Obb);
		m_Spears[i]->SetObjType((int32_t)Object3d::OBJType::Enemy);
		m_Spears[i]->SetObbScl({9.f,9.f,9.f});
		m_Spears[i]->SetHitRadius(0.5f);
		m_Spears[i]->SetScale({0.0f, 0.0f, 0.0f});
	}

	m_Body = Object3d::UniquePtrCreate(m_Model_Idle[0]);
	m_Body->SetColType(Object3d::CollisionType::Obb);
	m_Body->SetObjType((int32_t)Object3d::OBJType::Enemy);
	m_Body->SetObbScl({ 9.f,9.f,9.f });
	m_Body->SetHitRadius(0.5f);
	m_Body->SetScale({ 0.040f, 0.040f, 0.040f });

	Action = new TogemaruAct();
}

void Togemaru::Upda()
{
	//�v���C���[�C���X�^���X�Z�b�g
	Action->SetPlayerIns(m_player);
	//�s���J��
	Action->Transition();
	//
	AnimationSett();
	//�e��p�����[�^�Z�b�g
	for (size_t i = 0; i < m_SpearArray; i++) {
		m_Spears[i]->SetRotation(Vector3(90, 0, 0));
		m_Spears[i]->SetScale(Vector3(0.02f, 0.02f, 1.f));
		m_Spears[i]->SetPosition(Action->GetSpearPos(i));//Act�N���X�����������
		m_Spears[i]->SetColor(XMFLOAT4(1, 1, 1, Action->GetSpearAlpha()));
		m_Spears[i]->Update();
	}

	//�{��
	m_Body->SetScale({ 0.040f, 0.040f, 0.040f });
	m_Body->SetPosition(Action->GetPos());
	m_Body->SetRotation(Vector3(0, 0, 0));
	m_Body->Update();
}


void Togemaru::Draw()
{
	//�j�`��
	for(size_t i=0;i<m_SpearArray;i++)
	{
		m_Spears[i]->Draw();
	}
	m_Body->Draw();
}

void Togemaru::Draw2()
{
	
}

void Togemaru::SpriteDraw()
{
	
}

void Togemaru::Attack()
{
	
}

void Togemaru::AddIndex(Model** model, int size)
{
	//�A�j���[�V�����Ԋu
	constexpr int NextIndInter = 30;
	//���݂̃t���[��
	int NowIndex = animeIndex / NextIndInter;

	if (NowIndex > size-1){//�Ō�܂ōĐ����ꂽ��ŏ��ɖ߂�
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

	case TogemaruAct::AnimeName::WALK:
		AddIndex(m_Model_Walk, 4);
		break;

	case TogemaruAct::AnimeName::ROLE:
		AddIndex(m_Model_Role, 2);
		break;
	}
}

void Togemaru::InitAnimatin()
{
	//�ҋ@
	for (size_t i = 0; i < 4; i++) {
		m_Model_Idle[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_idle.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
	//����
	for (size_t i = 0; i < 4; i++) {
		m_Model_Walk[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_move.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
	//�ːi
	for (size_t i = 0; i < 2; i++) {
		m_Model_Role[i] = Shapes::CreateSquare({ 0, 0 }, { 128.0f, 128.0f }, "togemaru_rot.png", { 128.0f, 64.0f }, { 0.5f, 0.5f }, { 128.0f * (float)i, 0.0f }, { 128.0f, 128.0f });
	}
}

bool Togemaru::Appear()
{
	return true;
}

void Togemaru::Finalize()
{
	
}



