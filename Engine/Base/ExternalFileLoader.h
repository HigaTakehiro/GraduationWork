#pragma once

#include <string>
#include <sstream>
#include "TextDraw.h"
#include "ImageManager.h"
#include "Sprite.h"

class ExternalFileLoader final
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ExternalFileLoader() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ExternalFileLoader() = default;
	/// <summary>
	/// コピーコンストラクタを禁止
	/// </summary>
	ExternalFileLoader(const ExternalFileLoader& obj) = delete;
	/// <summary>
	/// 代入演算子を禁止
	/// </summary>
	ExternalFileLoader& operator=(const ExternalFileLoader& obj) = delete;

public: //静的メンバ関数
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns></returns>
	static ExternalFileLoader* GetIns();

public: //メンバ関数

	/// <summary>
	/// 外部ファイル読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>ファイルデータ</returns>
	std::stringstream ExternalFileOpen(const std::string& fileName);

	/// <summary>
	/// string型をwstring型に変換する(UTF-8)
	/// </summary>
	/// <param name="text">変換したいテキスト</param>
	/// <returns>wstring型のテキスト</returns>
	std::wstring StringToWstring(const std::string& text);

private: //静的メンバ変数
	//デフォルトファイルパス
	const static std::string defaultDirectory;

private: //メンバ関数


private: //メンバ変数

};

