#pragma once
#include "UI/UIFrame.h"
#include "UI/UIButtonTypes.h"
#include "MediaBank.h"
#include "Cell.h"
//#include "Tower.h"

#define CELL_SELECT_INFO_DIST_FROM_CENTER	 200.0f
#define CELL_SELECT_INFO_BACK_SIZE			 200.0f
#define CELL_SELECT_BUTTON_DIST_FROM_CENTER  100.0f
#define CELL_SELECT_BUTTON_SIZE				 100.0f
#define CELL_SELECT_BUTTON_ICON_DEPTH		 2
#define CELL_SELECT_BUTTON_BACKGROUND_DEPTH	 3
#define CELL_SELECT_BUTTON_PRESS_SCALE		 1.2f
						
#define CELL_SELECT_BUTTON_INITIAL_TOWER_ID	 1
#define CELL_SELECT_BUTTON_AIM_ID			 10
#define CELL_SELECT_BUTTON_MEGA_ID			 11
#define CELL_SELECT_BUTTON_SELL_ID			 12

class CellSelectionMenuButton : public UISpriteButton
{
public:
	CellSelectionMenuButton(AgkImage* _iconImg) 
		: UISpriteButton(GETIMG(ASSET::IMG_CELL_MENU_BTN), glm::vec2(CELL_SELECT_BUTTON_SIZE))
	{		
		iconSprite = new Sprite(_iconImg, glm::vec2(0.0f), glm::vec2(CELL_SELECT_BUTTON_SIZE));
		fixToScreen();
		setShape(AgkSpriteShape::circle);
		iconSprite->setShape(AgkSpriteShape::circle);
		setDepth(CELL_SELECT_BUTTON_BACKGROUND_DEPTH);
		iconSprite->setDepth(CELL_SELECT_BUTTON_ICON_DEPTH);

		//buttonInfoText = btnInfoText;
	}

	using AgkSprite::setPosition;
	using Sprite::setState;
	using Sprite::setScale;
public:
	
	//std::string infoText() { return buttonInfoText; }
	void setScale(float sx, float sy) {
		Sprite::setScale(sx, sy);
		iconSprite->setScale(sx, sy);
	}
	void setPosition(float x, float y) {
		AgkSprite::setPosition(x, y);
		iconSprite->setPosition(x, y);
	}
	void setState(bool state) {
		Sprite::setState(state);
		iconSprite->setState(state);
	}
	void fixToScreen() {
		Sprite::fixToScreen();
		iconSprite->fixToScreen();
	}

	//The button action was confirmed:
	bool wasPressedTwice() {
		return _wasPressedTwice;
	}
	//The button was selected:
	bool wasPressedOnce() {
		return _wasPressedOnce;
	}
	void reset() {
		if (!isActive()) 
			setInactiveState();
		else
			setMainState_normal();
		_wasPressedOnce = false;
		_wasPressedTwice = false;		
	}

	void setDefaultScale(float s) { 
		setScale(s);
		defaultScale = s; 
	}
	void setPressedScale(float s) { pressedScale = s; }

private:
	Sprite* iconSprite = NULL;
	float defaultScale = 1.0f;
	float pressedScale = CELL_SELECT_BUTTON_PRESS_SCALE;

	//Set state of image when not selected:
	void setMainState_normal() {
		setScale(defaultScale);
		setImage(GETIMG(ASSET::IMG_CELL_MENU_BTN));
	}
	//Set state of image when selected:
	void setMainState_clicked() {
		setScale(pressedScale);
		setImage(GETIMG(ASSET::IMG_CELL_MENU_BTN_SEL));
	}
	//Set state of image while being pressed:
	void setSecondaryState() {
		setScale(pressedScale);
	}

	//Set state of image when its deactivated (not clickable):
	void setInactiveState() {
		setScale(defaultScale);
		setImage(GETIMG(ASSET::IMG_CELL_MENU_BTN_INC));
	}

	//Set state of image ..... :
	void setMainState() {
		
		if (!_wasPressedOnce){
			if (getButtonState()) {
				setMainState_clicked();
				_wasPressedOnce = true;
				_wasPressedTwice = false;
			}
			else
				setMainState_normal();
		}
		else{
			if (getButtonState()) {
				setMainState_normal();
				_wasPressedOnce = false;
				_wasPressedTwice = true;
			}
			else
				setMainState_clicked();
		}
		
	}


	bool cursorOverButton() {
		return isHoovered();
	}

	bool _wasPressedOnce = false;
	bool _wasPressedTwice = false;
};


class CellSelectionMenu2 {
	
public:
	CellSelectionMenu2(
		const std::vector<int>&			btnIds,
		const std::vector<AgkImage*>&	btnIconImages);

	void display(const glm::vec2& pos, const std::vector<int>& buttonsids);

	void processButtons(bool pPressed, bool pReleased, bool pState);
	bool buttonWasConfirmed(int* btnid);
	bool buttonWasPressed(int* btnid);

	void setButtonInactive(int buttonID);
	void cancel();
	bool isActive();
	void update();

	void initInfoSprite();
	void setInfoSpriteText(const std::string& text);

	//void setCurrentBuilding(Building* building);

private:
	std::map<int, CellSelectionMenuButton*>	buttons;
	std::vector<int>						activeButtons;

	//Building* currentBuilding = NULL;

	glm::vec2 currentPos;
	bool active = false;

	SpriteAndText* infoSprite = NULL;

	int lastPressedButton = -1;
	int lastConfirmedButton = -1;
	bool hadButtonPress = false;
	bool hadButtonConfirmed = false;

	void updatePosition();

};
