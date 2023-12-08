#pragma once
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

private:
    /* ジャンプ */
    std::unique_ptr<Object3d>m_ImpactTex = nullptr;
    float m_JumpPower_ = 1.f;
    float m_JumpInter = 0.f;
	void Jump();
    /*         */

};

