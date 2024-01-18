#include "SkillPanel.h"
#include "SafeDelete.h"
#include "ImageManager.h"
#include "Vector3.h"

SkillPanel::~SkillPanel()
{
	safe_delete(text_);
}

void SkillPanel::Initialize(const std::wstring& skillName, const Vector2& pos)
{
	//スキル名を設定
	skillName_ = skillName;
	//フラグを初期化
	isActive_ = false;
	isSkillGet_ = false;
	//座標をセット
	pos_ = pos;
	//スキルパネルスプライト
	skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	skillPanel_->SetSize({ 96, 64 });
	//説明パネルスプライト
	textPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, pos_, { 1.0f, 1.0f, 1.0f, 1.f }, { 0.5f, 0.5f });
	//タイマー初期化
	panelOpenTimer_ = 0;
	//説明パネルサイズ
	textPanelSize_ = { 0.f, 0.f };
	textPanel_->SetSize(textPanelSize_);
}

void SkillPanel::Update(Vector2 cursorPos)
{
	const Vector3 gray = { 0.2f, 0.2f, 0.2f };
	const Vector3 white = { 1.f, 1.f, 1.f };
	const Vector3 green = { 0.2f, 0.6f, 0.2f };
	Vector2 textPanelPos = { pos_.x + 100.f, pos_.y + 100.f };


	if (!isActive_) {
		skillPanel_->SetColor(gray);
	}
	else if(isActive_ && !isSkillGet_){
		skillPanel_->SetColor(white);
	}
	else {
		skillPanel_->SetColor(green);
	}

	skillPanel_->SetPosition(pos_);
	textPanel_->SetPosition(textPanelPos);
	TextPanelOpen(cursorPos);
}

void SkillPanel::SpriteDraw()
{
	skillPanel_->Draw();
	textPanel_->Draw();
}

void SkillPanel::TextMessageDraw()
{
	D2D1_RECT_F drawRange = { pos_.x - 40.f, pos_.y - 15.f, pos_.x + 50.f, pos_.y + 100.f };
	std::string textColor = "black";
	if (!isActive_) {
		textColor = "white";
	}
	if (isSkillGet_) {
		textColor = "yellow";
	}
	text_->Draw("bestTen_16", textColor, skillName_.c_str(), drawRange);
}

void SkillPanel::TextPanelOpen(Vector2 cursorPos)
{

	const float openTime = 30;
	const Vector2 textPanelEndSize = { 128.f, 256.f };

	if (PanelToCursorHit(cursorPos)) {
		if (panelOpenTimer_ < openTime) panelOpenTimer_++;
		if (!isActive_) {
			skillPanel_->SetColor({ 0.6f, 0.2f, 0.2f });
		}
		else if (!isSkillGet_) {
			skillPanel_->SetColor({ 0.2f, 0.2f, 0.6f });
		}
		float time = panelOpenTimer_ / openTime;
		textPanelSize_ = easeIn(textPanelSize_, textPanelEndSize, time);
	}
	else {
		textPanelSize_ = { 0.f, 0.f };
		panelOpenTimer_ = 0.f;
	}
	textPanel_->SetSize(textPanelSize_);

}

bool SkillPanel::PanelToCursorHit(Vector2 cursorPos)
{
	const float panelSizeX = 96 / 2;
	const float panelSizeY = 64 / 2;

	if (cursorPos.x >= pos_.x - panelSizeX && cursorPos.x <= pos_.x + panelSizeX && cursorPos.y >= pos_.y - panelSizeY && cursorPos.y <= pos_.y + panelSizeY) return true;

	return false;
}
