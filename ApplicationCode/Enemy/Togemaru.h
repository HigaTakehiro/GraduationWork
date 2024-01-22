#pragma once
#include "BossBase.h"
#include<array>
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

    //�A�j���[�V�����֘A
    void InitAnimatin();
    void AnimationSett();
    void AddIndex(Model**model,int size);
private:
    //�j�̐�
	static constexpr UINT m_SpearArray = 8;
    //�؂̐�
    static constexpr UINT m_ScaleArray = 3;
    //
    static constexpr UINT m_TexArray = 8;
    //
private:
    //�{��
    std::unique_ptr<Object3d>m_Body=nullptr;

   Model*m_Model_Idle[4] = {};
   Model*m_Model_Walk[4] = {};
   Model* m_Model_Walk_Left[4] = {};
   Model* m_Model_Walk_Right[4] = {};
   Model* m_Model_Walk_Back[4] = {};
	Model*m_Model_Role[2] = {};
    Model* m_Model_Crush[4] = {};
    Model* m_Model_Crush_Left[4] = {};
    Model* m_Model_Crush_Right[4] = {};
    Model* m_Model_Crush_Back[4] = {};
	//�A�j���[�V�����J�E���^
	int animeIndex = 0;

    //�jOBJ
    std::array < std::unique_ptr<Object3d>,m_SpearArray>m_Spears;
    Model*m_SpearsModel={};
    std::array<float, m_SpearArray>m_SpearAngle = {};
    std::array<float, m_SpearArray>m_SpearRot = {};

    //�ؑϋv
    std::array<uint32_t, m_ScaleArray>m_ScaleHp = { 1,1,1 };

    
    BOOL DamF;
    int DamCoolTime;
private:
    //UI
	//�؂̐�
    std::array<std::unique_ptr<Object3d>,m_ScaleArray>m_ScaleSizeUI = {};
    std::array < Vector3, m_ScaleArray> ScaleUI_Pos = {};
    std::array < Vector3, m_ScaleArray> ScaleUI_Scl = {};
    std::array < XMFLOAT4, m_ScaleArray> ScaleColor = {};
    //
	void UI_Init();
    void UI_Upda();
    void UI_Draw();
};

