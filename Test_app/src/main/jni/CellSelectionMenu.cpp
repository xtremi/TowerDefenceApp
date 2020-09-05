#include "CellSelectionMenu.h"


CellSelectionMenu2::CellSelectionMenu2(
	const std::vector<int>&			btnIds,
	const std::vector<AgkImage*>&	btnIconImages)
{
	for (int i = 0; i < btnIconImages.size(); i++) {
		CellSelectionMenuButton* b = new CellSelectionMenuButton(btnIconImages[i]);
		b->hide();
		b->deactivate();
		buttons[btnIds[i]] = b;
	}
	initInfoSprite();
};

void CellSelectionMenu2::display(const glm::vec2& pos, const std::vector<int>& buttonsids) {
	lastPressedButton = -1;
	lastConfirmedButton = -1;

	currentPos = pos;

	activeButtons = buttonsids;
	for (int btnid : activeButtons) {
		buttons[btnid]->activate();
		buttons[btnid]->reset();
		buttons[btnid]->show();		
	}
	infoSprite->show();
	updatePosition();
	active = true;
}

void CellSelectionMenu2::setButtonInactive(int buttonID) {
	if (buttons.find(buttonID) != buttons.end())
		buttons[buttonID]->setActiveState(false);
}

void CellSelectionMenu2::cancel() {
	for (int btnID : activeButtons)
		buttons[btnID]->hide();
	infoSprite->hide();
	infoSprite->setText("");
	active = false;
	//currentBuilding = NULL;
}

bool CellSelectionMenu2::isActive() { return active; }

void CellSelectionMenu2::update() {
	updatePosition();
}


/*
	This is called every frame if the map is panned or zoomed... TODO: check if this is necessary
*/
void CellSelectionMenu2::updatePosition() {
	glm::vec2 spos = worldToScreen(currentPos);
	float dang = 360.0f / ((float)(activeButtons.size()));
	float ang = activeButtons.size() == 1 ? 0.0f : 90.0f;
	
	float scale = 1.0f;
	if (activeButtons.size() > 6) scale = 0.8f;
	else if (activeButtons.size() > 5) scale = 0.85f;
	
	for (int btnID : activeButtons) {
		glm::vec2 bpos = spos + CELL_SELECT_BUTTON_DIST_FROM_CENTER*glm::vec2(agk::Cos(ang), agk::Sin(ang));
		ang += dang;
		buttons[btnID]->setPosition(bpos);
		buttons[btnID]->setDefaultScale(scale);
		buttons[btnID]->setPressedScale(1.2f*scale);
	}
	float infoSpriteDist = activeButtons.size() == 1 ? CELL_SELECT_INFO_DIST_FROM_CENTER / 2.0f : CELL_SELECT_INFO_DIST_FROM_CENTER;
	infoSprite->setPosition(spos + glm::vec2(0.0f, -infoSpriteDist));
	infoSprite->postProcess();
}


/*void CellSelectionMenu2::setCurrentBuilding(Building* building) {
	currentBuilding = building;
}*/

void CellSelectionMenu2::processButtons(bool pPressed, bool pReleased, bool pState) {

	hadButtonPress = false;
	hadButtonConfirmed = false;

	for (int btnID : activeButtons)
		buttons[btnID]->processButtonState(pPressed, pReleased, pState);

	if (pReleased) {
		for (int btnID : activeButtons) {

			if(buttons[btnID]->getButtonState()){
				if (buttons[btnID]->wasPressedOnce()) {
					if (lastPressedButton > 0 && btnID != lastPressedButton) {
						buttons[lastPressedButton]->reset();
					}
					lastPressedButton = btnID;
					hadButtonPress = true;
				}
				else if (buttons[btnID]->wasPressedTwice()) {
					lastConfirmedButton = btnID;
					hadButtonConfirmed = true;
				}
			}

		}
	}

}

bool CellSelectionMenu2::buttonWasPressed(int* pressedButtonID) {
	if (hadButtonPress){
		(*pressedButtonID) = lastPressedButton;

		/*std::string infoText = "";

		if (lastPressedButton == CELL_SELECT_BUTTON_AIM_ID)
			infoText = "Change tower aiming\ndirection";
		else if (lastPressedButton == CELL_SELECT_BUTTON_MEGA_ID)
			infoText = "Upgrade to MEGA tower";
		else if (lastPressedButton == CELL_SELECT_BUTTON_INITIAL_TOWER_ID)
			infoText = "Standard tower\nCost :    10";
		else if (lastPressedButton == CELL_SELECT_BUTTON_SELL_ID) {
			float cost = currentBuilding->value();
			infoText = "Sell tower\nValue  :  " + std::to_string(cost);
		}
		else {
			float cost = currentBuilding->value();
			infoText = "Sell tower\nValue  :  " + std::to_string(cost);
		}*/
			

		//setInfoSpriteText(buttons[lastPressedButton]->infoText());
		return true;
	}	
	pressedButtonID = NULL;
	return false;
}

bool CellSelectionMenu2::buttonWasConfirmed(int* confirmedButtonID) {
	if (hadButtonConfirmed) {
		(*confirmedButtonID) = lastConfirmedButton;
		return true;
	}
	confirmedButtonID = NULL;
	return false;
}

void CellSelectionMenu2::initInfoSprite()
{
	infoSprite = new SpriteAndText(GETIMG(ASSET::IMG_CELL_MENU_INFO_BACK), " ", glm::vec3(1, 1.0, 1.0), glm::vec2(CELL_SELECT_INFO_BACK_SIZE, -1), 15.0f);
	infoSprite->setDepth(CELL_SELECT_BUTTON_BACKGROUND_DEPTH);
	infoSprite->fixToScreen();
	infoSprite->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	infoSprite->getText()->setVertOffset(-25.0f);
	infoSprite->getText()->setBold(true);

}

void CellSelectionMenu2::setInfoSpriteText(const std::string& text) 
{
	infoSprite->setText((char*)text.c_str());
	
}
