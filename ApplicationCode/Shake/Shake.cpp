#include "Shake.h"
#include "KeyInput.h"
#include"time.h"
#include "ExternalFileLoader.h"

void Shake::Initialize(ID3D12Device* device, Camera* camera)
{
	shakeTimer = 0;
	shakeFlag = false;
	ShakeSet();
	iwaFlag = false;
	for (int i = 0; i < 2; i++) {
		iwa[i] = new IwaEffect();
		iwa[i]->Initialize(device, camera);
	}
	iwaCount = 0;
	iwaPos[0] = { -300,-100 };
	iwaPos[1] = { 1500,-100 };
}

void Shake::Update()
{
	//今はエンターキーを押したらシェイクするようになってい
	//壁ができたら壁に当たったらシェイクする処理に変更する
	if (KeyInput::GetIns()->TriggerKey(DIK_RETURN)) {
		iwaCount = 0;
		shakeFlag = true;
		iwaFlag = true;
	}
	if (shakeFlag == true) {
		if (shakeTimer < shakeMaxTimer) {
			shakeTimer++;
			//-10~10の範囲でランダム
			shakePos = rand() % (int)pos.x - (int)pos.y;
		}
		else {
			shakeTimer = 0;
			shakeFlag = false;
			shakePos = 0;
		}
	}

	if (iwaFlag == true) {
		iwaCount++;
		if (iwaCount < 100) {
			for (int i = 0; i < 2; i++) {
				iwa[i]->Update(iwaPos[i]);
			}
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

void Shake::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (iwaCount < 100) {
		for (int i = 0; i < 2; i++) {
			iwa[i]->Draw(cmdList);
		}
	}
}
