#pragma once
#include "Object3d.h"
#include "BaseCollision.h"

class SphereAndSphere : public BaseCollision
{
public: //�����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SphereAndSphere() = default;
	/// <summary>
	/// �����蔻��`�F�b�N(���Ƌ�)
	/// </summary>
	/// <param name="obj1">���肵�����I�u�W�F�N�g1</param>
	/// <param name="obj2">���肵�����I�u�W�F�N�g2</param>
	/// <returns>�������Ă���</returns>
	bool HitTest(Object3d* obj1, Object3d* obj2) override;
};

