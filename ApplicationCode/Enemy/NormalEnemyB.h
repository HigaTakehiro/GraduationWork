#pragma once
#include "BaseEnemy.h"
class NormalEnemyB :
    public BaseEnemy
{
public:
    /** ������ **/
    void Init() override;
    /** �X�V **/
    void Upda(Camera* camera) override;
    /** �`�� **/
    void Draw() override;
    /** ���ɂ߁[����� **/
    void TextureAnimation() override;

    void AttackAction() override;

    void TutorialUpda(Camera* camera, bool flag) override;

    void TutorialDraw(float Mindis)override;

private:
    /* JUMP */
    std::unique_ptr<Object3d>m_ImpactTex = nullptr;
    float m_JumpInter_ = 0.f;
    float m_JumpPower = 1.f;
    void Jump();
    /*      */
};

