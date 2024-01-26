#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:

public:
    /** 初期化 **/
    void Init() override;
    /** 更新 **/
    void Upda(Camera*camera) override;
    /** 描画 **/
    void Draw() override;
    /** あにめーしょん **/
	void TextureAnimation() override;

    void AttackAction() override;

    void TutorialUpda(Camera* camera, bool flag) override;

    void TutorialDraw(float Mindis)override;

    void SetPosDeb(Vector3 pos)override;

    void TexDraw(XMFLOAT3 Pos) override;
private:
    /*じゃんぷ*/
	float JFrame;
    void Jump();
    /*こうげき*/
    void Attack();

    void ClampMap();
    uint16_t m_KnockCoolT = 0;
    bool m_knockF = FALSE;
};

