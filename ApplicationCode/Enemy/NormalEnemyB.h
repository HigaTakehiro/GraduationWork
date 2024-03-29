#pragma once

#include "MunniAction.h"
#include "BaseEnemy.h"
class NormalEnemyB :
    public BaseEnemy
{
public:
    /** 初期化 **/
    void Init() override;
    /** 更新 **/
    void Upda(Camera* camera) override;
    /** 描画 **/
    void Draw() override;
    /** あにめーしょん **/
    void TextureAnimation() override;

    void AttackAction() override;

    void TutorialUpda(Camera* camera, bool flag) override;

    void TutorialDraw(float Mindis)override;

    void SetPosDeb(Vector3 pos)override;

    void TexDraw() override;
private:
    /* JUMP */
    std::unique_ptr<Object3d>m_ImpactTex = nullptr;
    float m_JumpInter_ = 0.f;
    float m_JumpPower = 1.f;
    void Jump();

    /* ANIMATION */
    uint32_t m_AnimationCount = 0;
    //向きの状態
    enum AState{FRONT,BACK,RIGHT,LEFT}animeState_;

	//行動遷移用
    MunniAction* action_=nullptr;
};

