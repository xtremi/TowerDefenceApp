#include "TowerUpgradeTreeWindow.h"


TowerUpgradeTreeWindow::TowerUpgradeTreeWindow(){}

void TowerUpgradeTreeWindow::init() {
	createReturnButton();

	background = new Sprite(colours::black, glm::vec2(0.0f), glm::vec2(VIRTUAL_WIDTH, VIRTUAL_HEIGHT), 
		AgkHAlign::left, AgkVAlign::top);
	this->addSprite(background, "background", glm::vec2(0.0f));
	realignSprites();
	hide();
}

void TowerUpgradeTreeWindow::update(bool pPressed, bool pReleased, bool pState) {
	processButtons(pPressed, pReleased, pState);
	//other updates....
}
bool TowerUpgradeTreeWindow::returnInvoked() {
	return returnButton->getButtonState();
}


void TowerUpgradeTreeWindow::createReturnButton() {
	
	AgkImage* img = MEDIA_BANK.getImage(ASSET::IMG_RETURN);
	AgkImage* img_p = MEDIA_BANK.getImage(ASSET::IMG_RETURN_P);
	returnButton = new UISpriteButton_T1(img, img_p, img, glm::vec2(50.0f));
	returnButton->setAlignment(AgkHAlign::right, AgkVAlign::top);
	returnButton->setDepth(TOWER_TREE_BUTTON_DEPTH);
	returnButton->fixToScreen();
	returnButton->setScreenMode();
	this->addButton(returnButton, "", glm::vec2(VIRTUAL_WIDTH, 5.0f));
}