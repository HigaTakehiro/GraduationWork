#include "SoundManager.h"

SoundManager* SoundManager::GetIns()
{
	static SoundManager instance;
	return &instance;
}

void SoundManager::Initialize(Sound* sound)
{
	this->sound_ = sound;
	//BGM
	//LoadBGM("Engine/Resources/Sound/BGM/maou_bgm_neorock82.wav", BGMKey::game);

	//SE
	LoadSE("Engine/Resources/Sound/SE/SE_Attack.wav", SEKey::attack);
	LoadSE("Engine/Resources/Sound/SE/SE_impact.wav", SEKey::impact);
	LoadSE("Engine/Resources/Sound/SE/SE_releaseHammer.wav", SEKey::hammerRelease);
}

void SoundManager::PlayBGM(const BGMKey bgmKey, const bool isRoop, const float volume)
{
	if (bgmMap_[bgmKey].data.size <= 0) {
		LoadBGM(bgmMap_[bgmKey].fileName, bgmKey);
	}
	sound_->PlaySoundData(bgmMap_[bgmKey], isRoop, volume);
	bgmMap_[bgmKey].isPlay = true;
}

void SoundManager::PlaySE(const SEKey seKey, const float volume)
{
	seMap_[seKey].pBuffer = nullptr;
	seMap_[seKey].sound = nullptr;
	seMap_[seKey].isPlay = false;
	seMap_[seKey].data.size = 0;

	if (seMap_[seKey].data.size <= 0) {
		LoadSE(seMap_[seKey].fileName, seKey);
	}
	sound_->PlaySoundData(seMap_[seKey], false, volume);
	seMap_[seKey].isPlay = true;
}

void SoundManager::StopBGM(const BGMKey bgmKey, const bool isPause)
{
	sound_->StopSoundData(bgmMap_[bgmKey], isPause);
	bgmMap_[bgmKey].isPlay = false;
	if (!isPause) {
		bgmMap_[bgmKey].data.size = 0;
	}
}

void SoundManager::StopSE(const SEKey seKey, const bool isPause)
{
	sound_->StopSoundData(seMap_[seKey], isPause);
	seMap_[seKey].isPlay = false;
	if (!isPause) {
		seMap_[seKey].data.size = 0;
	}
}

void SoundManager::LoadBGM(const std::string& fileName, BGMKey bgmKey)
{
	if (bgmMap_[bgmKey].fileName.size() <= 0) {
		bgmMap_[bgmKey].fileName = fileName;
	}
	sound_->LoadSound(fileName, bgmMap_[bgmKey]);
}

void SoundManager::LoadSE(const std::string& fileName, SEKey seKey)
{
	if (seMap_[seKey].fileName.size() <= 0) {
		seMap_[seKey].fileName = fileName;
	}
	sound_->LoadSound(fileName, seMap_[seKey]);
}
