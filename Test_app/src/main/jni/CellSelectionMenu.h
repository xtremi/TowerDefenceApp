#pragma once
#include "UI/UIFrame.h"
#include "UI/UIButtonTypes.h"
#include "MediaBank.h"
#include "Cell.h"
#include "Tower.h"

class CellSelectionMenuButton : public UISpriteButton
{
public:
	CellSelectionMenuButton() : 
		UISpriteButton(GETIMG(BUY_OTHER_BUTTON_IMAGE), glm::vec2(150.f)) {
		setShape(AgkSpriteShape::circle);
		setDepth(4);
	}
	CellSelectionMenuButton(AgkImage* img, AgkImage* imgp, AgkImage* imgi) 
		: UISpriteButton(img, glm::vec2(150.f)){
		mainimg = img;
		secimg = imgp;
		inactiveimg = imgi;
		setShape(AgkSpriteShape::circle);
		setDepth(4);
	}
	
public:
	void setMainImage(AgkImage* img) { mainimg = img; }
	void setSecondaryImage(AgkImage* img) { secimg = img; }
	void setInactiveImage(AgkImage* img) { inactiveimg = img; }
	void setImages(AgkImage* img, AgkImage* imgp, AgkImage* imgi) {
		setMainImage(img);
		setSecondaryImage(imgp);
		setInactiveImage(imgi);
	}

	bool wasPressedTwice() {
		return _wasPressedTwice;
	}
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

private:
	AgkImage* mainimg = NULL;
	AgkImage* secimg = NULL;
	AgkImage* inactiveimg = NULL;

	void setMainState_clicked() {
		setScale(1.2f);
		setImage(secimg);

	}
	void setMainState_normal() {
		setScale(1.0f);
		setImage(mainimg);
	}
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
	void setSecondaryState() {
		setScale(1.25f);
		//setImage(secimg);
	}
	void setInactiveState() {
		setScale(1.0f);
		setImage(inactiveimg);
	}
	bool cursorOverButton() {
		return isHoovered();
	}

	bool _wasPressedOnce = false;
	bool _wasPressedTwice = false;
};



/*enum class cell_button
{
	unknown_button,
	building_button,
	tower_default,
	tower_aim_1,
	set_building_direction
};*/


class CellSelectionMenu2 {

	struct button_images {
		button_images() {};
		button_images(const std::string& m, const std::string& s, const std::string& i) {
			main = m; secondary = s; inactive = i;
		};
		std::string main;
		std::string secondary;
		std::string inactive;
	};
	
public:
	CellSelectionMenu2(
		const std::vector<int>& buttonids,
		const std::vector<AgkImage*>& btnImage, 
		const std::vector<AgkImage*>& btnImgPressed, 
		const std::vector<AgkImage*>& btnImgInactive);

	//void initButtons();
	//void assignToCell(Cell* c);
	void display(const glm::vec2& pos, const std::vector<int>& buttonsids);

	void processButtons(bool pPressed, bool pReleased, bool pState);
	bool buttonWasConfirmed(int* btnid);
	bool buttonWasPressed(int* btnid);
	//bool buttonWasConfirmed(int btnid);
	//bool getButtonState(int btnid);

	void cancel();
	bool isActive();
	void update();

private:
	//std::map<int, button_images>	buttonImageMap;
	std::map<int, CellSelectionMenuButton*>	buttons;
	std::vector<int> activeButtons;
	//std::map<cell_button, int>				buttonIdMap;
	//int maxbuttons	= 0;
	//int usedButtons	= 0;
	glm::vec2 currentPos;
	bool active = false;

	int lastPressedButton = -1;
	int lastConfirmedButton = -1;
	bool hadButtonPress = false;
	bool hadButtonConfirmed = false;

	void updatePosition();
	//void activateUsedButtons();	
	//void setupButtonImageMap();
	//void setButtonImages(cell_button buttonType, int index);


};

class CellSelectionMenu  : public UIFrame
{
public:
	CellSelectionMenu() : UIFrame()
	{
		initCellSelectionMenu();
	}
	
	void initCellSelectionMenu();
	void assignToCell(Cell* c);

	void update();
	void resetButtons();

	bool buttonWasConfirmed(const std::string& buttonID);
	void processButtons(bool pressed, bool released, bool pstate);

private:
	glm::vec2 currentCellPos;
};
