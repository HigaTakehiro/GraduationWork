#pragma once
#include "Object3d.h"

class Object3d;

class BaseCollision
{
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseCollision() {};
	/// <summary>
	/// �����蔻��`�F�b�N
	/// </summary>
	/// <param name="obj1">�I�u�W�F�N�g1</param>
	/// <param name="obj2">�I�u�W�F�N�g2</param>
	virtual bool HitTest(Object3d* obj1, Object3d* obj2) = 0;
};

