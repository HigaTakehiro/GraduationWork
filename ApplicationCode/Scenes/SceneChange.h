#pragma once
#include "Sprite.h"
class SceneChangeEffect
{
public:
	void Initialize();
	void Change(int num);
	void Draw();
	void SetStart(bool start) { changeStartFlag_ = start; }
	bool GetEnd() { return changeEndFlag_; }
	float GetFadeNum() {return fadeNum; }
private:
	std::unique_ptr<Sprite> fade_;
	std::unique_ptr<Sprite> arrow;
	float fadeNum = 0;
	float fadeTime;
	bool fadeStartFlag_;
	bool fadeEndFlag_;
	bool changeStartFlag_;
	bool changeEndFlag_;
};

