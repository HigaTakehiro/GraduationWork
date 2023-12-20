#pragma once
#include "Sprite.h"
class SceneChangeEffect
{
public:
	void Initialize();
	void Change(int num);
	void Draw();
	void SetFStart(bool start) { fadeStartFlag_ = start; }
	void SetFEnd(bool end) { fadeEndFlag_ = end; }
	bool GetEnd() { return EndFlag_; }
	void SetEnd(bool endFlag) { EndFlag_ = endFlag; }
	bool GetFEnd(){return fadeEndFlag_;}
	bool GetFStart() { return fadeStartFlag_; }
	float GetFadeNum() { return fadeNum; }
	void SetFadeNum(float fadeNum) { this->fadeNum = fadeNum; }
private:
	std::unique_ptr<Sprite> fade_;
	std::unique_ptr<Sprite> arrow;
	float fadeNum = 0;
	float fadeTime;
	bool fadeStartFlag_;
	bool fadeEndFlag_;
	bool EndFlag_;
};

