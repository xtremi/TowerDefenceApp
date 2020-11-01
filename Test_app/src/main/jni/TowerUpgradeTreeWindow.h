#pragma once
#include "UI/UIButtonTypes.h"
#include "UI/UIFrame.h"
#include "MediaBank.h"

#define TOWER_TREE_BUTTON_DEPTH 5
#define TOWER_TREE_FRAME_DEPTH 6
#define TOWER_TREE_FRAME_ITEM_DEPTH 5


/***********************************************/
class TowerUpgradeTreeWindow : public UIFrame
/***********************************************/
{
public:
	TowerUpgradeTreeWindow();

	void init();
	void update(bool pPressed, bool pReleased, bool pState);
	bool returnInvoked();


private:
	UISpriteButton_T1* returnButton = nullptr;
	Sprite*			   background = nullptr;

	void createReturnButton();

};

