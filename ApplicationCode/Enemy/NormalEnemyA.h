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
private:
    float JFrame;
    void Jump();
    void Attack();
};

