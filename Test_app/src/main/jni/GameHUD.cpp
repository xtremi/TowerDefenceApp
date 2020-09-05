#include "GameHUD.h"

void GameHUD::defineDimensions() {

	GHpos = 
	{
		{"frame-tower-info",	glm::vec2(5.0f, 100.0f)},
		{"frame-tower-upgrade", glm::vec2(5.f, VIRTUAL_HEIGHT - 10.0f - 175.0f)},		//out pos		
		{"frame-side-stats",	glm::vec2(VIRTUAL_WIDTH - 75.0f, 100.0f)},		//out pos
		{"button-menu",			glm::vec2(12.5f,   5.0f)},
		{"button-pause",		glm::vec2(12.5f,  65.0f)},
		{"button-music",		glm::vec2(12.5f, 125.0f)},
		{"button-fast",			glm::vec2(12.5f, 185.0f)},
		{"button-debug",		glm::vec2(12.5f, 245.0f)},
		{"pause-sign",			glm::vec2(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2)},
		//{"button-next-wave",	glm::vec2(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT-75.0f)},
		{"player-HUD-stats",	glm::vec2(75.0f, 10.0f)},
		{"wave-HUD",			glm::vec2(50.0f, VIRTUAL_HEIGHT - 125.0f)}
	};
	GHsize = 
	{
		{"frame-tower-info",	glm::vec2(170.0f, 100.0f)},
		{"frame-tower-upgrade", glm::vec2(VIRTUAL_WIDTH - 10.f, 175.0f)},		
		{"frame-side-stats",	glm::vec2(75.0f, 300.0f)},
		{"button-menu",			glm::vec2(50.0f)},
		{"button-pause",		glm::vec2(50.0f)},
		{"button-music",		glm::vec2(50.0f)},
		{"button-fast",			glm::vec2(50.0f)},
		{"button-debug",		glm::vec2(50.0f)},
		//{"button-next-wave",	glm::vec2(150.0f, -1.0f)},
		{"player-HUD-stats",	glm::vec2(350.0f, 35.0f)},
		{"wave-HUD",			glm::vec2(400.0f, 110.0f)}
		
	};

	GHsize_d = 
	{
		{"pause-sign", 100.0f}
	};
}



/****************************************************/
GameHUD::GameHUD()
/****************************************************/
{}
/****************************************************/
void GameHUD::init()
/****************************************************/
{
	defineDimensions();

	createTowerInfoFrame();
	createTowerUpgradeFrame();
	createSideStatsFrame();

	createPauseSign();

	createPauseButton();
	createMusicButton();
	createMainMenuButton();	
	createFastButton();
	createDebugButton();
	
	createWaveHUDframe();
	createPlayerStatsHUDframe();

	realignSprites();
	
}
void GameHUD::createMainMenuButton() {
	std::string id = "button-menu";
	AgkImage* img = MEDIA_BANK.getImage(ASSET::IMG_MENU);
	AgkImage* img_p = MEDIA_BANK.getImage(ASSET::IMG_MENU);
	mainMenuButton = new UISpriteButton_T1(img, img_p, img, GHsize[id]);
	mainMenuButton->setAlignment(AgkHAlign::left, AgkVAlign::top);
	mainMenuButton->setDepth(GAME_HUD_FRAME_ITEM_DEPTH);
	mainMenuButton->fixToScreen();
	mainMenuButton->setScreenMode();
	frames["frame-side-stats"]->addButton(mainMenuButton, id, GHpos[id]);
}
void GameHUD::createPauseButton() {
	std::string id = "button-pause";
	AgkImage* imgPause = MEDIA_BANK.getImage(ASSET::IMG_PAUSE);
	AgkImage* imgPause_p = MEDIA_BANK.getImage(ASSET::IMG_PAUSE_P);
	AgkImage* imgResume = MEDIA_BANK.getImage(ASSET::IMG_RESUME);
	AgkImage* imgResume_p = MEDIA_BANK.getImage(ASSET::IMG_RESUME_P);
	pauseButton = new UISpriteButton_toogle(imgPause, imgPause_p, imgResume, imgResume_p, GHsize[id]);
	pauseButton->setAlignment(AgkHAlign::left, AgkVAlign::top);
	pauseButton->setDepth(GAME_HUD_FRAME_ITEM_DEPTH);
	pauseButton->fixToScreen();
	pauseButton->setScreenMode();
	frames["frame-side-stats"]->addButton(pauseButton, id, GHpos[id]);
}

void GameHUD::createFastButton(){
	std::string id = "button-fast";
	AgkImage* img = MEDIA_BANK.getImage(ASSET::IMG_FAST);
	AgkImage* img_p = MEDIA_BANK.getImage(ASSET::IMG_FAST_P);
	fastButton = new UISpriteButton_T1(img, img_p, img, GHsize[id]);
	fastButton->setAlignment(AgkHAlign::left, AgkVAlign::top);
	fastButton->setDepth(GAME_HUD_FRAME_ITEM_DEPTH);
	fastButton->fixToScreen();
	fastButton->setScreenMode();
	frames["frame-side-stats"]->addButton(fastButton, id, GHpos[id]);
}
void GameHUD::createDebugButton() {
	std::string id = "button-debug";
	AgkImage* img = MEDIA_BANK.getImage(ASSET::IMG_DEBUG);
	AgkImage* img_p = MEDIA_BANK.getImage(ASSET::IMG_DEBUG_P);
	debugButton = new UISpriteButton_T1(img, img_p, img, GHsize[id]);
	debugButton->setAlignment(AgkHAlign::left, AgkVAlign::top);
	debugButton->setDepth(GAME_HUD_FRAME_ITEM_DEPTH);
	debugButton->fixToScreen();
	debugButton->setScreenMode();
	frames["frame-side-stats"]->addButton(debugButton, id, GHpos[id]);
}

void GameHUD::createMusicButton() {
	std::string id = "button-music";
	AgkImage* imgON = MEDIA_BANK.getImage(ASSET::IMG_MUSIC_ON);
	AgkImage* imgONp = MEDIA_BANK.getImage(ASSET::IMG_MUSIC_ON_P);
	AgkImage* imgOFF = MEDIA_BANK.getImage(ASSET::IMG_MUSIC_OFF);
	AgkImage* imgOFFp = MEDIA_BANK.getImage(ASSET::IMG_MUSIC_OFF_P);
	musicButton = new UISpriteButton_toogle(imgON, imgONp, imgOFF, imgOFFp, GHsize[id]);
	musicButton->setAlignment(AgkHAlign::left, AgkVAlign::top);
	musicButton->setDepth(GAME_HUD_FRAME_ITEM_DEPTH);
	musicButton->fixToScreen();
	musicButton->setScreenMode();
	frames["frame-side-stats"]->addButton(musicButton, id, GHpos[id]);
}


void GameHUD::createPauseSign() {
	std::string id = "pause-sign";
	pauseSign = new TextSprite((char*)"Pause", glm::vec3(0.9f), glm::vec2(0), GHsize_d[id], AgkHAlign::center, AgkVAlign::center);
	pauseSign->setBold(true);
	pauseSign->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	pauseSign->fixToScreen();
	this->addSprite(pauseSign, id, GHpos[id]);
	hidePauseSign();
}

void GameHUD::createWaveHUDframe() {
	std::string id = "wave-HUD";
	waveHUDframe = new WaveHUDframe(GHsize[id], GAME_HUD_FRAME_DEPTH);
	addFrame(waveHUDframe, id, GHpos[id]);
}

void GameHUD::createPlayerStatsHUDframe() {
	std::string id = "player-HUD-stats";
	playerStatsHUDframe = new PlayerStatsHUDframe(GHsize[id], GAME_HUD_FRAME_DEPTH);
	addFrame(playerStatsHUDframe, id, GHpos[id]);
}

void GameHUD::createTowerInfoFrame(){
	std::string id = "frame-tower-info";
	towerInfoFrame = new InfoFrameList(GHsize[id], GAME_HUD_FRAME_DEPTH, 0, "Tower info");	
	addFrame(towerInfoFrame, id, GHpos[id]);
}

void GameHUD::createTowerUpgradeFrame(){
	std::string id = "frame-tower-upgrade";
	towerUpgradeFrame = new InfoFrameList(GHsize[id], GAME_HUD_FRAME_DEPTH, 0, "Tower upgrade");
	addFrame(towerUpgradeFrame, id, GHpos[id]);
	
	towerUpgradeFrame->setListItemValue("whautup", 1.23, 2);
	towerUpgradeFrame->setListItemValue("Range", 1.23, 2);
	towerUpgradeFrame->setListItemValue("Damage", 11.0, 2);
	
	SpriteAnimation::animation_state dfState(0.25f, glm::vec2(0.f, GHsize[id].y + 12.f + 10.0f), 0.0f, 1.0f, movement_type::linear, interpolation_type::cubic);
	SpriteAnimation::animation_state outState(0.25f, glm::vec2(0.f, 0.f), 0.0f, 1.0f, movement_type::linear, interpolation_type::cubic);
	towerUpgradeFrame->setDefaultState(dfState);
	towerUpgradeFrame->addState("out", outState);
	towerUpgradeFrame->setHideOnDefaultState();
	addSpriteAnimation(towerUpgradeFrame, id);
	
}


void GameHUD::createSideStatsFrame(){
	std::string id = "frame-side-stats";
	sideStatsFrame = new InfoFrame(GHsize[id], GAME_HUD_FRAME_DEPTH, 1, "Menu");
	addFrame(sideStatsFrame, id, GHpos[id]);

	SpriteAnimation::animation_state inState(0.25f, glm::vec2(GHsize[id].x + 10.0f, 0.f), 0.0f, 1.0f, movement_type::linear, interpolation_type::cubic);
	SpriteAnimation::animation_state outState(0.25f, glm::vec2(0.f, 0.f), 0.0f, 1.0f, movement_type::linear, interpolation_type::cubic);
	sideStatsFrame->setDefaultState(outState);
	sideStatsFrame->addState("in", inState);
	addSpriteAnimation(sideStatsFrame, id);
}



void GameHUD::setWaveTimer(float v) {
	waveHUDframe->setTimer(v);
}
void GameHUD::startWaveTimer() {
	waveHUDframe->startTimer();
}

void GameHUD::update(bool pPressed, bool pReleased, bool pState)
{
	processButtons(pPressed, pReleased, pState);
	processAnimations();	
	waveHUDframe->processNextWaveButtonBlink();
}

bool GameHUD::goToMainMenuInvoked(){
	return mainMenuButton->getButtonState();
}
bool GameHUD::pauseGameInvoked(){
	return pauseButton->getButtonState();
}
bool GameHUD::fastButtonInvoked() {
	return fastButton->getButtonState();
}
bool GameHUD::debugButtonInvoked() {
	return debugButton->getButtonState();
}

bool GameHUD::pauseMusicInvoked() {	
	return musicButton->getButtonState();
}
bool  GameHUD::nextWaveInvoked() {
	return  waveHUDframe->nextWaveButtonState();
}
void GameHUD::showPauseSign(){
	pauseSign->show();
}
void GameHUD::hidePauseSign(){
	pauseSign->hide();
}

bool GameHUD::musicButtonState() {
	return musicButton->buttonState();
}
void GameHUD::setMusicButtonState(bool musicButtonState) {
	musicButton->setButtonState(musicButtonState);
}

bool GameHUD::pointerIsOverHUD() {

	return (
		towerInfoFrame->isHoovered()		||
		towerUpgradeFrame->isHoovered()		||		
		sideStatsFrame->isHoovered()		||
		pauseButton->isHoovered()			||
		musicButton->isHoovered()			||
		mainMenuButton->isHoovered()		||
		playerStatsHUDframe->isHoovered()	||
		waveHUDframe->isHoovered()		
		);
}