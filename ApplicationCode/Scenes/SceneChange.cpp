#include "SceneChange.h"
#include "ImageManager.h"
#include "Easing.h"
void SceneChangeEffect::Initialize()
{
	fade_ = Sprite::UniquePtrCreate((UINT)ImageManager::ImageName::bar, { 0, 0 }, { 0,0,0,0 }, { 0.0f, 0.0f });
	fadeEndFlag_ = false;
	fadeStartFlag_ = false;
	EndFlag_ = false;
	fadeNum = 0;
	fadeTime = 0;
}

void SceneChangeEffect::Change(int num)
{
		//fade
		if (fadeStartFlag_ == true) {
			if (fadeNum < 1.0f) {
				fadeTime++;
				fadeNum = Easing::easeInOut(fadeTime, 30, 0, 1);
			}
			else if (fadeNum >= 1.0f) {
				fadeStartFlag_ = false;
				EndFlag_ = true;
			}
		}

		if (fadeEndFlag_ == true) {
			if (fadeNum > 0.0f) {
				fadeTime++;
				fadeNum = Easing::easeInOut(fadeTime, 30, 1, 0);
			}
			if (fadeNum <= 0.0f) {
				fadeEndFlag_ = false;
				EndFlag_ = true;
			}
	}
	fade_->SetSize({ 1280, 720 });
	fade_->SetAlpha(fadeNum);
}

void SceneChangeEffect::Draw()
{
	fade_->Draw();
}

