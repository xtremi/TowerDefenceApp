#pragma once
#include "global.h"
#include "UI/UIFrame.h"
#include "UI/UIButtonTypes.h"
#include "MediaBank.h"


class TowerDirectionMenu : public UIFrame {

public:
	TowerDirectionMenu() : UIFrame() {
		initSprites();
	}

	bool directionButtonWasClicked(td::direction& dir);

private:
	void initSprites();
};