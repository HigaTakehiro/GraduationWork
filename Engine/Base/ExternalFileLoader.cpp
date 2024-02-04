#include "ExternalFileLoader.h"
#include "DirectXSetting.h"
#include <fstream>

const std::string ExternalFileLoader::defaultDirectory = "Engine/Resources/GameData/";

ExternalFileLoader* ExternalFileLoader::GetIns()
{
	static ExternalFileLoader instance;
	return &instance;
}

std::stringstream ExternalFileLoader::ExternalFileOpen(const std::string& fileName)
{
	//ファイルストリーム
	std::ifstream file;
	std::stringstream fileData;
	fileData.str("");
	fileData.clear(std::stringstream::goodbit);

	file.open(defaultDirectory + fileName);
	if (file.fail()) {
		assert(0);
	}

	fileData << file.rdbuf();

	file.close();

	return fileData;
}

std::wstring ExternalFileLoader::StringToWstring(const std::string& text)
{
	//文字サイズを取得
	int32_t iBufferSize = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, (wchar_t*)NULL, 0);
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];
	//SJISからwstringに変換
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, cpUCS2, iBufferSize);
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	delete[] cpUCS2;

	return oRet;
}

std::string ExternalFileLoader::ReplaceStr(std::string& replacedStr, std::string fromStr, std::string toStr)
{
	const int32_t pos = replacedStr.find(fromStr);
	const int32_t len = fromStr.length();

	if (pos == std::string::npos || fromStr.empty()) {
		return replacedStr;
	}

	return replacedStr.replace(pos, len, toStr);
}

std::wstring ExternalFileLoader::ReplaceWstr(std::wstring& replacedStr, std::wstring fromStr, std::wstring toStr)
{
	const int32_t pos = replacedStr.find(fromStr);
	const int32_t len = fromStr.length();

	if (pos == std::string::npos || fromStr.empty()) {
		return replacedStr;
	}

	return replacedStr.replace(pos, len, toStr);
}

std::wstring ExternalFileLoader::ReplaceAllWstr(std::wstring& replacedStr, std::wstring fromStr, std::wstring toStr)
{
	int32_t pos = replacedStr.find(fromStr);
	const int32_t len = toStr.length();

	if (fromStr.empty()) return replacedStr;

	while ((pos != std::string::npos)) {
		replacedStr.replace(pos, fromStr.length(), toStr);
		if (pos == std::string::npos) {
			break;
		}
		pos = replacedStr.find(fromStr);
	}

	return replacedStr;
}
