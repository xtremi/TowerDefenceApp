#include "template.h"


using namespace AGK;
app App;

/****************************************************/
void app::initWindow()
/****************************************************/
{
	LOGI("app::initWindow()");
	agk::SetVirtualResolution(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
	agk::SetBorderColor(240, 255, 240);
	agk::SetClearColor(255, 255, 255);	
	agk::SetSyncRate(WANTED_FPS, 0);
	agk::SetScissor(0, 0, 0, 0);
	agk::UseNewDefaultFonts(0);

	//agk::SetTextDefaultMagFilter(1);
	LOGI("app::initWindow() - complete");
}

void app::beginLoadingScreen() {

	glm::vec2 centerPos = glm::vec2(VIRTUAL_WIDTH / 2.0f, VIRTUAL_HEIGHT / 2.0f);
	float logoSize = 300.0f;

	AgkImage tempBackgroundImage(glm::vec3(1.0f));
	Sprite tempBackroundSprite(&tempBackgroundImage, glm::vec2(0.0f, 0.0f), glm::vec2(VIRTUAL_WIDTH, VIRTUAL_HEIGHT), AgkHAlign::left, AgkVAlign::top);
	

	AgkImage loadingScreenImage("images/loading_screen/logo.png");
	Sprite loadingScreenSprite(&loadingScreenImage, centerPos, glm::vec2(logoSize));

	int n_loading_frames = 12;
	for (int i = 0; i < n_loading_frames; i++) {
		std::string id = std::to_string(i + 1);
		if (i < 9) id = "0" + id;
		std::string fileName = "images/loading_screen/logo_loading_" + id + ".png";
		loadingImages.push_back(AgkImage(&fileName[0]));
	}

	Sprite loadingScreenSpriteRot;
	for(int i = 0; i < n_loading_frames; i++)
		loadingScreenSpriteRot.addSpriteAnimationFrame(&loadingImages[i]);
	loadingScreenSpriteRot.setSize(glm::vec2(logoSize));
	loadingScreenSpriteRot.setAlignment(AgkHAlign::center, AgkVAlign::center);
	loadingScreenSpriteRot.setPosition(centerPos);
	loadingScreenSpriteRot.playSprite(15.0f, 1);
	
	loadingImages.push_back(tempBackgroundImage);
	loadingImages.push_back(loadingScreenImage);
	loadingSprites.push_back(tempBackroundSprite);
	loadingSprites.push_back(loadingScreenSprite);
	loadingSprites.push_back(loadingScreenSpriteRot);
	
	tempBackroundSprite.setDepth(2);
	loadingScreenSprite.setDepth(1);
	loadingScreenSpriteRot.setDepth(1);

	agk::Sync();

}
void app::endLoadingScreen() {
	for (int i = 0; i < loadingSprites.size(); i++) {
		loadingSprites[i].deleteSprite();
	}
	for (int i = 0; i < loadingImages.size(); i++) {
		loadingImages[i].deleteImage();
	}
}

/****************************************************/
void app::Begin(void)
/****************************************************/
{
	LOGI("app::Begin()");
#ifdef WIN_64_BUILD
	int s = agk::SetFolder("/assets/");
	std::string f = agk::GetFolder();

#else	
	//VIRTUAL_HEIGHT = agk::GetDeviceHeight();
	//VIRTUAL_WIDTH = (int)((float)VIRTUAL_HEIGHT / ASPECT);
#endif

	initWindow();
	beginLoadingScreen();
	MEDIA_BANK.init();	
	menuUI.init();
	gamePlay.init();
	menuUI.show();
	towerUpgrWindow.init();
	endLoadingScreen();

	prevState = GAME_STATE::play;
	state = GAME_STATE::menu_ui;
	LOGI("app::Begin() - complete");
}

/****************************************************/
int app::Loop (void)
/****************************************************/
{
	bool pPressed	= agk::GetPointerPressed();
	bool pReleased	= agk::GetPointerReleased();	
	bool pState		= agk::GetPointerState();
	gamePlay.setPointerState(pPressed, pReleased, pState);

	if(state == GAME_STATE::play){
		gamePlay.update();
	}
	else if (state == GAME_STATE::game_pause) {		
		gamePlay.getHud()->update(pPressed, pReleased, pState);
	}
	else if (state == GAME_STATE::menu_ui){
		menuUI.update(pPressed, pReleased, pState);
	}
	else if (state == GAME_STATE::game_pause_level_completed) {
		
	}	
	else if (state == GAME_STATE::tower_upgrade_tree_window) {
		towerUpgrWindow.update(pPressed, pReleased, pState);
	}

	updateGameState();

	agk::Sync();
	return 0;
}

/****************************************************/
void app::updateGameState()
/****************************************************/
{
	if(state == GAME_STATE::play || state == GAME_STATE::game_pause){

		if (gamePlay.getHud()->goToMainMenuInvoked()) {
			menuUI.show();
			gamePlay.pause(false);
			gamePlay.hide();
			prevState = state;
			state = GAME_STATE::menu_ui;
		}
		else if (gamePlay.getHud()->pauseGameInvoked()) {
				
			if(state == GAME_STATE::play){
				gamePlay.pause(true);
				prevState = state;
				state = GAME_STATE::game_pause;
			}
			else {
				gamePlay.unpause();
				prevState = state;
				state = GAME_STATE::play;
			}
				
		}
		else if (gamePlay.getHud()->pauseMusicInvoked()) {
			gamePlay.toogleBackgroundMusic();
		}
		else if (gamePlay.getHud()->debugButtonInvoked()) {
			gamePlay.toogleDebugLoggerFrame();
		}
		else if (gamePlay.getHud()->towerTreeButtonInvoked()) {
			gamePlay.hide();
			saveViewState();
			setViewOffsetAndZoomLevel(glm::vec2(0.0f), 1.0f);
			towerUpgrWindow.show();			
			prevState = state;
			state = GAME_STATE::tower_upgrade_tree_window;
		}


	}
	else if(state == GAME_STATE::menu_ui){

		if (menuUI.playGameInvoked()) {
			menuUI.hide();
			gamePlay.show();
			if(!gamePlayHasStarted){
				gamePlay.reset(1);
				gamePlayHasStarted = true;
			}
			
			state = prevState;
			if(state == GAME_STATE::play)
				gamePlay.unpause();
		}

	}
	else if (state == GAME_STATE::tower_upgrade_tree_window) {
		if (towerUpgrWindow.returnInvoked()) {
			towerUpgrWindow.hide();
			restoreViewState();
			gamePlay.show();
			state = prevState;
			if (state == GAME_STATE::play)
				gamePlay.unpause(); //hides the pause sign (if it wasn't paused before)
		}
	}

	if (agk::GetResumed()) {
		
	}
}

/****************************************************/
void app::End (void)
/****************************************************/
{
    
}


void app::saveViewState() {
	getCurrentViewOffsetAndZoomLevel(savedViewOffset, savedZoomOffset);
}
void app::restoreViewState() {
	setViewOffsetAndZoomLevel(savedViewOffset, savedZoomOffset);
}

/*
	Get the current view offset and zoom level from
*/
void app::getCurrentViewOffsetAndZoomLevel(glm::vec2& offset, float& zoomLvl)
{
	offset.x = agk::GetViewOffsetX();
	offset.y = agk::GetViewOffsetY();
	zoomLvl = agk::GetViewZoom();
}

void app::setViewOffsetAndZoomLevel(const glm::vec2& offset, float zoomLvl) {
	agk::SetViewOffset(offset.x, offset.y);
	agk::SetViewZoom(zoomLvl);
}

