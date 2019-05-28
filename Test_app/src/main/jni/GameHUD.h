#pragma once
#include "UI/UIButtonTypes.h"
#include "UI/UIFrame.h"
#include "UI/SpriteAnimation.h"
#include "UI/NumberTextSprite.h"
#include "InfoFrameList.h"
#include "MediaBank.h"
#include <map>
#include "global.h"



#define GAME_HUD_BUTTON_DEPTH 5



/***********************************************/
class GameHUD : public UIFrame
/***********************************************/
{
public:
	GameHUD();

	void init();
	void update(bool pPressed, bool pReleased, bool pState);
	bool goToMainMenuInvoked();
	bool pauseGameInvoked();

	void incrTimer(float dt);
	void setTimer(float t);
	float getTimer();
	void setMoney(float m) { moneySprite->setValue(m, 0); }
	void setWaveNumber(int n) { waveSprite->setValue(n, 0); }

	void showPauseSign();
	void hidePauseSign();

	void towerUpgradeFrameAppear() { 
		if(towerUpgradeFrame->getCurrentToState() != "out"){
			towerUpgradeFrame->enable();
			towerUpgradeFrame->show();
			towerUpgradeFrame->animateFromTo("default", "out");
		}
	}
	void towerUpgradeFrameDuck() {
		if (towerUpgradeFrame->getCurrentToState() != "default"){
			towerUpgradeFrame->animateFromTo("out", "default");
		}
	}
	void sideStatsFrameAppear() {
		sideStatsFrame->animateFromTo("default", "out");
	}
	void sideStatsFrameDuck() {
		sideStatsFrame->animateTo("default");
	}

	bool pointerIsOverHUD();

	InfoFrameList* getTowerInfoFrame() { return towerInfoFrame; }
	InfoFrameList* getTowerUpgradeFrame() { return towerUpgradeFrame; }

private:
	UISpriteButton_T1* mainMenuButton = NULL;
	UISpriteButton_T1* pauseButton = NULL;
	UISpriteButton_T2* megaTowerButton = NULL;
	void createMainMenuButton();
	void createPauseButton();
	void createMegaTowerButton();

	TextSprite* pauseSign = NULL;
	NumberTextSprite* timerSprite = NULL;
	NumberTextSprite* moneySprite = NULL;
	NumberTextSprite* waveSprite = NULL;
	void createPauseSign();

	InfoFrameList* towerInfoFrame		= NULL;
	InfoFrameList* towerUpgradeFrame	= NULL;
	InfoFrame* upperStatsFrame		= NULL;
	InfoFrame* sideStatsFrame		= NULL;
	void createTowerInfoFrame();
	void createTowerUpgradeFrame();
	void createUpperStatsFrame();
	void createSideStatsFrame();

	std::map<std::string, glm::vec2>	GHpos;
	std::map<std::string, glm::vec2>	GHsize;
	std::map<std::string, double>		GHsize_d;
	void defineDimensions();
};

