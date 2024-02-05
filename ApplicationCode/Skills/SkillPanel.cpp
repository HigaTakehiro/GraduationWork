#include "SkillPanel.h"
#include "SafeDelete.h"
#include "ImageManager.h"
#include "Vector3.h"
#include "ExternalFileLoader.h"

SkillPanel::~SkillPanel()
{
	safe_delete(text_);
}

void SkillPanel::Initialize(const std::wstring& skillName, const Vector2& pos, SkillType type, int32_t num)
{
	//スキル名を設定
	skillName_ = skillName;
	//フラグを初期化
	isActive_ = false;
	isSkillGet_ = false;
	//座標をセット
	pos_ = pos;
	skillType_ = type;
	//スキルパネルスプライト
	if (type == HPUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_HPUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == ATKUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_ATKUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == DEFUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_DEFUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == SPDUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_SPDUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == HammerReturn) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_HammerReturn, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == FallHammer) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_FallHammer, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == HyperMode) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_HyperMode, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == Empty) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_Empty, pos_, { 1.f, 1.f, 1.f, 1.f }, { 0.5f, 0.5f });
	}
	else {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	//ステータス上昇数値をセット
	num_ = num;
	drawTime_ = 0;
	SkillTextLoad();

	skillPanel_->SetSize({ 50, 50 });
}

void SkillPanel::Update(Vector2 cursorPos)
{
	const Vector3 gray = { 0.2f, 0.2f, 0.2f };
	const Vector3 white = { 1.5f, 1.5f, 1.5f };
	const Vector3 green = { 0.2f, 0.8f, 0.2f };
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

	TextOpen(cursorPos);
	skillPanel_->SetPosition(pos_);
}

void SkillPanel::SpriteDraw()
{
	skillPanel_->Draw();
}

void SkillPanel::TextMessageDraw()
{
	D2D1_RECT_F drawRange = { pos_.x, pos_.y - 5.f, pos_.x + 30.f, pos_.y + 50.f };
	D2D1_RECT_F panelNameRange = { 800, 520, 1200, 900 };
	D2D1_RECT_F textRange = { 550, 600, 1200, 700 };
	std::string textColor = "white";
	if (isSelect_) {
		text_->Draw("bestTen", textColor, skillName_, panelNameRange);
		if (drawTime_ > 0) {
			text_->Draw("bestTen_16", textColor, drawText_, textRange);
		}
		else {
			text_->Draw("bestTen_16", textColor, skillText_, textRange);
		}
	}

	if (!isActive_) {
		textColor = "gray";
	}
	if (isSkillGet_) {
		textColor = "yellow";
	}

	if (num_ != 0) {
		std::wstring numText = std::to_wstring(num_);
		text_->Draw("bestTen_16", textColor, L"+" + numText, drawRange);
	}


}

void SkillPanel::TextOpen(Vector2 cursorPos)
{
	std::string textColor = "white";

	if (PanelToCursorHit(cursorPos)) {
		if (drawTimer_ < drawTime_) drawTimer_++;
		if (drawTimer_ >= drawTime_ && skillText_.size() != drawText_.size()) {
			drawTimer_ = 0;
			drawText_ += skillText_.substr(drawText_.size(), 1);
		}

		if (!isActive_) {
			skillPanel_->SetColor({ 0.6f, 0.2f, 0.2f });
		}
		else if (!isSkillGet_) {
			skillPanel_->SetColor({ 0.2f, 0.2f, 0.6f });
		}
		isSelect_ = true;
	}
	else {
		isSelect_ = false;
		drawText_.clear();
		drawTimer_ = 0;
	}

}

bool SkillPanel::PanelToCursorHit(Vector2 cursorPos)
{
	const float panelSizeX = 50 / 2;
	const float panelSizeY = 50 / 2;

	if (cursorPos.x >= pos_.x - panelSizeX && cursorPos.x <= pos_.x + panelSizeX && cursorPos.y >= pos_.y - panelSizeY && cursorPos.y <= pos_.y + panelSizeY) return true;

	return false;
}

void SkillPanel::SkillTextLoad()
{
	std::string line;
	std::string text;
	std::stringstream stream;
	std::wstring skillText;
	int32_t time;

	stream = ExternalFileLoader::GetIns()->ExternalFileOpen("skillText.csv");

	while (getline(stream, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ' ');

		if (word.find("#") == 0) {
			continue;
		}
		if (word.find("hR") == 0 && skillType_ == HammerReturn) {
			line_stream >> text;
		}
		if (word.find("hU") == 0 && skillType_ == HPUP) {
			line_stream >> text;
		}
		if (word.find("aU") == 0 && skillType_ == ATKUP) {
			line_stream >> text;
		}
		if (word.find("dU") == 0 && skillType_ == DEFUP) {
			line_stream >> text;
		}
		if (word.find("sU") == 0 && skillType_ == SPDUP) {
			line_stream >> text;
		}
		if (word.find("fH") == 0 && skillType_ == FallHammer) {
			line_stream >> text;
		}
		if (word.find("hM") == 0 && skillType_ == HyperMode) {
			line_stream >> text;
		}
		if (word.find("em") == 0 && skillType_ == Empty) {
			line_stream >> text;
		}
		if (word.find("time") == 0) {
			line_stream >> time;
		}
	}

	skillText_ = ExternalFileLoader::GetIns()->StringToWstring(text);
	skillText_ = ExternalFileLoader::GetIns()->ReplaceAllWstr(skillText_, L"/", L"\n");
	drawTime_ = time;
	if (skillType_ == HPUP || skillType_ == ATKUP || skillType_ == DEFUP || skillType_ == SPDUP) {
		skillText_ = ExternalFileLoader::GetIns()->ReplaceWstr(skillText_, L"_n", std::to_wstring(num_));
	}

}

void SkillPanel::SetSkillType(SkillType type)
{
	//スキルタイプ
	skillType_ = type;
	//スキルパネルスプライト
	if (type == HPUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_HPUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == ATKUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_ATKUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == DEFUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_DEFUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == SPDUP) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_SPDUp, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == HammerReturn) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_HammerReturn, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == FallHammer) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_FallHammer, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else if (type == HyperMode) {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::skillIcon_HyperMode, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
	else {
		skillPanel_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, pos_, { 0.2f, 0.2f, 0.2f, 1.f }, { 0.5f, 0.5f });
	}
}
