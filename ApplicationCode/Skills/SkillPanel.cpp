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
}

void SkillPanel::Update()
{
	const Vector3 gray = { 0.2f, 0.2f, 0.2f };
	const Vector3 white = { 1.f, 1.f, 1.f };
	const Vector3 blue = { 0.2f, 0.6f, 0.2f };

	if (!isActive_) {
		skillPanel_->SetColor(gray);
	}
	else if(isActive_ && !isSkillGet_){
		skillPanel_->SetColor(white);
	}
	else {
		skillPanel_->SetColor(blue);
	}
}

void SkillPanel::SpriteDraw()
{
	skillPanel_->Draw();
}

void SkillPanel::TextMessageDraw()
{
	D2D1_RECT_F drawRange = { pos_.x - 50.f, pos_.y, pos_.x + 50.f, pos_.y + 100.f };
	std::string textColor = "black";
	if (!isActive_) {
		textColor = "white";
	}
	if (isSkillGet_) {
		textColor = "yellow";
	}
	text_->Draw("bestTen_16", textColor, skillName_.c_str(), drawRange);
}
