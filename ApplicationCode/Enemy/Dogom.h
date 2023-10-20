#pragma once
#include<array>
#include "BossBase.h"
#include"Texture.h"
class Dogom :
    public BossBase
{
private:
    enum Armtag
    {
	    RIGHT,
        LEFT
    };

    std::unique_ptr<Texture> m_Body;
    std::array<std::unique_ptr<Texture>,2> m_Arm;

    //プレイヤーモデル
    Model* BodyModel_;
    Model* ArmModel_[2];

    Vector3 m_BodyPos;
    Vector3 m_BodyRot;
	std::array<Vector3, 2>m_ArmPos;
    std::array<Vector3, 2>m_ArmRot;
    std::array<float, 2>m_ArmMov_Y;
    std::array<float, 2>m_ArmAttckEaseT;
    
    Vector3 BefoPos[2];
    Vector3 PlayerPos;
    float BefoEaseT = 0.f;
    float WinceEaseT;
    float StanCount;
    bool WinceF;

    UINT m_ImpactCout=0;
    UINT m_ActionTimer;
    UINT ActionRandom;
private:
    void Init()override;
    void Upda()override;
    void Draw()override;
    void Draw2()override;
    void Finalize() override;
    void Attack()override;

private:
    enum ArmMove
    {
	    DEFAULT,
        ATTACK_IMPACT,
        ATTACK_CROSS
    }arm_move_;

    enum class Phase_Impact
    {
	    PHASE_1,
        PHASE_2,
        PHASE_3,
        END
    }phase_;

    enum class Phase_Cross
    {
	    PHASE_1,
        PHASE_2,
        PHASE_3,
        END
    }phase_cross_;

    enum class WincePhase
    {
	    PHASE_1,
        PHASE_2,
        END
    }wince_phase_;

    void ResetArmParam() { m_ArmMov_Y[LEFT] = 0.f; m_ArmMov_Y[RIGHT] = 0.f;}

    void SetAttack_Impact() { phase_ = Phase_Impact::PHASE_1; }
    void SetAttack_Cross() { phase_cross_ = Phase_Cross::PHASE_1; }
    void ArmAct();

    void Follow();

    void Wince();
};

