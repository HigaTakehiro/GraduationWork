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
	//今はエンターキーを押したらシェイクするようになってい
	//壁ができたら壁に当たったらシェイクする処理に変更する
	if (KeyInput::GetIns()->TriggerKey(DIK_RETURN)) {
		shakeFlag = true;
	}
	if (shakeFlag == true) {
		if (shakeTimer < shakeMaxTimer) {
			shakeTimer++;
			//-10~10の範囲でランダム
			shakePos= rand() % 7 - 3;
		}
		else {
			shakeTimer = 0;
			shakeFlag = false;
			shakePos = 0;
		}
	}
}
