#pragma once

#include <string>
#include <sstream>
#include "TextDraw.h"
#include "ImageManager.h"
#include "Sprite.h"
#include "Vector3.h"

class ExternalFileLoader final
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ExternalFileLoader() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ExternalFileLoader() = default;
	/// <summary>
	/// �R�s�[�R���X�g���N�^���֎~
	/// </summary>
	ExternalFileLoader(const ExternalFileLoader& obj) = delete;
	/// <summary>
	/// ������Z�q���֎~
	/// </summary>
	ExternalFileLoader& operator=(const ExternalFileLoader& obj) = delete;

public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static ExternalFileLoader* GetIns();

public: //�����o�֐�

	/// <summary>
	/// �O���t�@�C���ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns>�t�@�C���f�[�^</returns>
	std::stringstream ExternalFileOpen(const std::string& fileName);

	/// <summary>
	/// string�^��wstring�^�ɕϊ�����(UTF-8)
	/// </summary>
	/// <param name="text">�ϊ��������e�L�X�g</param>
	/// <returns>wstring�^�̃e�L�X�g</returns>
	std::wstring StringToWstring(const std::string& text);

	/// <summary>
	/// ������u��(string)
	/// </summary>
	/// <param name="replacedStr">�u��������������</param>
	/// <param name="fromStr">�u��������������</param>
	/// <param name="toStr">�u�����镶����</param>
	/// <returns>�u���㕶����</returns>
	std::string ReplaceStr(std::string& replacedStr, std::string fromStr, std::string toStr);

	/// <summary>
	/// ������u��(wstring)
	/// </summary>
	/// <param name="replacedStr">�u��������������</param>
	/// <param name="fromStr">�u��������������</param>
	/// <param name="toStr">�u�����镶����</param>
	/// <returns>�u���㕶����</returns>
	std::wstring ReplaceWstr(std::wstring& replacedStr, std::wstring fromStr, std::wstring toStr);

	/// <summary>
	/// �S�Ă̕�����u��(wstring)
	/// </summary>
	/// <param name="replacedStr">�u��������������</param>
	/// <param name="fromStr">�u��������������</param>
	/// <param name="toStr">�u�����镶����</param>
	/// <returns>�u���㕶����</returns>
	std::wstring ReplaceAllWstr(std::wstring& replacedStr, std::wstring fromStr, std::wstring toStr);

private: //�ÓI�����o�ϐ�
	//�f�t�H���g�t�@�C���p�X
	const static std::string defaultDirectory;

private: //�����o�֐�


private: //�����o�ϐ�

};

