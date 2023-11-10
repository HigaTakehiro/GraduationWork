#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:

public:
    /** ������ **/
    void Init() override;
    /** �X�V **/
    void Upda(Camera*camera) override;
    /** �`�� **/
    void Draw() override;
    /** ���ɂ߁[����� **/
	void TextureAnimation() override;

    void AttackAction() override;
private:
    /*������*/
	float JFrame;
    void Jump();
    /*��������*/
    void Attack();
    uint16_t m_KnockCoolT = 0;
    bool m_knockF = FALSE;
};

