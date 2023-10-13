#include "Shake.h"
#include "KeyInput.h"
#include"time.h"
#include "ExternalFileLoader.h"
void Shake::Initialize()
{
	shakeTimer = 0;
	shakeFlag = false;
	ShakeSet();
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
			shakePos = rand() % (int)pos.x - (int)pos.y;
		}
		else {
			shakeTimer = 0;
			shakeFlag = false;
			shakePos = 0;
		}
	}
}

void Shake::ShakeSet()
{
	std::string line;
	std::stringstream stream;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("Shake.csv");

	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("#") == 0) {
			continue;
		}
		if (word.find("pos") == 0) {
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
		}
		if (word.find("timer") == 0) {
			line_stream >> timer;
		}
		shakeMaxTimer = timer;

	}
}
