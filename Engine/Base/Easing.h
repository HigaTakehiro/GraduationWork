#pragma once

class Easing final
{
private:
	Easing() = default;
	~Easing() = default;

public: //ÉÅÉìÉoä÷êî
	static inline float easeIn(float time, float maxTime, float nowPos, float maxPos) {
		maxPos -= nowPos;
		time /= maxTime;

		return maxPos * time * time + nowPos;
	}

	static inline float easeOut(float time, float maxTime, float nowPos, float maxPos) {
		maxPos -= nowPos;
		time /= maxTime;

		return -maxPos * time * (time - 2.0f) + nowPos;
	}

	static inline float easeInOut(float time, float maxTime, float nowPos, float maxPos) {
		maxPos -= nowPos;
		time /= maxTime;

		if (maxTime / 2.0f < time) return maxTime / 2.0f * time * time + nowPos;

		return -maxPos * time * (time - 2.0f) + nowPos;
	}

	static inline float easeInBack(float time, float maxTime, float nowPos, float maxPos, float back = 1.70158f) {
		maxPos -= nowPos;
		time /= maxTime;

		return maxPos * time * time * ((back + 1) * time - back) + nowPos;
	}

	static inline float easeOutBack(float time, float maxTime, float nowPos, float maxPos, float back = 1.70158f) {
		maxPos -= nowPos;
		time = time / maxTime - 1;

		return maxPos * (time * time * ((back + 1) * time * back) + 1) + nowPos;
	}

	static inline float easeOutBounce(float time, float maxTime, float nowPos, float maxPos) {
		if (time <= 0.0f) {
			return nowPos;
		}

		if (time >= maxTime) {
			return maxPos;
		}

		maxPos -= nowPos;
		float timeRate = time / maxTime;

		if (timeRate < (1.0f / 2.75f)) {
			return nowPos * (7.5625f * timeRate * timeRate) + maxPos;
		}
		else if (timeRate < (2.0f / 2.75f)) {
			timeRate -= (1.5f / 2.75f);
			return nowPos * (7.5625f * (timeRate) * timeRate + 0.75f) + maxPos;
		}
		else if (timeRate < (2.5f / 2.75f)) {
			timeRate -= (2.25f / 2.75f);
			return nowPos * (7.5625f * (timeRate) * timeRate + 0.9375f) + maxPos;
		}
		else {
			timeRate -= (2.625f / 2.75f);
			return nowPos * (7.5625f * (timeRate) * timeRate + 0.984375f) + maxPos;
		}
	}
};

