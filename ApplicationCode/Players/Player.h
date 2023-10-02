#pragma once
#include "Object3d.h"
#include "Vector3.h"

class Player
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

private: //�����o�ϐ�
	//���W
	Vector3 pos_;
	//�傫��
	Vector3 scale_;
	//��]�p
	float rot_;

	//�I�u�W�F�N�g
	std::unique_ptr<Object3d> player_;
	//���f��
	Model* model_[5];
};

