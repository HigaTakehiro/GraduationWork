#include "SceneChange.h"
#include "ImageManager.h"
#include "Easing.h"
void SceneChangeEffect::Initialize()
{
	fade_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 0, 0 }, { 0,0,0,0 }, { 0.0f, 0.0f });
	fadeEndFlag_ = false;
	fadeStartFlag_ = false;
	changeStartFlag_ = false;
	changeEndFlag_ = false;
	fadeNum = 0;
	fadeTime = 0;
}

void SceneChangeEffect::Change(int num)
{
	if (num == 0) {
		//fade
		if (changeStartFlag_ == true) {
			fadeStartFlag_ = true;
		}
		if (fadeStartFlag_ == true) {
			if (fadeNum < 1.0f) {
				fadeTime++;
				fadeNum = Easing::easeInOut(fadeTime, 30, 0, 1);
			}
			else if (fadeNum >= 1.0f) {
				changeStartFlag_ = false;
				fadeStartFlag_ = false;
				fadeEndFlag_ = true;
				fadeTime = 0;
			}
		}

		if (fadeEndFlag_ == true) {
			if (fadeNum > 0.0f) {
				fadeTime++;
				fadeNum = Easing::easeInOut(fadeTime, 30, 1, 0);
			}
			if (fadeNum <= 0.0f) {
				fadeEndFlag_ = false;
				changeEndFlag_ = true;
			}
		}


	}
	fade_->SetSize({ 1280, 720 });
	fade_->SetAlpha(fadeNum);
}

void SceneChangeEffect::Draw()
{
	fade_->Draw();
}

