#pragma once
#include "BaseEnemy.h"
class NormalEnemyA :
    public BaseEnemy
{
public:
    ~NormalEnemyA();
public:
    /** ������ **/
    void Init() override;
    /** �X�V **/
    void Upda(Camera*camera) override;
    /** �`�� **/
    void Draw() override;
    /** ���ɂ߁[����� **/
	void TextureAnimation() override;
private:
    float JFrame;
    void Jump();
};

