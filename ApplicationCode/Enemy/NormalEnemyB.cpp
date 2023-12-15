#include "NormalEnemyB.h"

#include <algorithm>
#include "Shapes.h"

void NormalEnemyB::Init()
{
	state_obj_.TexSize_ = 6;
	state_obj_.Model_.resize(state_obj_.TexSize_);

	for (size_t i = 0; i < state_obj_.TexSize_; i++) {
		state_obj_.Model_[i] = Shapes::CreateSquare({ 0, 0 }, { 64.0f, 64.0f }, "rockmuni_idle.png", { 96.0f, 96.0f }, { 0.5f, 0.5f }, { 64.0f * (float)i, 2.0f }, { 64.0f, 64.0f });
	}
	state_obj_.obj_ = Object3d::UniquePtrCreate(state_obj_.Model_[m_AnimationCount]);

	state_obj_.obj_->SetColType(Object3d::CollisionType::Obb);
	state_obj_.obj_->SetObjType((int32_t)Object3d::OBJType::Enemy);
	state_obj_.obj_->SetObbScl({ 1.f,1.f,1.f });
	state_obj_.obj_->SetHitRadius(0.5f);
	state_obj_.Scl={ 0.02f, 0.02f, 0.02f };

	Tag_ = "Munni";
	action_ = new MunniAction();

}

void NormalEnemyB::Upda(Camera* camera)
{
	action_->SetPlayer(_player.get());
	//行動遷移
	action_->AttackTransition();
	state_obj_.Pos_ = action_->GetPos();
	state_obj_.Rot_ = action_->GetRor();

	// アニメーションのカウンタ
	{
		//アニメーション間隔
		constexpr uint32_t animationInter = 30;
		//インデクス
		int animeTexIndx = m_AnimationCount / animationInter;

		if (++m_AnimationCount % animationInter == 0) {
			// 最後まで行ったら０番に
			if (animeTexIndx > (state_obj_.TexSize_ - 1)) {
				m_AnimationCount = 0;
			} else {
				//テクスチャ切り替え30フレーム毎
				state_obj_.obj_ = Object3d::UniquePtrCreate(state_obj_.Model_[animeTexIndx]);
			}
		}
		//インデックス範囲制限
		animeTexIndx= std::clamp(animeTexIndx, 0, state_obj_.TexSize_-1);
	}


	//各種パラメータセット 更新
	//state_obj_.obj_->SetRotation(state_obj_.Rot_);
	state_obj_.obj_->SetScale(state_obj_.Scl);
	state_obj_.obj_->SetPosition(state_obj_.Pos_);
	state_obj_.obj_->SetColor(state_obj_.Color_);
	state_obj_.obj_->SetIsBillboardY(true);
	state_obj_.obj_->Update();
}


#include "DirectXSetting.h"
void NormalEnemyB::Draw()
{
	Object3d::PreDraw(DirectXSetting::GetIns()->GetCmdList());
	action_->ImpTexDraw();
	state_obj_.obj_->Draw();
	Object3d::PostDraw();
}

void NormalEnemyB::Jump()
{
	float SubPower = 0.001f;
	//落下の緩急
	constexpr float Distortion = 1.f;
	//地面の高さ
	constexpr float GroundY = -2.5f;
	//ジャンプ高さ
	constexpr float Height = 0.6f;
	/* 変換 */
	constexpr float pi = 3.14f;
	//ドッスン挙動

	m_JumpInter_ += 1.f / 60.f;
	_status.Pos.y = GroundY + (1.0f - pow(1.0f - sin(pi * m_JumpInter_), Distortion)) * Height;

	m_JumpInter_ = std::clamp(m_JumpInter_, 0.f, 1.f);
}

void NormalEnemyB::AttackAction()
{

}

void NormalEnemyB::TextureAnimation()
{

}

void NormalEnemyB::TutorialUpda(Camera* camera, bool flag)
{

}
void NormalEnemyB::TutorialDraw(float Mindis)
{

}

void NormalEnemyB::SetPosDeb(Vector3 pos)
{
	//行動遷移クラスに送る座標(やり方少し汚いけど)
	action_->SetInitPos(state_obj_.Pos_);
}

