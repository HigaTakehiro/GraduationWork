#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"
#include<DirectXMath.h>
using namespace DirectX;

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
	void Update(Player* player, bool& Display);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(bool Display);

	void Jump();

	void Spown();

	void HitPlayer(bool& Display);
private:
	//�I�u�W�F�N�g
	Object3d* heart_;
	//���f��
	Model* model_[2];

	Player* player_;

	bool display_ = false;

	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�A�j������
	int32_t animeTime_;
	//�A�j���J�E���g
	int32_t animeCount_;

	Vector3 pos_;

	bool spown_ = true;

	XMFLOAT4 col_{ 1.f,1.f,1.f,1.f };
};

