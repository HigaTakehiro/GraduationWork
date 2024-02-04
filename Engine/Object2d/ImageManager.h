#pragma once
#include "Sprite.h"

class ImageManager final
{
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager& obj) = delete;
	ImageManager& operator=(const ImageManager& obj) = delete;

public: //静的メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static ImageManager* GetIns();

public: //画像名
	enum class ImageName {
		background = 1,
		bar,
		skill,
		skillButton,
		susumuButton,
		Arrow,
		filter,
		title,
		asist_,
		wake,
		TextWindow,
		skillCursor = 15,
		skillIcon_HammerReturn,
		skillIcon_HPUp,
		skillIcon_ATKUp,
		skillIcon_DEFUp,
		skillIcon_SPDUp,
		BossBar,
		BossBar_Toge,
		Flash,
		PlayerUIBack
	};

	enum Image2DName {
		USA_1,
		USA_2,
		USA_3,
		//
		BUSA1,
		BUSA2,
		BUSA3,
		//
		//
		RUSA1,
		RUSA2,
		RUSA3,
		//
		LUSA1,
		LUSA2,
		LUSA3,
		//
		STONE,
		TestPlay,
		IdlePlayer,
	};
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private:

};

