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
    /* �W�����v */
    std::unique_ptr<Object3d>m_ImpactTex = nullptr;
    float m_JumpPower_ = 1.f;
    float m_JumpInter = 0.f;
	void Jump();
    /*         */

};

