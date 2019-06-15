#include "CellSelectionMenu.h"


CellSelectionMenu2::CellSelectionMenu2(
	const std::vector<int>& buttonids,
	const std::vector<AgkImage*>& btnImage,
	const std::vector<AgkImage*>& btnImgPressed,
	const std::vector<AgkImage*>& btnImgInactive) 
{
	for (int i = 0; i < btnImage.size(); i++) {
		CellSelectionMenuButton* b = new CellSelectionMenuButton(btnImage[i], btnImgPressed[i], btnImgInactive[i]);
		b->fixToScreen();
		b->hide();
		b->deactivate();
		buttons[buttonids[i]] = b;
	}
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
	active = false;
}

bool CellSelectionMenu2::isActive() { return active; }

void CellSelectionMenu2::update() {
	updatePosition();
}

void CellSelectionMenu2::updatePosition() {
	glm::vec2 spos = worldToScreen(currentPos);
	float dang = 360.0f / ((float)(activeButtons.size()));
	float ang = 0.0f;
	static float R = 100.0f;
	for (int btnID : activeButtons) {
		glm::vec2 bpos = spos + 140.0f*glm::vec2(agk::Cos(ang), agk::Sin(ang));
		ang += dang;
		buttons[btnID]->setPosition(bpos);
	}
}

/*void CellSelectionMenu2::activateUsedButtons() {
	for (int i = 0; i < usedButtons; i++) {
		buttons[i]->show();
		buttons[i]->activate();
	}
}*/


/*void CellSelectionMenu2::setupButtonImageMap() {
	buttonImageMap[cell_button::unknown_button] = button_images(BUY_OTHER_BUTTON_IMAGE, BUY_OTHER_BUTTON_P_IMAGE, BUY_OTHER_BUTTON_I_IMAGE);
	buttonImageMap[cell_button::building_button] = button_images(BUY_BUILDING_BUTTON_IMAGE, BUY_BUILDING_BUTTON_P_IMAGE, BUY_BUILDING_BUTTON_I_IMAGE);
	buttonImageMap[cell_button::tower_default] = button_images(BUY_TOWER_BUTTON_IMAGE, BUY_TOWER_BUTTON_P_IMAGE, BUY_TOWER_BUTTON_I_IMAGE);
	buttonImageMap[cell_button::tower_aim_1] = button_images(UPGRADE_TOWER_BUTTON_IMAGE, UPGRADE_TOWER_BUTTON_P_IMAGE, UPGRADE_TOWER_BUTTON_I_IMAGE);
	buttonImageMap[cell_button::set_building_direction] = button_images(SET_BUILDING_DIR_BUTTON_IMAGE, SET_BUILDING_DIR_BUTTON_P_IMAGE, SET_BUILDING_DIR_BUTTON_I_IMAGE);
}

void CellSelectionMenu2::setButtonImages(cell_button buttonType, int index) {
	button_images* btnimages = &buttonImageMap[buttonType];
	buttons[index]->setImages(GETIMG(btnimages->main), GETIMG(btnimages->secondary), GETIMG(btnimages->inactive));
	buttonIdMap[buttonType] = index;
	buttons[index]->reset();
	buttons[index]->show();
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

/*bool CellSelectionMenu2::buttonWasConfirmed(int btnID) {
	return buttons[activeButtons[btnID]]->wasPressedTwice();
}
bool CellSelectionMenu2::getButtonState(int btnid) {
	return buttons[activeButtons[btnid]]->getButtonState();
}*/

/**************************************************************************/


void CellSelectionMenu::initCellSelectionMenu()
{
	CellSelectionMenuButton* twrB = new CellSelectionMenuButton(GETIMG(BUY_TOWER_BUTTON_IMAGE),		GETIMG(BUY_TOWER_BUTTON_P_IMAGE),		GETIMG(BUY_TOWER_BUTTON_I_IMAGE));
	CellSelectionMenuButton* bldB = new CellSelectionMenuButton(GETIMG(BUY_BUILDING_BUTTON_IMAGE),	GETIMG(BUY_BUILDING_BUTTON_P_IMAGE),	GETIMG(BUY_BUILDING_BUTTON_I_IMAGE));
	CellSelectionMenuButton* othB = new CellSelectionMenuButton(GETIMG(BUY_OTHER_BUTTON_IMAGE),		GETIMG(BUY_OTHER_BUTTON_P_IMAGE),		GETIMG(BUY_OTHER_BUTTON_I_IMAGE));

	twrB->fixToScreen();
	bldB->fixToScreen();
	othB->fixToScreen();

	twrB->setDepth(4);
	bldB->setDepth(4);
	othB->setDepth(4);

	othB->setActiveState(false);

	addButton(twrB, "tower",	glm::vec2(   0.0f,  100.0f), true);
	addButton(bldB, "building", glm::vec2(-100.0f, -100.0f), true);
	addButton(othB, "other",	glm::vec2( 100.0f, -100.0f), true);

	hide();
	disable();
}

void CellSelectionMenu::assignToCell(Cell* c) {
	currentCellPos = c->getPos() + glm::vec2(CELL_SIZE / 2.0f);
	show();
	enable();
	resetButtons();
	update();
}

void CellSelectionMenu::update() {
	setPosition(worldToScreen(currentCellPos));
	realignSprites();
}


void CellSelectionMenu::resetButtons() {
	((CellSelectionMenuButton*)getButton("tower"))->reset();
	((CellSelectionMenuButton*)getButton("building"))->reset();
	((CellSelectionMenuButton*)getButton("other"))->reset();
	getButton("other")->setActiveState(false);
}


bool CellSelectionMenu::buttonWasConfirmed(const std::string& buttonID) {
	if (isInMap<UIButton>(buttonID, &buttons))
		return ((CellSelectionMenuButton*)buttons[buttonID])->wasPressedTwice();
	return false;
}

void CellSelectionMenu::processButtons(bool pressed, bool released, bool pstate) {
	UIFrame::processButtons(pressed, released, pstate);
	if (released) {
		if (getButtonState("tower")) {
			((CellSelectionMenuButton*)buttons["building"])->reset();
			((CellSelectionMenuButton*)buttons["other"])->reset();
		}
		else if (getButtonState("building")) {
			((CellSelectionMenuButton*)buttons["tower"])->reset();
			((CellSelectionMenuButton*)buttons["other"])->reset();
		}
		else if (getButtonState("other")) {
			((CellSelectionMenuButton*)buttons["tower"])->reset();
			((CellSelectionMenuButton*)buttons["building"])->reset();
		}
	}
}
