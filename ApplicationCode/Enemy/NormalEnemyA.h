#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:
    /** ‰Šú‰» **/
    void Init() override;
    /** XV **/
    void Upda(Camera*camera) override;
    /** •`‰æ **/
    void Draw() override;

private:

};

