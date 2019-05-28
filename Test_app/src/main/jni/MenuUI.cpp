#include "MenuUI.h"

void MenuUI::init()
{	
	setWindowSettings();
	//setBackgroundColor(glm::vec4(0.9f, 0.9f, 0.95, 1.0f));

   AgkImage* pimg = MEDIA_BANK.getImage(BUMPER_IMAGE);
   playButton = new UISpriteButton_T1(pimg, pimg, pimg, glm::vec2(50.0f, -1), glm::vec2(0));   
   addButton(playButton, "play", glm::vec2(50.0f));

	hide();
}

bool MenuUI::playGameInvoked() {
	return playButton->getButtonState();
}

void MenuUI::show() {
	UIFrame::show();
	setWindowSettings();
}

void MenuUI::setWindowSettings() {
	//agk::SetDisplayAspect(ASPECT);
	agk::SetVirtualResolution(100, 100);
	agk::SetViewZoom(1.0f);
	agk::SetViewOffset(0.0f, 0.0f);
}


void MenuUI::update(bool pPressed, bool pReleased, bool pState)
{
	processButtons(pPressed, pReleased, pState);
}