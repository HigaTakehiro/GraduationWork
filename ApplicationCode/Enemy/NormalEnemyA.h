#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:
    ~NormalEnemyA();
public:
    /** 初期化 **/
    void Init() override;
    /** 更新 **/
    void Upda(Camera*camera) override;
    /** 描画 **/
    void Draw() override;
    /** あにめーしょん **/
	void TextureAnimation() override;
private:
    float JFrame;
    void Jump();
};

