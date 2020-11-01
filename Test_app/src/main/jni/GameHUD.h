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
	bool towerTreeButtonInvoked();
	
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

	InfoFrame*							sideStatsFrame = nullptr;	/*TODO: create a side menu frame class with these buttons*/
	UISpriteButton_T1*					mainMenuButton = nullptr;
	UISpriteButton_T1*					fastButton = nullptr;
	UISpriteButton_T1*					debugButton = nullptr;
	UISpriteButton_toogle*				pauseButton = nullptr;
	UISpriteButton_toogle*				musicButton = nullptr;
	UISpriteButton_T1*					towerTreeButton = nullptr;

	WaveHUDframe*						waveHUDframe = nullptr;
	PlayerStatsHUDframe*				playerStatsHUDframe = nullptr;

	//NextWaveButton*						nextWaveButton = nullptr;
	InfoFrameList*						towerInfoFrame = nullptr;
	InfoFrameList*						towerUpgradeFrame = nullptr;
	//InfoFrame*							upperStatsFrame = nullptr;
	
	TextSprite*							pauseSign = nullptr;
	//NumberTextSprite*					timerSprite = nullptr;
	//NumberTextSprite*					moneySprite = nullptr;
	//NumberTextSprite*					waveSprite = nullptr;

	void defineDimensions();

	void createWaveHUDframe();
	void createPlayerStatsHUDframe();

	void createMainMenuButton();
	void createPauseButton();
	void createMusicButton();
	void createFastButton();
	void createDebugButton();
	void createPauseSign();
	void createTowerTreeButton();

	void createTowerInfoFrame();
	void createTowerUpgradeFrame();	
	void createSideStatsFrame();
	
};

