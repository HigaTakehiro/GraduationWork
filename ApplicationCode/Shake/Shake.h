#pragma once
#include "Vector3.h"
#include "TextDraw.h"
class Shake
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �V�F�C�N���̗h��鐔�l�̎擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetShakePos() { return shakePos; }
	float GetShakeTimer() { return shakeTimer; }
private:
	//�V�F�C�N���̍ő厞��
	float shakeMaxTimer;
	
	float shakeTimer;
	bool shakeFlag;
	//�V�F�C�N�̗h��鐔�l(�J������eye�ɑ������)
	Vector3 shakePos;

};

