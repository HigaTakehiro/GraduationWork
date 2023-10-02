#include "Sound.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

bool Sound::Initialize() {
	HRESULT result;

	// XSoundエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return true;
}

void Sound::PlaySoundData(const SoundData& soundData, bool isRoop, float volume)
{
	HRESULT result;

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData.pBuffer;
	buf.pContext = soundData.pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData.data.size;
	if (isRoop == true) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData.sound->SetVolume(volume);

	// 波形データの再生
	if (!soundData.isPlay) {
		result = soundData.sound->SubmitSourceBuffer(&buf);
		result = soundData.sound->Start();
	}

}

void Sound::StopSoundData(const SoundData& soundData, bool isPause)
{
	HRESULT result;
	XAUDIO2_VOICE_STATE voiceState;
	soundData.sound->GetState(&voiceState);
	soundData.sound->Stop();
	if (!isPause) {
		result = soundData.sound->FlushSourceBuffers();
	}
}

void Sound::LoadSound(const std::string& fileName, SoundData& soundData)
{
	HRESULT result;
	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(fileName, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	// Dataチャンクの読み込み
	file.read((char*)&soundData.data, sizeof(soundData.data));

	// Dataチャンクのデータ部（波形データ）の読み込み
	soundData.pBuffer = new char[soundData.data.size];
	file.read(soundData.pBuffer, soundData.data.size);

	// Waveファイルを閉じる
	file.close();

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// 波形フォーマットを元にSourceVoiceの生成
	result = xAudio2->CreateSourceVoice(&soundData.sound, &wfex, 0, 2.0f, &voiceCallback);
}