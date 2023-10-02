#include "Button.h"
#include "MouseInput.h"

Button* Button::CreateButton(ImageManager::ImageName imageName, const Vector2& pos, const Vector2& size, const float rot)
{
	Button* ins = new Button();
	ins->Initialize(imageName, pos, size, rot);
	return ins;
}

std::unique_ptr<Button> Button::CreateUniqueButton(ImageManager::ImageName imageName, const Vector2& pos, const Vector2& size, const float rot)
{
	std::unique_ptr<Button> ins = std::make_unique<Button>();
	ins->Initialize(imageName, pos, size, rot);
	return ins;
}

void Button::Initialize(ImageManager::ImageName imageName, const Vector2& pos, const Vector2& size, const float rot)
{
	//変数の初期化
	pos_ = pos;
	size_ = size;
	initSize_ = size;
	rot_ = rot;
	alpha_ = 1.0f;
	initAlpha_ = 1.0f;
	//画像スプライトの生成
	button_ = Sprite::UniquePtrCreate((UINT)imageName, pos_);
	button_->SetSize(size_);
	button_->SetRotation(rot_);
	button_->SetAlpha(alpha_);
	button_->SetAnchorPoint({ 0.5f, 0.5f });
}

void Button::Update()
{
	isClick_ = false;
	//マウスが当たっている場合
	if (MouseHitCheck()) {
		size_ = initSize_ * 0.8f;
		alpha_ = initAlpha_ * 0.5f;
		if (MouseInput::GetIns()->TriggerClick(MouseInput::MouseState::LEFT_CLICK)) {
			isClick_ = true;
		}
	}
	else {
		size_ = initSize_;
		alpha_ = initAlpha_;
	}

	button_->SetPosition(pos_);
	button_->SetSize(size_);
	button_->SetAlpha(alpha_);
}

void Button::Draw()
{
	button_->Draw();
}

bool Button::MouseHitCheck()
{
	Vector2 mousePos = { (float)MouseInput::GetIns()->GetMousePoint().x, (float)MouseInput::GetIns()->GetMousePoint().y };
	Vector2 leftTop = { pos_.x - (initSize_.x * 0.5f), pos_.y - (initSize_.y * 0.5f) };
	Vector2 rightBottom = { pos_.x + (initSize_.x * 0.5f), pos_.y + (initSize_.y * 0.5f) };

	//マウスが矩形の範囲内にあるか判定
	return mousePos.x >= leftTop.x && mousePos.x <= rightBottom.x && mousePos.y >= leftTop.y && mousePos.y <= rightBottom.y;
}
