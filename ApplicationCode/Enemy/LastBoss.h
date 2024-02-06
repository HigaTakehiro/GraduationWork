#pragma once
#include "BossBase.h"
#include "LastBossAct.h"
#include "ParticleManager.h"

class LastBoss :
    public BossBase
{
public:
private:
    void Init()override;
    void Upda()override;
    void Draw()override;
    void Draw2()override;
    void Finalize() override;
    void Attack()override;
    void AddIndex(Model** model, int size);
    void AnimationSett();
    void SpriteDraw()override;
    void InitAnimatin();
    bool Appear()override;

private:
    int holeanimtime;
    int index;
    int animeIndex;
    Model* m_Model_Hole[3] = {};
	Model* m_Model_Idle[4] = {};
       Model* m_Model_Walk[4] = {};
       Model* m_Model_Walk_Left[4] = {};
       Model* m_Model_Walk_Right[4] = {};
       Model* m_Model_Walk_Back[4] = {};
       Model* MeteoModel[2];
       int meteoanim;
    std::array<std::unique_ptr<Object3d>, 2>m_HoleTex;
    std::array<float, 2>m_HoleRot;
    std::array<std::unique_ptr<Object3d>, 4>m_FlameTex;
    std::array<std::unique_ptr<Object3d>, 3>m_GuardTex;
    std::unique_ptr<Object3d>m_SpellTex;
    std::unique_ptr<Object3d>m_MeteoTex;
	std::unique_ptr<Object3d>m_Body;
    BOOL DamF; int DamCoolTime;
    LastBossAct* Action;
    std::unique_ptr<ParticleManager>particle;
    bool bomf;
    int ptime;
public:
    
};

