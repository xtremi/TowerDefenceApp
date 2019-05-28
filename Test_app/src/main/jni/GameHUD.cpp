#include "GameHUD.h"

void GameHUD::defineDimensions() {

	GHpos = 
	{
		{"frame-tower-info",	glm::vec2(5.0f, 100.0f)},
		{"frame-tower-upgrade", glm::vec2(5.f, VIRTUAL_HEIGHT - 10.0f - 175.0f)},		//out pos
		{"frame-upper-stats",	glm::vec2(100.0f, 10.0f)},
		{"frame-side-stats",	glm::vec2(395.0f, 100.0f)},		//out pos
		{"button-menu",			glm::vec2(10.0f, 1.0f)},
		{"button-pause",		glm::vec2(VIRTUAL_WIDTH - 10.0f, 10.0f)},
		{"pause-sign",			glm::vec2(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2)},
		{"button-mega",			glm::vec2(VIRTUAL_WIDTH - 10.0f, VIRTUAL_HEIGHT / 2)}
	};
	GHsize = 
	{
		{"button-menu",			glm::vec2(50.0f)},
		{"button-pause",		glm::vec2(50.0f)},
		{"frame-tower-info",	glm::vec2(170.0f, 400.0f)},
		{"frame-tower-upgrade", glm::vec2(VIRTUAL_WIDTH - 10.f, 175.0f)},
		{"frame-upper-stats",	glm::vec2(350.0f, 35.0f)},
		{"frame-side-stats",	glm::vec2(150.0f, 400.0f)},
		{"button-mega",			glm::vec2(100.0f, 20.0f)}
	};

	GHsize_d = 
	{
		{"pause-sign", 200.0f}
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
	LOGI("GameHUD::init()");
	defineDimensions();
	createPauseButton();
	createMainMenuButton();
	createPauseSign();
	createMegaTowerButton();

	createTowerInfoFrame();
	createTowerUpgradeFrame();
	createUpperStatsFrame();
	createSideStatsFrame();
	
	realignSprites();
	LOGI("GameHUD::init() - complete");
}
void GameHUD::createMainMenuButton() {
	std::string id = "button-menu";
	AgkImage* mmimg = MEDIA_BANK.getImage(MENU_IMAGE);
	mainMenuButton = new UISpriteButton_T1(mmimg, mmimg, mmimg, GHsize[id], glm::vec2(0));
	mainMenuButton->setAlignment(AgkHAlign::left, AgkVAlign::top);
	mainMenuButton->setDepth(GAME_HUD_BUTTON_DEPTH);
	mainMenuButton->fixToScreen();
	mainMenuButton->setScreenMode();
	addButton(mainMenuButton, "main-menu", GHpos[id]);
}
void GameHUD::createPauseButton() {
	std::string id = "button-pause";
	AgkImage* pimg = MEDIA_BANK.getImage(PAUSE_IMAGE);
	pauseButton = new UISpriteButton_T1(pimg, pimg, pimg, GHsize[id], glm::vec2(0));
	pauseButton->setAlignment(AgkHAlign::right, AgkVAlign::top);
	pauseButton->setDepth(GAME_HUD_BUTTON_DEPTH);
	pauseButton->fixToScreen();
	pauseButton->setScreenMode();
	addButton(pauseButton, id, GHpos[id]);
}

void GameHUD::createMegaTowerButton() {
	std::string id = "button-mega";
	megaTowerButton = new UISpriteButton_T2(glm::vec3(1, 1, 0.5), glm::vec3(1, 0, 0), glm::vec3(0.2), GHsize[id], glm::vec2(0));
	megaTowerButton->setAlignment(AgkHAlign::right, AgkVAlign::top);
	megaTowerButton->setDepth(GAME_HUD_BUTTON_DEPTH);
	megaTowerButton->fixToScreen();
	megaTowerButton->setScreenMode();
	addButton(megaTowerButton, id, GHpos[id]);
}

void GameHUD::createPauseSign() {
	std::string id = "pause-sign";
	pauseSign = new TextSprite((char*)"Pause", glm::vec3(0.5f), glm::vec2(0), GHsize_d[id], AgkHAlign::center, AgkVAlign::center);
	pauseSign->setBold(true);
	pauseSign->setFont(MEDIA_BANK.getFont(FONT1));
	pauseSign->fixToScreen();
	this->addSprite(pauseSign, id, GHpos[id]);
	hidePauseSign();
}



void GameHUD::createTowerInfoFrame(){
	std::string id = "frame-tower-info";
	towerInfoFrame = new InfoFrameList(GHsize[id], 0, "Tower info");
	addFrame(towerInfoFrame, id, GHpos[id]);
}

void GameHUD::createTowerUpgradeFrame(){
	std::string id = "frame-tower-upgrade";
	towerUpgradeFrame = new InfoFrameList(GHsize[id], 0, "Tower upgrade");
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

void GameHUD::createUpperStatsFrame(){
	std::string id = "frame-upper-stats";
	upperStatsFrame = new InfoFrame(GHsize[id], 0, "upper stats");
	addFrame(upperStatsFrame, id, GHpos[id]);

	TextSprite* ts = new TextSprite("Next\nWave:", glm::vec3(1), glm::vec2(0), 15.0f);
	ts->setDepth(3);
	ts->setBold(true);
	ts->setFont(MEDIA_BANK.getFont(FONT1));
	ts->fixToScreen();
	upperStatsFrame->addSprite(ts, "next-wave", glm::vec2(5, 3), true);

	timerSprite = new NumberTextSprite(0.0, glm::vec3(1.0f), glm::vec2(0.0f), 35.0f);
	timerSprite->setDepth(3);
	timerSprite->setFont(MEDIA_BANK.getFont(FONT1));
	timerSprite->fixToScreen();
	timerSprite->setBold(true);
	upperStatsFrame->addSprite(timerSprite, "timer", glm::vec2(65, -1), true);

	Sprite* moneyIcon = new Sprite(GETIMG(COIN_IMAGE), glm::vec2(0), glm::vec2(25.0f), AgkHAlign::left, AgkVAlign::top);
	moneyIcon->setDepth(3);
	moneyIcon->fixToScreen();
	upperStatsFrame->addSprite(moneyIcon, "coin", glm::vec2(320, 5), true);

	waveSprite = new NumberTextSprite(0, glm::vec3(1.0f), glm::vec2(0.0f), 35.0f);
	waveSprite->setBold(true);
	waveSprite->setFont(MEDIA_BANK.getFont(FONT1));
	waveSprite->setDepth(3);
	waveSprite->fixToScreen();
	upperStatsFrame->addSprite(waveSprite, "wave", glm::vec2(150, 1), true);

	moneySprite = new NumberTextSprite(2564.0, glm::vec3(1.0f), glm::vec2(0.0f), 30.0f);
	moneySprite->setBold(true);
	moneySprite->setValue(2564.0, 0);
	moneySprite->setFont(MEDIA_BANK.getFont(FONT1));
	moneySprite->setDepth(3);
	moneySprite->fixToScreen();
	moneySprite->setAlignment(AgkHAlign::right, AgkVAlign::top);
	upperStatsFrame->addSprite(moneySprite, "money", glm::vec2(315, 1), true);
}
void GameHUD::createSideStatsFrame(){
	std::string id = "frame-side-stats";
	sideStatsFrame = new InfoFrame(GHsize[id], 1, "side stats");
	addFrame(sideStatsFrame, id, GHpos[id]);

	SpriteAnimation::animation_state dfState(0.25f, glm::vec2(GHsize[id].x + 10.0f, 0.f), 0.0f, 1.0f, movement_type::linear, interpolation_type::cubic);
	SpriteAnimation::animation_state outState(0.25f, glm::vec2(0.f, 0.f), 0.0f, 1.0f, movement_type::linear, interpolation_type::cubic);
	sideStatsFrame->setDefaultState(dfState);
	sideStatsFrame->addState("out", outState);
	addSpriteAnimation(sideStatsFrame, id);
}


void GameHUD::incrTimer(float dt) {
	setTimer(getTimer() + dt);
}
void GameHUD::setTimer(float v) {
	timerSprite->setValue(v, 1);
}
float GameHUD::getTimer() {
	return timerSprite->getValue();
}

void GameHUD::update(bool pPressed, bool pReleased, bool pState)
{
	processButtons(pPressed, pReleased, pState);
	processAnimations();	
}

bool GameHUD::goToMainMenuInvoked(){
	return mainMenuButton->getButtonState();
}
bool GameHUD::pauseGameInvoked(){
	return pauseButton->getButtonState();
}
void GameHUD::showPauseSign(){
	pauseSign->show();
}
void GameHUD::hidePauseSign(){
	pauseSign->hide();
}


bool GameHUD::pointerIsOverHUD() {

	return (
		this->towerInfoFrame->isHoovered()		||
		this->towerUpgradeFrame->isHoovered()	||
		(this->upperStatsFrame->isActive() && this->upperStatsFrame->isHoovered())		||
		this->sideStatsFrame->isHoovered()		||
		this->pauseButton->isHoovered()			||
		this->mainMenuButton->isHoovered()		||
		this->megaTowerButton->isHoovered()
		);
}