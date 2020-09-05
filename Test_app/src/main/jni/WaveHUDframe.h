#pragma once
#include "InfoFrame.h"
#include "NextWaveButton.h"
#include "WaveDescriptor.h"
#include "MobController.h"

#define NEXT_WAVE_BUTTON_SIZE 80.0f

class WaveHUDframe : public InfoFrame
{
public:
	WaveHUDframe(glm::vec2 size,int depth) : InfoFrame(size, depth, 0, "Wave") {
		bgsize = size;

		glm::vec2 btnPos(bgsize.x - NEXT_WAVE_BUTTON_SIZE/ASPECT - 10.0f, bgsize.y/2.0);
		createNextWaveButton(btnPos, depth - 1);
		initMobSprites(depth - 1);
	}


	bool nextWaveButtonState() { return nextWaveButton->getButtonState(); }

	void setTimer(float time) {
		nextWaveButton->setTimer(time);
	}
	void startTimer() {
		nextWaveButton->startTimer();
		nextWaveButton->stopBlink();
	}
	void processNextWaveButtonBlink(){
		nextWaveButton->process();
	}

	void setNextWaveMobInfo(WaveDescriptor* nextWaveDescr, MobController* mobController, bool swapCurrentAndNext = true);

private:
	NextWaveButton* nextWaveButton	= NULL;
	WaveDescriptor* nextWaveDescr	= NULL;
	AgkImage*		noMobImage		= NULL;
	glm::vec2		bgsize;
	float			mobSpriteSizeY = 0.0f;
	const int		n_mobs_per_col = 3;
	const int		n_col		   = 2;
	bool			col1isCurrent = true;


	void createNextWaveButton(const glm::vec2& pos, int depth);
	void createWaveDescriptorHeaders(int depth, const glm::vec2& pos, float dX);	
	void initMobSprites(int depth);
	void setNextMobsAsCurrent();
	
	void addMobTextSprite(const glm::vec2& pos, float txtSize, int depth, const std::string& id);
	void addMobSprite(const glm::vec2& pos, const glm::vec2& size, int depth, const std::string& id);
};

