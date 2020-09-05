#include "NextWaveButton.h"


/*
	Called once when the button shows time instead of START
*/
void NextWaveButton::startTimer() {
	text->setVertOffset(timerTextVertOffset);
	text->setTextSize(timerTextSize);
	text->realign();
	text->setFont(MEDIA_BANK.getFont(ASSET::FONT_COUR));
}

/*
	Updates the time shown on the button
*/
void NextWaveButton::setTimer(float time) {
	setText(agk::Str(time, 1));
	if (time < blink_on_start_time && !blinkON) {
		startBlink();
	}
	else if (time > blink_on_start_time && blinkON) {
		stopBlink();
	}
}

void NextWaveButton::startBlink() {
	blinkON = true;
}

void NextWaveButton::stopBlink() {
	blinkON = false;
	blinkTimer = 0.0f;
	setColor(colours::red);
	text->setColor(colours::white);

}

void NextWaveButton::process() {
	
	if (blinkON) {
		blinkTimer += blinkDT;

		float m = (1.0 + agk::SinRad(10.0f*blinkTimer));
		float r = 1.0 - blinkAmp * m;
		float bg = 0.0 + 0.4*blinkAmp*m;

		setColor(glm::vec3(r, bg, bg));
		text->setColor(glm::vec3(r, r, r));

		if (blinkTimer > blink_timer_max_value) blinkTimer = 0.0f;

	}

}