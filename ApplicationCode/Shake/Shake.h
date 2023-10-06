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
	void ShakeSet();
	/// <summary>
	/// �V�F�C�N���̗h��鐔�l�̎擾
	/// </summary>
	/// <returns></returns>
	float GetShakePos() { return shakePos; }
	float GetShakeTimer() { return shakeTimer; }
	bool GetShakeFlag() { return shakeFlag; }
private:
	//�V�F�C�N���̍ő厞��
	float shakeMaxTimer;
	
	float shakeTimer;
	bool shakeFlag;
	//�V�F�C�N�̗h��鐔�l(�J������eye�ɑ������)
	float shakePos;
	Vector3 pos{};
	float timer{};
};

