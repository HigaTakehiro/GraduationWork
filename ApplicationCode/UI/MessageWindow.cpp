#include "MessageWindow.h"
#include "safeDelete.h"
#include "Easing.h"
#include"PadInput.h"
#include"KeyInput.h"

void MessageWindow::Initialize(const std::string& fileName)
{
	textData_ = ExternalFileLoader::GetIns()->ExternalFileOpen(fileName);
	textWindow_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::TextWindow, { 640, 630 });
	textWindow_->SetAlpha(1.f);
	textWindowSize_ = textWindow_->GetSize();
	textWindowSize_.y = 0;
	textWindow_->SetAnchorPoint({ 0.5f, 0.5f });

	textAddTimer_ = 0;
	textSpeed_ = 1;
	textCount_ = 0;
	isTextDrawComplete_ = false;

}

void MessageWindow::Update()
{
	std::string line;
	std::string face;
	std::string messageData;
	std::wstring messageDataW;

	if (isMessageUpdateWait_) {
		if (isTextDrawComplete_) {
			messageWaitTimer_--;
		}
		if (messageWaitTimer_ <= 0) {
			isMessageUpdateWait_ = false;
			textCount_ = 0;
			message_.clear();
			drawMessage_.clear();
		}
		return;
	}

	if (isTextFinishBottun) {
		
		messageWaitTimer_=10;
		
		if (PadInput::GetIns()->TriggerButton(PadInput::Button_A) || KeyInput::GetIns()->PushKey(DIK_SPACE)) {
			isTextFinishBottun = false;
			textCount_ = 0;
			message_.clear();
			drawMessage_.clear();
		}
		return;
	}

	while (getline(textData_, line)) {
		std::istringstream line_stream(line);
		std::string word;
		//���p��؂�ŕ�������擾
		getline(line_stream, word, ' ');
		if (word == "#") {
			continue;
		}
		if (word == "OPEN") {
			isTextWindowOpen_ = true;
		}
		if (word == "TEXT") {
			line_stream >> messageData;
			messageDataW = ExternalFileLoader::GetIns()->StringToWstring(messageData);
			message_ = messageDataW;
		}
		if (word == "SPEED") {
			line_stream >> textSpeed_;
		}
		if (word == "WAIT") {
			isMessageUpdateWait_ = true;
			line_stream >> messageWaitTimer_;
			break;
		}
		if (word == "CLOSE") {
			isTextWindowOpen_ = false;
		}
		if (word == "BOTTUN") {
			isTextFinishBottun = true;
			break;
		}
	}
}

void MessageWindow::SpriteDraw()
{
	textWindow_->Draw();
}

void MessageWindow::TextMessageDraw()
{
	//�E�B���h�E�T�C�Y(�N���[�Y��)
	const float closeWindowSizeY = 0.0f;
	//�E�B���h�E�T�C�Y(�I�[�v����)
	const float openWindowSizeY = 160.0f;

	//���b�Z�[�W�E�B���h�E�J����
	//���b�Z�[�W�E�B���h�E������
	if (!isTextWindowOpen_) {
		windowOpenTimer_ = 0;
		windowCloseTimer_++;
		if (windowCloseTimer_ >= windowCloseTime) {
			windowCloseTimer_ = windowCloseTime;
		}
		//�C�[�Y�C���A�E�g�Ń��b�Z�[�W�E�B���h�E�����
		textWindowSize_.y = Easing::easeInOut((float)windowCloseTimer_, (float)windowCloseTime, closeWindowSizeY, textWindowSize_.y);
	}
	//���b�Z�[�W�E�B���h�E�J������
	else if (isTextWindowOpen_) {
		windowCloseTimer_ = 0;
		windowOpenTimer_++;
		if (windowOpenTimer_ >= windowOpenTime) {
			windowOpenTimer_ = windowOpenTime;
		}
		//�C�[�Y�C���A�E�g�Ń��b�Z�[�W�E�B���h�E���J��
		textWindowSize_.y = Easing::easeInOut((float)windowOpenTimer_, (float)windowOpenTime, openWindowSizeY, textWindowSize_.y);
	}

	//���b�Z�[�W�E�B���h�E�T�C�Y��ύX
		textWindow_->SetSize(textWindowSize_);

	//�ǂݍ��񂾃e�L�X�g�`�悪�������Ă�����
	//�e�L�X�g�X�s�[�h��0�ȉ��ɂȂ�Ȃ��悤�ɂ���
	if (textSpeed_ <= 0) {
		textSpeed_ = 1;
	}
	//�e�L�X�g�`��͈�
	D2D1_RECT_F textDrawPos = {
		260, 580, 950, 700
	};
	D2D1_RECT_F bombMessageDrawPos = {
		400, 0, 900, 300
	};
	//�e�L�X�g��1�������w�莞�Ԃ��Ƃɒǉ�����
	textAddTimer_++;
	isTextDrawComplete_ = false;
	if (textAddTimer_ >= textSpeed_) {
		textAddTimer_ = 0;
		if (textCount_ < message_.size()) {
			if (message_.substr(textCount_, 1) != L"/") {
				drawMessage_ += message_.substr(textCount_, 1);
			}
			else {
				drawMessage_ += L"\n";
			}
			textCount_++;
		}
		//�ǂݍ��񂾃e�L�X�g�`�悪����������
		if (textCount_ >= message_.size()) {
			isTextDrawComplete_ = true;
		}
	}
	//���ݒǉ�����Ă��镶����S�ĕ`�悷��
	textDraw_->Draw("bestTen", "white", drawMessage_, textDrawPos);
}
