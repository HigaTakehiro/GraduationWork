#pragma once
#include "Sprite.h"
#include "TextDraw.h"
#include "Vector2.h"
#include "ExternalFileLoader.h"
#include <sstream>

class MessageWindow
{
public: //列挙型

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MessageWindow() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MessageWindow() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& fileName);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// スプライト描画処理
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// テキストデータの描画
	/// </summary>
	void TextMessageDraw();

private: //静的メンバ変数
	//テキストウィンドウ開放時間
	static const int32_t windowOpenTime = 120;
	//テキストウィンドウ閉鎖時間
	static const int32_t windowCloseTime = 120;


private: //メンバ変数

	//テキストウィンドウ
	std::unique_ptr<Sprite> textWindow_;
	//テキストウィンドウサイズ
	Vector2 textWindowSize_;
	//テキストスピード
	int32_t textSpeed_;
	//テキスト数
	int32_t textCount_;
	//テキスト追加時間
	int32_t textAddTimer_;
	//文字描画クラス
	TextDraw* textDraw_;
	//ウィンドウ開放タイマー
	int32_t windowOpenTimer_;
	//ウィンドウ閉鎖タイマー
	int32_t windowCloseTimer_;
	//メッセージデータ更新待機フラグ
	bool isMessageUpdateWait_;
	//メッセージデータ待機タイマー
	int32_t messageWaitTimer_;
	//メッセージデータ描画完了フラグ
	bool isTextDrawComplete_;
	//テキストウィンドウ開閉フラグ
	bool isTextWindowOpen_;
	//テキストをボタンで管理用フラグ
	bool isTextFinishBottun;
	//メッセージデータ格納用文字列
	std::stringstream textData_;
	//メッセージ出力用文字列
	std::wstring drawMessage_;
	//メッセージ格納用文字列
	std::wstring message_;

public:
	bool GetCloseWindow() { return isTextWindowOpen_; }
};

