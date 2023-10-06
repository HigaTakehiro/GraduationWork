#include "Shake.h"
#include "KeyInput.h"
void Shake::Initialize()
{
	shakeMaxTimer = 30;
	shakeTimer = 0;
	shakePos = {0,0,0};
	shakeFlag = false;
}

void Shake::Update()
{
	if (KeyInput::GetIns()->TriggerKey(DIK_RETURN)) {
		shakeFlag = true;
	}
	if (shakeFlag == true) {
		if (shakeTimer < shakeMaxTimer) {
			shakeTimer++;
		}
		else {
			shakeTimer = 0;
			shakeFlag = false;
		}
	}
}
