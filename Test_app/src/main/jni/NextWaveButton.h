#pragma once
#include "UI/UIButtonTypes.h"
#include "MediaBank.h"

namespace next_wave_button {
	static const float startTextSize = 22.0f;
}

class NextWaveButton : public UISpriteAndTextButton_T2
{
public:
	NextWaveButton(glm::vec2 size) : UISpriteAndTextButton_T2(
			GETIMG(ASSET::IMG_NEXT_WAVE), GETIMG(ASSET::IMG_NEXT_WAVE), GETIMG(ASSET::IMG_NEXT_WAVE),
			"START", glm::vec3(1.0), size, next_wave_button::startTextSize, glm::vec2(0.0f))
	{
		text->setVertOffset(startTextVertOffset);
		text->setFont(MEDIA_BANK.getFont(ASSET::FONT_COUR));
		text->setBold(true);
	}

	void startTimer();
	void setTimer(float time);
	void startBlink();
	void stopBlink();
	void process();

private:
	bool blinkON = false;
	float blinkTimer = 0.0f;
	float blinkAmp = 0.2f;
	float blinkDT = 0.025f;


	const float timerTextSize = 30.0f;
	const float startTextVertOffset = -14.0f;
	const float timerTextVertOffset = -18.0f;
	const float blink_timer_max_value = 1000.0f;
	const float blink_on_start_time = 2.0f;


	virtual void setMainState() {
		sprite->setColor(glm::vec3(1.0, 0.0, 0.0));
	}
	virtual void setSecondaryState() {
		sprite->setColor(glm::vec3(0.0, 0.0, 1.0));
	}
	virtual void setInactiveState() {
		sprite->setColor(glm::vec3(0.7, 0.7, 0.7));
	}
};

