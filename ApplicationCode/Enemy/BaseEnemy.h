#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>

using namespace DirectX;

class BaseEnemy
{
protected:
	/*�G�̊e�p�����[�^*/
	struct Status
	{
		//�̗�
		unsigned int HP;
		//�U���l
		unsigned int DamageValue;
		//��_���[�W��̋����莞��
		float KnockTime;
		//�ړ��X�s�[�h
		float MoveSpeed;
		//2D
		std::shared_ptr<Texture>Tex;
		//���W�E��]�E�X�P�[��
		XMFLOAT3 Pos,Rot,Scl;
	}_status;

	
public:
	/** ������ **/
	virtual void Init()=0;

	/** �X�V **/
	virtual void Upda(Camera*camera)=0;

	/** �`�� **/
	virtual void Draw()=0;

public:
	bool DeathJudg();

public:
	unsigned int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;
};

