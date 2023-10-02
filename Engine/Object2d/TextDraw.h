#pragma once
#include "DirectXSetting.h"
#include <cstdlib>

class TextDraw
{
private: //エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const std::string& textFormatKey, const std::string& solidColorBrushKey, const std::wstring& text, const D2D1_RECT_F& rect);

private: //静的メンバ変数
	//デフォルトフォントサイズ
	static const int32_t defaultFontSize = 32;
	//デフォルトフォント横幅
	static const int32_t defaultFontWeight = 1000;
	//デフォルトフォント
	static const std::wstring defaultFont;

private: //メンバ変数

};

