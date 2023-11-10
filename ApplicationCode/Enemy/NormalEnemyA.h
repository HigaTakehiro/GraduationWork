#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:

public:
    /** ‰Šú‰» **/
    void Init() override;
    /** XV **/
    void Upda(Camera*camera) override;
    /** •`‰æ **/
    void Draw() override;
    /** ‚ ‚É‚ß[‚µ‚å‚ñ **/
	void TextureAnimation() override;

    void AttackAction() override;
private:
    /*‚¶‚á‚ñ‚Õ*/
	float JFrame;
    void Jump();
    /*‚±‚¤‚°‚«*/
    void Attack();
    uint16_t m_KnockCoolT = 0;
    bool m_knockF = FALSE;
};

