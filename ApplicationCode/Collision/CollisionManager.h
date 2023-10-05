#pragma once
#include "BaseCollision.h"
#include <list>

class Object3d;

class CollisionManager
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CollisionManager();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~CollisionManager();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �I�u�W�F�N�g�ǉ�
	/// </summary>
	/// <param name="obj">�ǉ��I�u�W�F�N�g</param>
	void AddObj(Object3d& obj);
	/// <summary>
	/// �R���W�����`�F�b�N
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	void CollisionCheck(Object3d* obj1, Object3d* obj2);
	/// <summary>
	/// �����蔻��v�Z
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	void HitTest(Object3d* obj1, Object3d* obj2);
	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

private: //�����o�ϐ�
	//�I�u�W�F�N�g���X�g
	std::list<Object3d*> objList_;
	//�R���W�����`�F�b�N
	BaseCollision* collision_[1][1];
};

