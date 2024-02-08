#pragma once
#include "Sound.h"
#include <map>

class SoundManager final
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SoundManager() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SoundManager() = default;
	/// <summary>
	/// コピーコンストラクタを禁止
	/// </summary>
	SoundManager(const SoundManager& obj) = delete;

	/// <summary>
	/// 代入演算子を禁止
	/// </summary>
	SoundManager& operator=(const SoundManager& obj) = delete;

public: //サブクラス
	enum class BGMKey {
		dungeon,
		firstBoss,
		restPoint,
		title,
	};

	enum class SEKey {
		attack,
		impact,
		hammerRelease,
		firstBossCrossAttack,
		firstBossDestroy,
		firstBossFaceAttack,
		firstBossHitAttack,
		firstBossShout,
		gimmickSteps,
		hammerAttack,
		hammerBigBlow,
		hammerShake,
		//impact,
		playerDamage,
		playerDestroy,
		playerLevelUp,
		releaseHammer,
		userChoice,
		userDecision,
		openBox,
		skillOpen,
	};

public:  //静的メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static SoundManager* GetIns();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="sound">サウンドクラスのインスタンス</param>
	void Initialize(Sound* sound);
	
	/// <summary>
	/// BGMを再生
	/// </summary>
	/// <param name="bgmKey">再生したいBGMマップのキー</param>
	/// <param name="isRoop">ループ再生フラグ</param>
	/// <param name="volume">BGM音量</param>
	void PlayBGM(const BGMKey bgmKey, const bool isRoop, const float volume);

	/// <summary>
	/// SEを再生
	/// </summary>
	/// <param name="seKey">再生したいSEマップのキー</param>
	/// <param name="volume">SE音量</param>
	void PlaySE(const SEKey seKey, const float volume);

	/// <summary>
	/// BGMを停止
	/// </summary>
	/// <param name="bgmKey">停止したいBGMマップのキー</param>
	void StopBGM(const BGMKey bgmKey, const bool isPause = false);

	/// <summary>
	/// 全てのBGMを停止
	/// </summary>
	void StopAllBGM();

	/// <summary>
	/// SEを停止
	/// </summary>
	/// <param name="seKey">停止したいSEマップのキー</param>
	void StopSE(const SEKey seKey, const bool isPause = false);

private: //メンバ変数

	/// <summary>
	/// BGM追加
	/// </summary>
	/// <param name="bgmKey">追加したいBGMマップのキー</param>
	/// <param name="fileName">ファイル名</param>
	void LoadBGM(const std::string& fileName, BGMKey bgmKey);

	/// <summary>
	/// SE追加
	/// </summary>
	/// <param name="seKey">追加したいSEマップのキー</param>
	/// <param name="fileName">ファイル名</param>
	void LoadSE(const std::string& fileName, SEKey seKey);

private: //メンバ変数
	//サウンドクラスのインスタンス
	Sound* sound_ = nullptr;
	//BGM連想配列
	std::map<BGMKey, Sound::SoundData> bgmMap_;
	//SE連想配列
	std::map<SEKey, Sound::SoundData> seMap_;
};

