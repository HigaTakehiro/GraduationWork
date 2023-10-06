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
    float JFrame;
    void Jump();
    void Attack();
};

