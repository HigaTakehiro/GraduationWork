#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:
    /** 初期化 **/
    void Init() override;
    /** 更新 **/
    void Upda(Camera*camera) override;
    /** 描画 **/
    void Draw() override;

private:

};

