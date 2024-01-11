#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "TextDraw.h"
#include "IwaEffect.h"
class Shake
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(ID3D12Device* device, Camera* camera);
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	void ShakeSet();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// �V�F�C�N���̗h��鐔�l�̎擾
	/// </summary>
	/// <returns></returns>
	float GetShakePos() { return shakePos; }
	float GetShakeTimer() { return shakeTimer; }
	bool GetShakeFlag() { return shakeFlag; }
	bool SetIwaFlag(bool iwaFlag) { return this->iwaFlag = iwaFlag; }
	Vector3 GetParticlePos();
private:
	//�V�F�C�N���̍ő厞��
	float shakeMaxTimer = 30;
	float shakeTimer=0;
	bool shakeFlag;
	//�V�F�C�N�̗h��鐔�l(�J������eye�ɑ������)
	float shakePos;
	Vector3 pos{};
	float timer{};
	//����鏈��
	IwaEffect* iwa[2];
	bool iwaFlag;
	float iwaCount;
	Vector2 iwaPos[2];

	float fade;
};

