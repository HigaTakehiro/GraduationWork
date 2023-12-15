#pragma once
#include"Player.h"
#include <array>
#include <intsafe.h>
#include<DirectXMath.h>
#include "BaseCollision.h"
#include"Vector3.h"
using namespace DirectX;
class TogemaruAct
{
private:
	//��΂��j�p
	static constexpr UINT spearSize = 8;

	std::array<Vector3, spearSize>SpearPos_ = {};
	std::array<float, spearSize>SpearAngle_ = {};
	//�j�̃A���t�@�l
	float spearsAlpha = 0.f;
	//���̔��˔͈�
	float ShotRange = 0.f;
	//���˃t���O
	bool isShot = FALSE;
	//
	void ResetParam_Spear();

private:
	//�ʏ�p�����[�^
	Vector3 Pos_,Rot_,Scl_={};
	//�퓬�J�n
	bool beginBattle = FALSE;
	//�v���C���[�C���X�^���X
	Player* Player_ = nullptr;
	//�U���C���^�[�o��
	uint32_t actionCount = 1;
	//�s�����X�g
	enum Act
	{
		MOVE,
		ATTACK_RUSH,
		ATTACK_SHOTSPEAR,
		RUNAWAY,
		DEATH
	}act_;

	void Move();
	void Attack_Rush();
	void Attack_ShotSpear();
	void RunAway();
	void Death();

	//�֐��|�C���^
	static void (TogemaruAct::*ActionList[])();

//�ړ�
private:
	float Walk();//�ʏ����
	float Follow();//�Ǐ]

	//�ړ��X�s�[�h
	float movSpeed = 0.1f;
	//���������Ɉړ�����
	XMVECTOR move = { 0.f,0.f, 0.1f, 0.0f };
	XMMATRIX matRot ={};

	//��]�p�̃A���O��
	float angle_ = 0.f;
	//
	float oldRotY_;
	//
	float rotEaseT = 0.f;
	//�ǐՂ��邩
	bool isFollow = FALSE;

	//�ːi�p�ϐ�
	float rushEaseT=0.f;
	//�ːi�I����Đj�o���܂ł̑҂�����
	int32_t waitShotCount = 0;
	Vector3 RushStartPos = {};


public:
	//�s���J��
	void Transition();
public:
	// �Q�b�^�[ //
	Vector3 GetPos()const { return Pos_; }
	Vector3 GetRot()const { return Rot_; }
	Vector3 GetScl()const { return Scl_; }

	Vector3 GetSpearPos(int index)const { return SpearPos_[index]; }

	float GetSpearAlpha()const { return spearsAlpha; }

	// �Z�b�^�[ //
	void SetPlayerIns(Player* player) { Player_ = player; }

};
