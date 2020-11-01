#include "GamePlay.h"


/*
	Empty contructor
*/
GamePlay::GamePlay(){}

/*
	Initalizes the GamePlay class
*/
void GamePlay::init()
{
	hud.init();	
	int	viewMode = 0;

#ifdef WIN_64_BUILD
	viewMode = 1;
#endif 
	float	width = xcells * CELL_SIZE;
	float	height = ycells * CELL_SIZE;
	viewControl = new ViewController(viewMode, 0.0f, width, 0.0f, height, ZOOM_MIN, ZOOM_MAX, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

	debugLogger = new OnScreenLogger();
	debugLogger->setPosition(DEBUG_LOGGER_POS);

	buildingControl.loadTowerDescriptors();
	mobControl.loadMobDescriptors();
	waveControl.loadWaveDescriptors();
	//animationControl.loadAnimations();	
	//mobControl.setAnimationController(&animationControl);

	towerDirectionMenu = new TowerDirectionMenu();
	initCellSelectionMarker();
	initRangeIndicator();
	initCellSelectionMenu();	
	initBackgroundMusic();	

	hide();
}

/*
	Reset the game (not really tested expect for at game init; don't think its removing anything before reseting)
*/
void GamePlay::reset(int lvl)
{
	startCellX = xcells / 2;
	startCellY = 4;

	viewControl->setCurrentViewPoint(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT, ZOOM_MIN);
	map.createMap(glm::vec2(0.0f), xcells, ycells, startCellX, startCellY);
	viewControl->setCurrentViewPoint(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT, 2.0);
	
	map.setMobEndCellFromCell(mobStartCell, map.getMobStartCell());
	map.setCellRowsValidState(0, map.getMobEndCell()->mapLocY(), true);

	waveControl.setMobStartCell(map.getMobStartCell());
		
	//toogleDebugLoggerFrame();

	money = START_MONEY;
	lives = START_LIVES;
	hud.setLife(lives);
	hud.setWaveNumber(1);
	hud.updateWaveMobInfo(waveControl.getWaveDescriptor(0), &mobControl, false);
}



/*
	Sent the mob wave corresponding to waveNumber
*/
void GamePlay::sendFirstWave()
{
	waveControl.sendWave(&mobControl, 0, map.getMobStartCell());
	hud.updateWaveMobInfo(waveControl.getNextWaveDescriptor(), &mobControl);
}

/*
	Set the state of single touch interaction / mouse press
*/
void GamePlay::setPointerState(bool pPressed, bool pReleased, bool pState)
{
	wis.pPressed	= pPressed;
	wis.pReleased	= pReleased;
	wis.pState		= pState;
}

/*
	Main function for updating the whole game play
*/
void GamePlay::update()
{	
	agk::Print(agk::GetRendererName());
	hud.update(wis.pPressed, wis.pReleased, wis.pState);
	mobControl.update(&map);
	buildingControl.update(&map);
	
	processWaves();

	wis.pointerOverGameHUD  = hud.pointerIsOverHUD();
	wis.wasTapped			= false;
	wis.wasPannedOrZoomed	= viewControl->processZoomPan(&wis.wasTapped);

#ifndef WIN_64_BUILD
	wis.pReleased = wis.wasTapped;
#else
	wis.wasTapped = wis.pReleased;
#endif

	processMoney();
	processDebugLoggerData();
	processUserSelection();

	if (int nLivesLost = mobControl.getNumberOfMobsReadEnd()) {
		lives -= nLivesLost;
		if (lives < 0) lives = 100;
		hud.setLife(lives);
	}

}

/*
	Called every game play update and checks if there is a new wave, and process it:
*/
void GamePlay::processWaves() {

	if (waveControl.firstWaveHasStarted()) {
		waveControl.update(&mobControl);
		hud.setWaveTimer(waveControl.timeRemaining());
	}

	if (hud.nextWaveInvoked()){
		if (waveControl.firstWaveHasStarted())
			waveControl.goToWaveEnd();
		else{
			sendFirstWave();
			hud.startWaveTimer();
		}
	}
	
	if (int newWaveNumber = waveControl.hasNewWave()) {
		map.setMobEndCellFromCell(1, map.getMobEndCell());
		map.setCellRowValidState(map.getMobEndCell()->mapLocY(), true);
		hud.setWaveNumber(newWaveNumber + 1);
		hud.updateWaveMobInfo(waveControl.getNextWaveDescriptor(), &mobControl);
	}
}

/*
	Called every game play update and updates the current money (including in the HUD)
*/
void GamePlay::processMoney() 
{
	money += buildingControl.getProfit();
	hud.setMoney(money);
}

/*
	Called every gameplay update and updates the values in the debug logger:
*/
void GamePlay::processDebugLoggerData() {
	if (debugLoggerIsOn){
		debugLogger->setItemValue("pPressed", wis.pPressed);
		debugLogger->setItemValue("pReleased", wis.pReleased);
		debugLogger->setItemValue("pState", wis.pState);
		debugLogger->setItemValue("wasPndOrZmd", wis.wasPannedOrZoomed);
		debugLogger->setItemValue("wasTapped", wis.wasTapped);
		debugLogger->setItemValue("pntrrOverHUD", wis.pointerOverGameHUD);

		debugLogger->setItemValue("live mobs", mobControl.liveMobs());
		debugLogger->setItemValue("mob queue", mobControl.mobQueueSize());
		debugLogger->setItemValue("cellmobs", map.numberOfCellsWithMobs());

		debugLogger->setItemValue("sprites", agk::GetManagedSpriteCount());
		debugLogger->setItemValue("fps", agk::ScreenFPS(), 0);
		debugLogger->setItemValue("BC", BULLET_COUNT);
		debugLogger->setItemValue("AMobs", mobControl.liveMobs());
		debugLogger->setItemValue("QMobs", mobControl.mobQueueSize());

		int nEff, nEffM;
		buildingControl.getNumberOfMobEffects(nEff, nEffM);
		debugLogger->setItemValue("Neff", nEff);
		debugLogger->setItemValue("NeffM", nEffM);

		debugLogger->process();
	}
}

/*
Calls functionality that process user interaction with buttons, cells, menus...
*/
void GamePlay::processUserSelection()
{
	//If a cell is currently not selected, and a cell is tapped:
	if (!selectedCell && !wis.pointerOverGameHUD && !wis.wasPannedOrZoomed && wis.wasTapped){
		selectedCell = getCellAtPointerSelection();
		if(selectedCell)
		    processCellSelection();		
	}
	else{

		bool buttonWasClicked = false;

		if (cellSelectionMenu->isActive())
			buttonWasClicked = processCellSelectionMenu();
		else if (towerDirectionMenu->isActive())
			buttonWasClicked = processGetUserTowerDirection();
		
		if(!buttonWasClicked && !wis.wasPannedOrZoomed){
			//If a cell is currently selected and hud was tapped:
			if (wis.pointerOverGameHUD && wis.wasTapped)
				cancelCellSelection(true);
			//If a cell is currently selected and another cell is tapped:
			else if(!wis.pointerOverGameHUD && wis.wasTapped) {				
				if (Cell* cell = getCellAtPointerSelection())
					cellSelectionChanged(cell);				
			}
		}

		debugLogger->setItemValue("btnClick", buttonWasClicked);

	}
}

/*
	Returns the cell which is at the pointer location
*/
Cell* GamePlay::getCellAtPointerSelection() {
	glm::vec2 ppos	= getPointerPositionWorld();	
	return map.getCellAtLocation(ppos);
}

/*
	Called when a new cell selection was done
*/
void GamePlay::processCellSelection()
{
	if (selectedCell->getBuilding())
		cellWithBuildingIsSelected();
	else
		emptyCellIsSelected();
}

/*
	Called every game update if the cell selection menu is active:
*/
bool GamePlay::processCellSelectionMenu()
{
	bool userClickedButton = false;

	if (wis.wasPannedOrZoomed) 
		cellSelectionMenu->update();	
	else if (!wis.pointerOverGameHUD)
	{		
		cellSelectionMenu->processButtons(wis.pPressed, wis.pReleased, wis.pState);
		userClickedButton = processCellSelectionMenuInput();	
	}
	
	return userClickedButton;
}

/*
	Called every game play update if the tower direction buttons are active:
*/
bool GamePlay::processGetUserTowerDirection()
{
	towerDirectionMenu->processButtons(wis.pPressed, wis.pReleased, wis.pState);
	
	td::direction clickedDirection;
	if(towerDirectionMenu->directionButtonWasClicked(clickedDirection)){	
		selectedCell->getBuilding()->face(clickedDirection);
		
		towerDirectionMenu->hide();
		towerDirectionMenu->disable();
		buildingControl.assignCellsInTowerRange(((Tower*)selectedCell->getBuilding()), &map);
		return true;
	}

	return false;
}




void GamePlay::processPressedCellSelectionMenuButtonPressed(int pressedButton) {
	std::string infoText = "";

	if (pressedButton == CELL_SELECT_BUTTON_INITIAL_TOWER_ID)
		infoText = "Build standard tower\n\nCost :   10";
	else if (pressedButton == CELL_SELECT_BUTTON_AIM_ID)
		infoText = "Choose tower aiming\ndirection";
	else if (pressedButton == CELL_SELECT_BUTTON_MEGA_ID)
		infoText = "Upgrade to MEGA tower";
	else if (pressedButton == CELL_SELECT_BUTTON_SELL_ID) {
		float value = selectedCell->getBuilding()->value();
		infoText = "Sell Tower\nValue :   " + std::to_string((int)value);
	}
	else {
		float price = buildingControl.getTowerDescriptor(pressedButton)->price;
		infoText = "Sell Tower\nValue :   " + std::to_string((int)price);
	}

	cellSelectionMenu->setInfoSpriteText(infoText);
}

void GamePlay::processConfirmedCellSelectionMenuButtonPressed(int confirmedButton) {
	float cost = 0.0f;
	
	if (!selectedCell->getBuilding()) {
		if (confirmedButton == CELL_SELECT_BUTTON_INITIAL_TOWER_ID) {
			Tower* t = buildingControl.addNewTower(selectedCell, &map);
			cost = t->price();
		}
	}
	else {
		if (confirmedButton == CELL_SELECT_BUTTON_AIM_ID) {
			showTowerDirectionSelections();
		}
		else if (confirmedButton == CELL_SELECT_BUTTON_MEGA_ID) {
			buildingControl.upgradeTowerToMega(selectedCell, &map);
			scaleCellSelectionMarkerToMega();
		}
		else if (confirmedButton == CELL_SELECT_BUTTON_SELL_ID) {
			cost = -buildingControl.sellBuildingAtCell(selectedCell);
		}
		else {
			Tower* t = buildingControl.upgradeTower(selectedCell, &map, confirmedButton);
			cost = t->price();
		}
	}

	cellSelectionMenu->cancel();
	if (selectedCell->getBuilding()) {
		updateTowerFrameData(hud.getTowerInfoFrame(), (Tower*)selectedCell->getBuilding());
		showRangeIndicator(selectedCell->getBuilding()->rangeDistance());
		//hud.towerUpgradeFrameAppear();
	}

	money -= cost;
}

/*
	Called every game update if the cell selection menu is active, adter cell selection menu buttons have been processed
		Checks if a button was pressed or confirmed and processes it
*/
bool GamePlay::processCellSelectionMenuInput() 
{
	int buttonID	= -1;

	if (cellSelectionMenu->buttonWasPressed(&buttonID)) {
		processPressedCellSelectionMenuButtonPressed(buttonID);
		return true;
	}
	else if (cellSelectionMenu->buttonWasConfirmed(&buttonID)) {
		processConfirmedCellSelectionMenuButtonPressed(buttonID);
		return true;
	}
	return false;
}

/*
	Called when any cell is called and displays the cell selection marker
*/
void GamePlay::showCellSelectionMarker()
{
	cellSelectionMarker->show();
	cellSelectionMarker->setScale(1.0f);
	cellSelectionMarker->setPosition(selectedCell->getPos() + CELL_SIZE / 2.0f);	

	if (Building* b = selectedCell->getBuilding())
	{
		if(b->buildingSizeUpgrade() == building_size_upgrade::mega)	
			scaleCellSelectionMarkerToMega();
	}
}


/*
	Called when a cell with building is selected and displays its range
*/
void GamePlay::showRangeIndicator(float radius) 
{
	rangeIndicator->show();
	rangeIndicator->setSize(glm::vec2(2.0f*radius, -1));
	rangeIndicator->setPosition(selectedCell->getPos() + CELL_SIZE / 2.0f);
}


/*
	Scales the cell selection marker to MEGA size:
*/
void GamePlay::scaleCellSelectionMarkerToMega() {
	cellSelectionMarker->setPosition(selectedCell->getBuilding()->getPosition());
	cellSelectionMarker->setScale(3.0f);
}

/*
	Called when a cell is selected while a different cell was already selected.
*/
void GamePlay::cellSelectionChanged(Cell* newCell) {
	
	bool isSameCell = (newCell == selectedCell);
		
	if(isSameCell){
		cancelCellSelection(true);	
	}
	else if(cellSelectionMenu->isActive() && selectedCell->getBuilding()) {
		cancelCellSelectionMenu();
	}
	else {
		bool previousSelectedCellHasBuilding	= selectedCell->getBuilding() ? true : false;
		bool selectedCellHasBuilding			= newCell->getBuilding() ? true : false;

		bool duckFrames = previousSelectedCellHasBuilding && !selectedCellHasBuilding;

		cancelCellSelection(duckFrames);
		selectedCell = newCell;
		processCellSelection();
	}
}


//TODO: move some of this to CellSelectionMenu class:
/*
	Called when a valid cell is selected
	Sets up the correct buttons according to the selected cell, to the cell selection menu
*/
void GamePlay::showCellSelectionMenu() {
	
	std::vector<int>	buttonIDs;
	bool				mega_upgrade_enabled = false;
	
	Building*			currentBuilding = selectedCell->getBuilding();
	bool				cell_is_empty   = !(bool)currentBuilding;
	
	bool is_megafied = cell_is_empty ? false : currentBuilding->buildingSizeUpgrade() == building_size_upgrade::mega;

	if(!cell_is_empty && currentBuilding->isTower()){
		buttonIDs = ((Tower*)currentBuilding)->getDescriptor()->upgradable;
		buttonIDs.push_back(CELL_SELECT_BUTTON_AIM_ID);
		buttonIDs.push_back(CELL_SELECT_BUTTON_SELL_ID);
		
		//Only add upgrade button is not already upgraded:
		if(!is_megafied){
			buttonIDs.push_back(CELL_SELECT_BUTTON_MEGA_ID);
			mega_upgrade_enabled = map.buildingCanUpgradeToMega(selectedCell->mapLocX(), selectedCell->mapLocY());
		}
	}
	else if (cell_is_empty) {
		buttonIDs.push_back(CELL_SELECT_BUTTON_INITIAL_TOWER_ID);
	}

	cellSelectionMenu->display(selectedCell->getPos() + CELL_SIZE / 2.0f, buttonIDs);

	if(is_megafied || (!mega_upgrade_enabled && !cell_is_empty))
		cellSelectionMenu->setButtonInactive(CELL_SELECT_BUTTON_MEGA_ID);
}

/*
	Called when the tower aim button from the cell selection menu is clicked
	Brings up the direction buttons
*/
void GamePlay::showTowerDirectionSelections() {
	towerDirectionMenu->show();
	towerDirectionMenu->enable();

	glm::vec2 cpos = selectedCell->posCenter();
	towerDirectionMenu->setPosition(cpos);
	towerDirectionMenu->setScale(glm::vec2(selectedCell->getBuilding()->rangeCells(), 1.0f));
}

/*
	Called when an empty cell is selected
	Only brings up the cell selection menu if the cell is valid
*/
void GamePlay::emptyCellIsSelected()
{
	showCellSelectionMarker();
	if(!selectedCell->isRoad() && selectedCell->valid())
		showCellSelectionMenu();
}


/*
	Called when a cell with building is selected
	Brings up the cell selection menu and "tower upgrade frame" and updates "tower frame data"
*/
void GamePlay::cellWithBuildingIsSelected()
{
	if (cellSelectionMenu->isActive()) {
		cellSelectionMenu->cancel();
	}
	
	showCellSelectionMarker();
	showCellSelectionMenu();
	
	//hud.towerUpgradeFrameAppear();

	float buildingRangeRadius = selectedCell->getBuilding()->rangeDistance();
	showRangeIndicator(buildingRangeRadius);

	if (selectedCell->getBuilding()->isTower())
		updateTowerFrameData(hud.getTowerInfoFrame(), (Tower*)selectedCell->getBuilding());
}

/*
	Cancels the current cell selection
	hides the cell slection marker, and cell selection menu if active, and tower direciton menu if active
	Ducks the "tower upgrade frame" if duckFrames is true
*/
void GamePlay::cancelCellSelection(bool duckFrames)
{
	cellSelectionMarker->hide();
	rangeIndicator->hide();
	if (cellSelectionMenu->isActive()) cancelCellSelectionMenu();
	else if (towerDirectionMenu->isActive()){
		cancelTowerDirectionMenu();		
	}

	//if (duckFrames) hud.towerUpgradeFrameDuck();

	selectedCell = NULL;
}

/*
	Disables and hide the cell selection menu
*/
void GamePlay::cancelCellSelectionMenu()
{
	cellSelectionMenu->cancel();
}

/*
	Disables and hide the tower direction menu
*/
void GamePlay::cancelTowerDirectionMenu()
{
	towerDirectionMenu->hide();
	towerDirectionMenu->disable();
}

/*
	Public function that hides everything in the game
		Where is the towers and mobs ?
*/
void GamePlay::hide()
{
	hud.hide();
	map.hide();
	debugLogger->hide();
	cellSelectionMenu->cancel();
	towerDirectionMenu->hide();
	cellSelectionMarker->hide();
	rangeIndicator->hide();
	agk::SetVirtualJoystickVisible(JOYSTICK_ID, false);
}

/*
	Shows the game content...
		Where is the towers and mobs ?
*/
void GamePlay::show()
{	
	hud.show();
	map.show();
	if(debugLoggerIsOn)
		debugLogger->show();	
	agk::SetVirtualJoystickVisible(JOYSTICK_ID, true);
}

/*
	Brings up the pause game sign (only if showPauseSign is true)
	TODO: something is not right here. Does nothing if false, where is the game pause
*/
void GamePlay::pause(bool showPauseSign)
{
	if(showPauseSign) hud.showPauseSign();
}

/*
	Hides the pause sign
*/
void GamePlay::unpause()
{
	hud.hidePauseSign();
}

/*
	Initializes the cell selection marker sprite
*/
void GamePlay::initCellSelectionMarker()
{
	cellSelectionMarker = new Sprite(GETIMG(ASSET::IMG_CELL_SELECT), glm::vec2(0), glm::vec2(CELL_SIZE));
	cellSelectionMarker->setDepth(CELL_SELECTION_MARKER_DEPTH);
	cellSelectionMarker->hide();
}


/*
	Initializes the building range indicator
*/
void GamePlay::initRangeIndicator() {
	rangeIndicator = new Sprite(GETIMG(ASSET::IMG_RANGE_INDICATOR), glm::vec2(0), glm::vec2(2.0*CELL_SIZE));
	rangeIndicator->setDepth(RANGE_INDICATOR_DEPTH);
	rangeIndicator->hide();
}

/*
	Initializes the cell seleciton menu
	Creates button for every possible towers
*/
void GamePlay::initCellSelectionMenu()
{
	std::vector<int>			buttonIDs;
	std::vector<AgkImage*>		btnIconImgs;
	std::vector<std::string>	btnInfoTexts;

	for (TowerDescriptor* td = buildingControl.firstTowerDescriptor(); td; td = buildingControl.nextTowerDescriptor()) {
		buttonIDs.push_back(td->ID);
		btnIconImgs.push_back(td->images.icons[0]);
		//btnInfoTexts.push_back("temp text\n\nCost : 10000");
	}

	buttonIDs.push_back(CELL_SELECT_BUTTON_AIM_ID);
	buttonIDs.push_back(CELL_SELECT_BUTTON_MEGA_ID);
	buttonIDs.push_back(CELL_SELECT_BUTTON_SELL_ID);
	btnIconImgs.push_back(GETIMG(ASSET::IMG_CELL_MENU_AIM_ICON));		
	btnIconImgs.push_back(GETIMG(ASSET::IMG_CELL_MENU_MEGA_ICON));			
	btnIconImgs.push_back(GETIMG(ASSET::IMG_CELL_MENU_SELL_ICON));
	

	cellSelectionMenu = new CellSelectionMenu2(buttonIDs, btnIconImgs);
}

/*
	This is not used
*/
bool GamePlay::levelIsCompleted() {
	return false;
}

/*
	Updates the data in the tower frame:
*/
void GamePlay::updateTowerFrameData(InfoFrameList* frame, Tower* tower)
{
	frame->setListItemValue("Name", tower->getDescriptor()->name);
	frame->setListItemValue("ID", tower->getDescriptor()->ID);

	frame->setListItemValue("Range", tower->getDescriptor()->range);
	frame->setListItemValue("Damage", tower->getDescriptor()->bulletStats.data.damage(), 2);
	frame->setListItemValue("Speed", tower->getDescriptor()->bulletStats.data.speed(), 2);
	frame->setListItemValue("Reload t.", tower->getDescriptor()->reload_time);
	frame->setListItemValue("Rot. speed.", tower->getDescriptor()->rotate_speed);
	frame->setListItemValue("Xpos :", tower->getLocX());
	frame->setListItemValue("Ypos :", tower->getLocY());
}


void GamePlay::initBackgroundMusic() {
	bgMusics.push_back(MEDIA_BANK.getMusic(ASSET::MUSIC_BG_01));
	bgMusics.push_back(MEDIA_BANK.getMusic(ASSET::MUSIC_BG_02));
	playBackgroundMusic();
	
	hud.setMusicButtonState(false);	
	pauseBackgroundMusic();
}
void GamePlay::playBackgroundMusic() {
	agk::PlayMusic(bgMusics[0]->getId(), 1, bgMusics[0]->getId(), bgMusics[1]->getId());
}
void GamePlay::pauseBackgroundMusic() {
	agk::PauseMusic();
}
void GamePlay::unpauseBackgroundMusic() {
	agk::ResumeMusic();
}
void GamePlay::toogleBackgroundMusic() {
	if (!hud.musicButtonState()) 
		pauseBackgroundMusic();
	else 
		unpauseBackgroundMusic();
}
void GamePlay::toogleDebugLoggerFrame() {
	if (debugLogger->isVisible()){
		debugLogger->hide();
		buildingControl.hideBuildingDebugInfo();
		debugLoggerIsOn = false;
	}
	else{
		debugLogger->show();
		buildingControl.showBuildingDebugInfo();
		debugLoggerIsOn = true;
	}
}
