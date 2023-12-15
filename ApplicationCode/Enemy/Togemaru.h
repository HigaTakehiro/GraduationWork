#pragma once
#include "BossBase.h"
#include<array>
#include"TogemaruAct.h"
class Togemaru :
    public BossBase
{
private:
    void Init()override;
    void Upda()override;
    void Draw()override;
    void Draw2()override;
    void Finalize() override;
    void Attack()override;
    void SpriteDraw()override;
    bool Appear()override;

private:
    //�j�̐�
	static constexpr UINT m_SpearArray = 8;
    //�؂̐�
    static constexpr UINT m_ScaleArray = 3;
    //
    static constexpr UINT m_TexArray = 8;
private:
    //�{��
    std::unique_ptr<Object3d>m_Body=nullptr;
    std::array<Model*, m_TexArray>m_Model = {};
    //�A�j���[�V�����J�E���^
	int animeIndex = 0;

    //�jOBJ
    std::array < std::unique_ptr<Object3d>,m_SpearArray>m_Spears;
    Model*m_SpearsModel={};
    std::array<float, m_SpearArray>m_SpearAngle = {};

    //�ؑϋv
    std::array<uint32_t, m_ScaleArray>m_ScaleHp = { 1,1,1 };

    //�e��p�����[�^
    Vector3 Pos_, Rot_, Scl_;

    TogemaruAct* Action=nullptr;
};
