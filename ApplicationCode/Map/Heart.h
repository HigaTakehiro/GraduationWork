#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"

class Player;
class Heart
{
public:
	~Heart();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos"></param>
	void Initialize(const Vector3& pos);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Player* player);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	void HitPlayer();
private:
	//�I�u�W�F�N�g
	Object3d* heart_;
	//���f��
	Model* model;

	Player* player_;

	bool display_ = false;

	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�A�j������
	int32_t animeTime_;
	//�A�j���J�E���g
	int32_t animeCount_;

};

