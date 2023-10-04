#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>
//#include
using namespace DirectX;

class BaseEnemy
{
protected:

	/*�G�̊e�p�����[�^*/
	//*************************************
	
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
		//���G���a
		float SearchRange;
		//2D
		std::shared_ptr<Texture>Tex;
		size_t TexSize;
		//���W�E��]�E�X�P�[��
		XMFLOAT3 Pos,Rot,Scl;
	}_status;
	//**************************************

	/*�s��*/
	//**************************************
	enum BaseAction
	{
		IDLE,
		WALK,
		FOLLOW,
		KNOCK,
		ATTACK,
		DEATH
	};
	BaseAction _action=IDLE;

	static void(BaseEnemy::* stateTable[])();

	void Idle(),
		 Walk(),
		 Follow(),
		 Knock(),
		 Attack(),
		 Death();

	void MoveDirection();
	//**************************************

	//�v���C���[
	std::shared_ptr<XMFLOAT3>_player;

	//�A�j���[�V�����֘A
	int AnimTim;
	unsigned int AnimationCount;
	unsigned int AnimationInterval;

	//�U���󂯂����H
	bool RecvDamage;

public:
	/** ������ **/
	virtual void Init()=0;

	/** �X�V **/
	virtual void Upda(Camera*camera)=0;

	/** �`�� **/
	virtual void Draw()=0;

	/** �A�j���[�V���� **/
	virtual void TextureAnimation()=0;
public:
	bool DeathJudg();

public:
	unsigned int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;
};

