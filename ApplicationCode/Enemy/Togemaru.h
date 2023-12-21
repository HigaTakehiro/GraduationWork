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

    //アニメーション関連
    void InitAnimatin();
    void AnimationSett();
    void AddIndex(Model**model,int size);
private:
    //針の数
	static constexpr UINT m_SpearArray = 8;
    //鱗の数
    static constexpr UINT m_ScaleArray = 3;
    //
    static constexpr UINT m_TexArray = 8;
    //
private:
    //本体
    std::unique_ptr<Object3d>m_Body=nullptr;

   Model*m_Model_Idle[4] = {};
   Model*m_Model_Walk[4] = {};
	Model*m_Model_Role[2] = {};
    Model* m_Model_Crush[4] = {};
	//アニメーションカウンタ
	int animeIndex = 0;

    //針OBJ
    std::array < std::unique_ptr<Object3d>,m_SpearArray>m_Spears;
    Model*m_SpearsModel={};
    std::array<float, m_SpearArray>m_SpearAngle = {};

    //鱗耐久
    std::array<uint32_t, m_ScaleArray>m_ScaleHp = { 1,1,1 };

    //ボスの行動パターン
    TogemaruAct* Action=nullptr;

private:
    //UI
	//鱗の数
    std::array<std::unique_ptr<Object3d>,m_ScaleArray>m_ScaleSizeUI = {};
    std::array < Vector3, m_ScaleArray> ScaleUI_Pos = {};
    std::array < Vector3, m_ScaleArray> ScaleUI_Scl = {};
    std::array < XMFLOAT4, m_ScaleArray> ScaleColor = {};
    //
	void UI_Init();
    void UI_Upda();
    void UI_Draw();
};

