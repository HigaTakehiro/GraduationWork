#pragma once
#include "BossBase.h"
class Dogom :
    public BossBase
{
private:

private:
    void Init()override;
    void Upda()override;
    void Draw()override;

    void Attack()override;
};

