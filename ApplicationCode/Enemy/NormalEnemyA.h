#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:
    /** ������ **/
    void Init() override;
    /** �X�V **/
    void Upda(Camera*camera) override;
    /** �`�� **/
    void Draw() override;

private:

};

