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
		std::shared_ptr<Texture>Tex,TexBack,TexRight,TexLeft;
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
	bool PlayerRecv;
	float t = 0.0f;
	int FlashCount;
	float back_t;
	XMFLOAT4 _color;

	int m_MaxHp=0;
	std::unique_ptr<Object3d>m_HpTex;
	std::unique_ptr<Object3d>m_ShadowTex;

	XMFLOAT3 OverPosMax, OverPosMin;

	bool FlashF; float val=1;
	//XMFLOAT4 color;
public:
	void SetFlash(bool f) { FlashF = f; }
	bool GetRecv() { return _isFlash; }
	void DamageFlash();
protected:
	void TexInit();
	void TexUpda();
public:
	void TexDraw();

	void TutorialTexDraw();

	void SetOverPos(XMFLOAT3 max, XMFLOAT3 min) { OverPosMax = max; OverPosMin = min; }
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

	virtual void TutorialUpda(Camera* camera,bool flag) = 0;

	virtual void TutorialDraw(float Mindis) = 0;
public:
	bool DeathJudg();

	void CollideHummmer();

	void RecvFlashColor();
	//�v���C���[�̃C���X�^���X�����p���@���ƂŒ�������
	void SetPlayerIns(Player* player) { _player.reset(player); }
	void SetHammerObb(OBB obb) { _playerOBB=obb; }

	inline void GetDamage()
	{
		if (!FlashF)FlashF = TRUE;
		_status.HP--;
		if (!_isAttack){ 
			RecvDamage = TRUE;
		}
	}
private:
	void RotforPlayer();
public:
	unsigned int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;

public:
	void SetPos(Vector3 pos) { _status.Pos = pos; }
};

