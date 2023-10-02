#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <map>
#include <vector>
#include <string>

/// <summary>
/// オーディオコールバック
/// </summary>
class XSound2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// ボイス処理パスの開始時
	//STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	// ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	// バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	// バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	// バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
		// バッファを解放する
		delete[] pBufferContext;
	};
	// 再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	// ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

/// <summary>
/// オーディオ
/// </summary>
class Sound
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // サブクラス
	// チャンクヘッダ
	struct Chunk
	{
		char	id[4]; // チャンク毎のID
		int32_t		size;  // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		Chunk	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		Chunk		chunk; // "fmt "
		WAVEFORMAT	fmt;   // 波形フォーマット
	};

	struct SoundData {
		IXAudio2SourceVoice* sound;
		Chunk data;
		std::string fileName;
		char* pBuffer;
		bool isPlay;
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// BGMを再生
	/// </summary>
	/// <param name="soundData">再生したい音データ</param>
	/// <param name="isRoop">ループフラグ</param>
	/// <param name="volume">bgm再生</param>
	void PlaySoundData(const SoundData& soundData, bool isRoop, float volume);

	/// <summary>
	/// BGMを停止
	/// </summary>
	/// <param name="soundData">停止したい音データ</param>
	/// <param name="isPause">一時停止フラグ</param>
	void StopSoundData(const SoundData& soundData, bool isPause);

	/// <summary>
	/// 音データ読み込み
	/// </summary>
	/// <param name="fileName">読み込む音データファイル</param>
	/// <param name="soundData">書き込む音データ構造体</param>
	void LoadSound(const std::string& fileName, SoundData& soundData);

private: // メンバ変数
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XSound2VoiceCallback voiceCallback;
};

