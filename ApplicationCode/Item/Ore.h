#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Ore
{
public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Ore() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Ore();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos">�������W</param>
	/// <param name="vec">�����x�N�g��</param>
	void Initialize(Vector3 pos, Vector3 vec);
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
	/// <summary>
	/// �����蔻��擾
	/// </summary>
	/// <returns></returns>
	bool GetIsHit() { return ore_->GetIsHit(); }

private: //�����o�ϐ�
	// �z�΃I�u�W�F�N�g
	std::unique_ptr<Object3d> ore_;
	// �z�΃��f��
	Model* oreModel_;
	// ���W
	Vector3 pos_;
	// �����x�N�g��
	Vector3 vec_;
	// ���x
	float speed_;
};

