#pragma once
#include "UI/UIButtonTypes.h"
#include "UI/UIFrame.h"
#include "UI/SpriteAnimation.h"
#include "UI/NumberTextSprite.h"

#include "InfoFrameList.h"
#include "WaveHUDframe.h"
#include "PlayerStatsHUDframe.h"

#include "MediaBank.h"
#include <map>
#include "global.h"

#define GAME_HUD_BUTTON_DEPTH 5
#define GAME_HUD_FRAME_DEPTH 6
#define GAME_HUD_FRAME_ITEM_DEPTH 5

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
	bool pauseMusicInvoked();
	bool nextWaveInvoked();
	bool fastButtonInvoked();
	bool debugButtonInvoked();
	
	void showPauseSign();
	void hidePauseSign();

	bool musicButtonState();
	void setMusicButtonState(bool state);

	void setWaveTimer(float t);
	void startWaveTimer();

	void setMoney(int m) { 
		playerStatsHUDframe->setMoney(m);
	}
	void setLife(int l) {
		playerStatsHUDframe->setLife(l);
	}
	void setWaveNumber(int n) { 
		playerStatsHUDframe->setWave(n); 
	}

	void updateWaveMobInfo(WaveDescriptor* nextWaveDescr, MobController* mobController, bool swapCurrentAndNext = true) {
		waveHUDframe->setNextWaveMobInfo(nextWaveDescr, mobController, swapCurrentAndNext);
	}


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
		sideStatsFrame->animateTo("default");
	}
	void sideStatsFrameDuck() {
		sideStatsFrame->animateFromTo("default", "out");
	}

	bool pointerIsOverHUD();

	InfoFrameList* getTowerInfoFrame() { return towerInfoFrame; }
	InfoFrameList* getTowerUpgradeFrame() { return towerUpgradeFrame; }

private:
	std::map<std::string, glm::vec2>	GHpos;
	std::map<std::string, glm::vec2>	GHsize;
	std::map<std::string, double>		GHsize_d;

	InfoFrame*							sideStatsFrame = NULL;	/*TODO: create a side menu frame class with these buttons*/
	UISpriteButton_T1*					mainMenuButton = NULL; 
	UISpriteButton_T1*					fastButton = NULL;		
	UISpriteButton_T1*					debugButton = NULL;
	UISpriteButton_toogle*				pauseButton = NULL;
	UISpriteButton_toogle*				musicButton = NULL;

	WaveHUDframe*						waveHUDframe = NULL;
	PlayerStatsHUDframe*				playerStatsHUDframe = NULL;

	//NextWaveButton*						nextWaveButton = NULL;
	InfoFrameList*						towerInfoFrame = NULL;
	InfoFrameList*						towerUpgradeFrame = NULL;
	//InfoFrame*							upperStatsFrame = NULL;
	
	TextSprite*							pauseSign = NULL;
	//NumberTextSprite*					timerSprite = NULL;
	//NumberTextSprite*					moneySprite = NULL;
	//NumberTextSprite*					waveSprite = NULL;

	void defineDimensions();

	void createWaveHUDframe();
	void createPlayerStatsHUDframe();

	void createMainMenuButton();
	void createPauseButton();
	void createMusicButton();
	void createFastButton();
	void createDebugButton();
	void createPauseSign();

	void createTowerInfoFrame();
	void createTowerUpgradeFrame();	
	void createSideStatsFrame();
	
};

