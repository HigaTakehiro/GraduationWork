#pragma once
#include "Sprite.h"
#include "TextDraw.h"
#include "Vector2.h"
#include "ExternalFileLoader.h"
#include <sstream>

class MessageWindow
{
public: //�񋓌^

public: //�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MessageWindow() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MessageWindow() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const std::string& fileName);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �X�v���C�g�`�揈��
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// �e�L�X�g�f�[�^�̕`��
	/// </summary>
	void TextMessageDraw();

private: //�ÓI�����o�ϐ�
	//�e�L�X�g�E�B���h�E�J������
	static const int32_t windowOpenTime = 120;
	//�e�L�X�g�E�B���h�E������
	static const int32_t windowCloseTime = 120;


private: //�����o�ϐ�

	//�e�L�X�g�E�B���h�E
	std::unique_ptr<Sprite> textWindow_;
	//�e�L�X�g�E�B���h�E�T�C�Y
	Vector2 textWindowSize_;
	//�e�L�X�g�X�s�[�h
	int32_t textSpeed_;
	//�e�L�X�g��
	int32_t textCount_;
	//�e�L�X�g�ǉ�����
	int32_t textAddTimer_;
	//�����`��N���X
	TextDraw* textDraw_;
	//�E�B���h�E�J���^�C�}�[
	int32_t windowOpenTimer_;
	//�E�B���h�E���^�C�}�[
	int32_t windowCloseTimer_;
	//���b�Z�[�W�f�[�^�X�V�ҋ@�t���O
	bool isMessageUpdateWait_;
	//���b�Z�[�W�f�[�^�ҋ@�^�C�}�[
	int32_t messageWaitTimer_;
	//���b�Z�[�W�f�[�^�`�抮���t���O
	bool isTextDrawComplete_;
	//�e�L�X�g�E�B���h�E�J�t���O
	bool isTextWindowOpen_;
	//�e�L�X�g���{�^���ŊǗ��p�t���O
	bool isTextFinishBottun;
	//���b�Z�[�W�f�[�^�i�[�p������
	std::stringstream textData_;
	//���b�Z�[�W�o�͗p������
	std::wstring drawMessage_;
	//���b�Z�[�W�i�[�p������
	std::wstring message_;

public:
	bool GetCloseWindow() { return isTextWindowOpen_; }
};

