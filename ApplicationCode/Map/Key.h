#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Vector3.h"
#include<DirectXMath.h>
using namespace DirectX;

class Player;
class Key
{
public:
	~Key();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos"></param>
	void Initialize(const Vector3& pos);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Player* player,bool& lock,bool Display);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(bool Display);

	void Jump();
private:
	//�I�u�W�F�N�g
	Object3d* key_;
	//���f��
	Model* model[2];

	Player* player_;

	Vector3 pos_;

	float addpos_ = 0.f;

	bool display_ = false;

	XMFLOAT4 col_{ 1.f,1.f,1.f,1.f };

	//�A�j���[�V�����^�C�}�[
	int32_t animeTimer_;
	//�A�j������
	int32_t animeTime_;
	//�A�j���J�E���g
	int32_t animeCount_;

};

