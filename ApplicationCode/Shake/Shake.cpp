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
	iwaPos[0] = { 200,0 };
	iwaPos[1] = { 1000,0 };
	fade = 1;
}

void Shake::Update()
{

	if (shakeFlag == true) {
		if (shakeTimer < shakeMaxTimer) {
			shakeTimer ++;
			//�����_��
			shakePos = rand() % (int)pos.x - (int)pos.y;
			shakePos /= 10.0f;
		}
		else {
			shakeTimer = 0;
			shakeFlag = false;
			shakePos = 0;
		}
	}
	//��
	if (iwaFlag == true) {
		iwaCount++;
		if (iwaCount < 100) {
			fade -= 0.01f;
			for (int i = 0; i < 2; i++) {
				iwa[i]->Update(iwaPos[i], fade);
			}
		}

	}
	if (iwaCount > 100) {
		iwaCount = 0;
		fade = 1;
		iwaFlag = false;
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
	if (iwaFlag == true) {
		for (int i = 0; i < 2; i++) {
			iwa[i]->Draw(cmdList);
		}
	}
}

Vector3 Shake::GetParticlePos()
{
	return iwa[0]->GetParticlePos();
}
