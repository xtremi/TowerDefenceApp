#pragma once
#include "MediaBank.h"
#include "UI/UIFrame.h"
#include "UI/UIButtonTypes.h"
#include "logger.h"
#include "global.h"


class MenuUI : public UIFrame
{
public:
   MenuUI() : UIFrame(glm::vec2(0)){}

public:
	void init();

	bool playGameInvoked();

	void show();
	void setWindowSettings();
	void update(bool pPressed, bool pReleased, bool pState);

private:
	UISpriteButton_T1* playButton		= NULL;
	UIButton* settingsButton	= NULL;
	UIButton* levelsButton		= NULL;

};