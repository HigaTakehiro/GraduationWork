#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"CollisionPrimitive.h"
#include "Player.h"
//#include
using namespace DirectX;

class BaseEnemy
{
public:
protected:

	/*�G�̊e�p�����[�^*/
	//*************************************

	struct Status
	{
		//�̗�
		int HP;
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
		/*
		std::shared_ptr<Object3d>Tex;
		std::array<Model*, 5>TexModel;
		size_t TexSize;
		 */
		 //���W�E��]�E�X�P�[��
		XMFLOAT3 Pos, Rot, Scl;
		OBB Obb;
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
	BaseAction _action = IDLE;

	static void(BaseEnemy::* stateTable[])();

	void Idle(),
		Walk(),
		Follow(),
		Knock(),
		Attack(),
		Death();

	void MoveDirection();
	//**************************************
	
	//�A�j���[�V�����֘A
	int AnimTim;
	unsigned int AnimationCount;
	unsigned int AnimationInterval;
	bool RecvDamage;
	std::shared_ptr<Player>_player;
	OBB _playerOBB;

	bool _isFlash;
	bool _isAttack;
	float t = 0.0f;
	int FlashCount;
	float back_t;
	XMFLOAT4 _color;
private:

	//�U���󂯂����H
	
public:
	/** ������ **/
	virtual void Init() = 0;

	/** �X�V **/
	virtual void Upda(Camera* camera) = 0;

	/** �`�� **/
	virtual void Draw() = 0;

	/** �A�j���[�V���� **/
	virtual void TextureAnimation() = 0;

	virtual void AttackAction()=0;
public:
	bool DeathJudg();

	void CollideHummmer();

	void RecvFlashColor();
	//�v���C���[�̃C���X�^���X�����p���@���ƂŒ�������
	void SetPlayerIns(Player* player) { _player.reset(player); }
	void SetHammerObb(OBB obb) { _playerOBB=obb; }

private:
	void RotforPlayer();
public:
	unsigned int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;
};

