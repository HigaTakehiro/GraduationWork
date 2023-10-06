#include "Shake.h"
#include "KeyInput.h"
#include"time.h"
void Shake::Initialize()
{
	shakeMaxTimer = 10;
	shakeTimer = 0;
	shakePos = 0;
	shakeFlag = false;
	srand((unsigned)time(NULL));
}

void Shake::Update()
{
	//���̓G���^�[�L�[����������V�F�C�N����悤�ɂȂ��Ă�
	//�ǂ��ł�����ǂɓ���������V�F�C�N���鏈���ɕύX����
	if (KeyInput::GetIns()->TriggerKey(DIK_RETURN)) {
		shakeFlag = true;
	}
	if (shakeFlag == true) {
		if (shakeTimer < shakeMaxTimer) {
			shakeTimer++;
			//-10~10�͈̔͂Ń����_��
			shakePos= rand() % 7 - 3;
		}
		else {
			shakeTimer = 0;
			shakeFlag = false;
			shakePos = 0;
		}
	}
}
