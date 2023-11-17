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

    std::unique_ptr<Object3d> m_Body;
    std::unique_ptr<Object3d>CrossAreaTex;
    std::array<std::unique_ptr<Object3d>, 2>m_ShadowTex={};
    float m_CrossAreaAlpha = 0.f;
    std::array<std::unique_ptr<Object3d>,2> m_Arm;
    std::array<std::unique_ptr<Object3d>, 2> m_ImpactTex;
    std::unique_ptr<Sprite>m_HpTex=nullptr;
    std::array<std::unique_ptr<Object3d>,2>m_ArmHpTex{};
    //プレイヤーモデル
    Model* BodyModel_[8];
    Model* ArmModel_[8];
    Vector3 BodyScl={0.1f,0.2f,0.2f};
    Vector3 m_BodyPos;
    Vector3 oldBodyPos = Vector3(0, 0, 0);
    Vector3 m_BodyRot;
	std::array<Vector3, 2>m_ArmPos;
    std::array<Vector3, 2>m_ArmRot;
    std::array<Vector3, 2>m_ImpactTexPos;
    std::array<Vector3, 2>m_ImpactTexScl;
    std::array<float, 2>m_ImpactTexAlpha;
    std::array<float, 2>m_ArmAlpha;;
	std::array<float, 2>m_ArmMov_Y;
    std::array<float, 2>m_ArmAttckEaseT;
	std::array<int, 2>m_ArmHp={6,6};
    std::array<int, 2>m_ArmHp_Max = { 6,6 };
    std::array<bool, 2>m_ArmDamF;

    std::array<bool, 2>m_ImpactF;
    std::array<Vector3, 3>WaitPos = { Vector3(0,0,-20),Vector3(-18,0,0),Vector3(18,0,0) };
    Vector3 BefoPos[2];
    Vector3 PlayerPos;
    float BefoEaseT = 0.f;
    float WinceEaseT;
    float StanCount;
    float MovingAngle;
    float CrossWaitCount = 0.f;
	bool WinceF;
    bool isLeaveBoss;
    float nextAngle = 180.f;
    int randAct = 0;

    UINT m_ImpactCout=0;
    UINT m_ActionTimer=1;
    UINT ActionRandom=1;
    UINT BossBodyMovingT=1;
private:
    void Init()override;
    void Upda()override;
    void Draw()override;
    void Draw2()override;
    void Finalize() override;
    void Attack()override;
    void SpriteDraw()override;
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
        IDLE,
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

    std::array<bool, 2>ColF;
    std::array<int, 2>DamCool;
    void CoollisionFace();
    void CoollisionArm();
    uint16_t ArmHP();

    void FaceFall();
    bool m_FallF = false;
    void ShakeArm(Vector3& Defopos, float& time);
    void MoveBody();

    float m_FaceRotEaseT;
    bool m_FaceRotaF;
    uint16_t isRotateTim = 1;
	float MorP_Sign = 1.f;
    void RotationFace(const uint16_t& interval);

    void ImpactTexScling();
    void ImpactKnock();
    void FaceCol();
    uint16_t l_t = 0, winceIdleT=0;
    void WinceIdle();
    Vector3 vec[2] = {};

    float t = 0,t2=0,t3=0,t4=0;
    bool movF;
    float OldMovAngle;
    float BodyMoveEase;

private:
    Vector3 ShadowScl(float YPos);
};

