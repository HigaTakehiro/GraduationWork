#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:

public:
    /** èâä˙âª **/
    void Init() override;
    /** çXêV **/
    void Upda(Camera*camera) override;
    /** ï`âÊ **/
    void Draw() override;
    /** Ç†Ç…ÇﬂÅ[ÇµÇÂÇÒ **/
	void TextureAnimation() override;

    void AttackAction() override;
private:
    /*Ç∂Ç·ÇÒÇ’*/
	float JFrame;
    void Jump();
    /*Ç±Ç§Ç∞Ç´*/
    void Attack();

    void ClampMap();
    uint16_t m_KnockCoolT = 0;
    bool m_knockF = FALSE;

    XMFLOAT2 mapOverX = { 23.f,39.f };
    XMFLOAT2 mapOverZ = { -5.f,49.f };

};

