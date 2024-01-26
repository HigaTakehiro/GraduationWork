#pragma once
#include"Object3d.h"
#include"Texture.h"
#include<memory>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"CollisionPrimitive.h"
#include "MunniAction.h"
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
		int HP = 1;
		//�U���l
		unsigned int DamageValue;
		//��_���[�W��̋����莞��
		float KnockTime;
		//�ړ��X�s�[�h
		float MoveSpeed;
		//���G���a
		float SearchRange;
		//2D
		std::shared_ptr<Texture>Tex, TexBack, TexRight, TexLeft;
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

	/* OBJ */
	struct StateObj
	{
		/*���f��*/
		std::unique_ptr<Object3d>obj_ = {};
		/* �摜���� */
		int TexSize_ = 4;
		/* ���f���摜 */
		std::vector<Model*>Model_ = {};
		/* �̗� */
		int Hp_ = 10;
		//�U���l*/
		unsigned int DamageValue_ = 0;
		/* ��_���[�W��̋����莞�� */
		float KnockTime_ = 0.f;
		/* �ړ��X�s�[�h */
		float MoveSpeed_ = 0.f;
		/* ���G���a */
		float SearchRange_ = 0.f;
		/* �e�p�����[�^ */
		Vector3 Pos_ = {}, Rot_ = {}, Scl = {};
		/* �F */
		XMFLOAT4 Color_ = { 1,1,1,1 };
		/*  */
		bool DamCool = FALSE;
	}state_obj_;
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

	int m_MaxHp = 0;
	std::unique_ptr<Object3d>m_HpTex;
	std::unique_ptr<Object3d>m_ShadowTex;

	XMFLOAT3 OverPosMax, OverPosMin;

	bool FlashF; float val = 1;
	std::string Tag_;
	//XMFLOAT4 color;

	int count_;
public:
	void SetFlash(bool f) { FlashF = f; }
	bool GetFlash() { return FlashF; }
	bool GetRecv() { return _isFlash; }
	void DamageFlash();

	void PlayerHitBody(float dis,bool& f);
	void DestryAct(float&alpha,int hp);
	float aTime;
	int32_t RefTime;

protected:
	void TexInit();
	void TexUpda();
public:
	virtual void TexDraw()=0;

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

	virtual void AttackAction() = 0;

	virtual void TutorialUpda(Camera* camera, bool flag) = 0;

	virtual void TutorialDraw(float Mindis) = 0;


public:
	bool DeathJudg();

	void CollideHummmer();

	void RecvFlashColor();
	//�v���C���[�̃C���X�^���X�����p���@���ƂŒ�������
	void SetPlayerIns(Player* player) { _player.reset(player); }
	void SetHammerObb(OBB obb) { _playerOBB = obb; }

	void SetCount(int Count) { this->count_ = Count; }
	int GetCount() { return count_; }

	inline void GetDamage()
	{
		if (!FlashF)FlashF = TRUE;
		_status.HP--;
		if (!_isAttack) {
			RecvDamage = TRUE;
		}
	}

	void CollideHammerDeb();
private:
	void RotforPlayer();
public:
	unsigned int GetHP()const;
	unsigned int GetAttackVal()const;

	XMFLOAT3 GetPos() const;
	XMFLOAT3 GetRot() const;
	XMFLOAT3 GetScl() const;

	XMFLOAT3 GetPos2() const;
	XMFLOAT3 GetRot2() const;
	XMFLOAT3 GetScl2() const;

public:
	void SetPos2(Vector3 pos) { state_obj_.Pos_ = pos; }
	void SetPos(Vector3 pos) { _status.Pos = pos; }

	virtual void SetPosDeb(Vector3 pos) = 0;
};

