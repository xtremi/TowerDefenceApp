#include "GamePlay.h"
//#include "D:\VS2017_Projects\container_benchmark\container_benchmark\TestContainer.h"

static std::vector<float> xpn;
static std::vector<float> ypn;
/****************************************************/
GamePlay::GamePlay()
/****************************************************/
{
	LOGI("GamePlay::GamePlay()");
	siv::PerlinNoise pn;
	
	for (int i = 0; i < 100; i++) {
		xpn.push_back(5.0f*(float)i);
		ypn.push_back(50.0f*pn.octaveNoise(xpn[i] * 0.005f, 4) + 500.0f);
	}
}

void GamePlay::setTestTowers2() {
	createTowerRow(14, 3, 6, 202);
	createTowerRow(14, 3, 7, 202);
	createTowerRow(14, 3, 8, 202);

	createTowerRow(17, 3, 6, 102);
	createTowerRow(17, 3, 7, 102);
	createTowerRow(17, 3, 8, 102);
}

void GamePlay::setTestTowers1() {
	createTowerRow(15, 7, 4, 202);
	createTowerRow(13, 7, 6, 202);
	createTowerRow(5, 14, 8, 202);
	createTowerRow(7, 14, 10, 202);
	createTowerRow(8, 13, 11, 202);
	createTowerRow(10, 8, 14, 202);
	createTowerRow(10, 8, 16, 202);
	createTowerRow(8, 12, 18, 202);
	createTowerRow(8, 12, 20, 202);
	createTowerRow(8, 12, 21, 202);
	createTowerRow(8, 12, 23, 202);
	createTowerRow(8, 12, 24, 202);
	createTowerRow(8, 12, 26, 202);
	createTowerRow(8, 6, 28, 202);
	createTowerRow(15, 6, 28, 202);
	createTowerRow(7, 12, 30, 202);
	createTowerRow(7, 12, 31, 202);
	createTowerRow(7, 12, 32, 202);

}

void GamePlay::createTowerRow(int startX, int nTowers, int y, int towerID) {
	for (int i = 0; i < nTowers; i++) {
		buildingControl.addNewTower(map.getCell(startX + i, y), &map);
		buildingControl.upgradeTower(map.getCell(startX + i, y), &map, towerID);
	}
}


//static std::string testContainerResult = "";
/****************************************************/
void GamePlay::init()
/****************************************************/
{
	LOGI("GamePlay::init()");	
	hud.init();
	agk::SetRandomSeed(1);

	logger = new OnScreenLogger();
	logger->setPosition(glm::vec2(200.0f, 50.0f));

	//agk::SetPrintColor(255, 0, 0);
	//agk::SetPrintSize(25.f);
	//agk::SetPhysicsGravity(0.0, DEFAULT_GRAVITY);

	buildingControl.loadTowerDescriptors();
	mobControl.loadMobDescriptors();
	waveControl.loadWaveDescriptors();
	animationControl.loadAnimations();
	
	mobControl.setAnimationController(&animationControl);


	towerDirectionMenu = new TowerDirectionMenu();
	initCellSelectionMarker();
	initCellSelectionMenu();


	hide();
	LOGI("GamePlay::init() complete");

	
	/*agk::SetPrintSize(20.0f);
	agk::SetPrintColor(255, 255, 255);
	testContainerResult = TestContainer::doContainerTest();*/
}

/****************************************************/
void GamePlay::reset(int lvl)
/****************************************************/
{
	int xcells		= 28;// VIRTUAL_WIDTH / CELL_SIZE;
	int ycells		= 60;//VIRTUAL_HEIGHT / CELL_SIZE - 0;
	float width		= xcells * CELL_SIZE;
	float height	= ycells * CELL_SIZE;
	float dx = 0.f;// (VIRTUAL_WIDTH - width) / 2.0f;

	int viewMode = 0;
#ifdef WIN_64_BUILD
	viewMode = 1;
#endif 
	viewControl = new ViewController(viewMode, 0.0f, width, 0.0f, height, 0.1f, 5.0f, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

	float zoom_factor = ((float)VIRTUAL_HEIGHT) / height; 

	map.createMap(glm::vec2(0.f), xcells, ycells, xcells / 2, 4);	
	map.setMobEndCellFromCell(20, map.getMobStartCell());
	map.setCellRowsValidState(0, map.getMobEndCell()->mapLocY(), true);

	waveControl.setMobStartCell(map.getMobStartCell());
	sendWave(0);
	hud.setWaveNumber(1);
	//MEDIA_BANK.getSound(BG_MUSIC_01)->play(100, 1);

	this->money = 1000.0f;

	setTestTowers2();
}



/****************************************************/
void GamePlay::sendWave(int waveNumber)
/****************************************************/
{
	waveControl.sendWave(&mobControl, waveNumber, map.getMobStartCell());
}

/****************************************************/
void GamePlay::update(bool pPressed, bool pReleased, bool pState)
/****************************************************/
{	
	//agk::Print(&testContainerResult[0]);
	/*for (int i = 0; i < xpn.size() - 1; i++) {
		agk::DrawLine(xpn[i], ypn[i], xpn[i + 1], ypn[i + 1], 255, 0, 0);
	}*/

	//logger->process();
	hud.update(pPressed, pReleased, pState);
	mobControl.update(&map);
	buildingControl.update();
	waveControl.update(&mobControl);
	
	if (int newWaveNumber = waveControl.hasNewWave()) {
		map.setMobEndCellFromCell(1, map.getMobEndCell());
		map.setCellRowValidState(map.getMobEndCell()->mapLocY(), true);
		hud.setWaveNumber(newWaveNumber + 1);
	}
	
	hud.setTimer(waveControl.timeRemaining());
	money += buildingControl.getProfit();
	hud.setMoney(money);

	bool pointerOverGameHUD = hud.pointerIsOverHUD();
	bool wasTapped			= false;
	bool wasPannedOrZoomed	= viewControl->processZoomPan(&wasTapped);
#ifndef WIN_64_BUILD
	pReleased = wasTapped;
#else
	wasTapped = pReleased;
#endif

	logger->setItemValue("pPressed", pPressed);
	logger->setItemValue("pReleased", pReleased);
	logger->setItemValue("pState", pState);
	logger->setItemValue("wasPndOrZmd", wasPannedOrZoomed);
	logger->setItemValue("wasTapped", wasTapped);
	logger->setItemValue("pntrrOverHUD", pointerOverGameHUD);

	logger->setItemValue("live mobs", mobControl.liveMobs());
	logger->setItemValue("mob queue", mobControl.mobQueueSize());
	logger->setItemValue("cellmobs", map.numberOfCellsWithMobs());

	logger->setItemValue("sprites", agk::GetManagedSpriteCount());
	logger->setItemValue("fps", agk::ScreenFPS(), 0);
	logger->setItemValue("BC", BULLET_COUNT);
	logger->setItemValue("AMobs", mobControl.liveMobs());
	logger->setItemValue("QMobs", mobControl.mobQueueSize());

	processUserSelection(pPressed, pReleased, pState, wasPannedOrZoomed, wasTapped, pointerOverGameHUD);
}

/*
Calls functionality that process user interaction with buttons, cells, menus...
*/
void GamePlay::processUserSelection(bool pPressed, bool pReleased, bool pState, bool wasPannedOrZoomed, bool wasTapped, bool pointerIsOverGameHUD)
{
	if (!selectedCell && !pointerIsOverGameHUD && !wasPannedOrZoomed && wasTapped){
		selectedCell = getSelectedCell(pPressed, pReleased,  pState, wasTapped);
		if(selectedCell){
		    processCellSelection();
		}
	}
	else{

		bool buttonWasClicked = false;

		if (cellSelectionMenu->isActive())
			buttonWasClicked = processCellSelectionMenu(pPressed, pReleased, pState, wasPannedOrZoomed, pointerIsOverGameHUD);
		else if (towerDirectionMenu->isActive())
			buttonWasClicked = processGetUserTowerDirection(pPressed, pReleased, pState, wasPannedOrZoomed, pointerIsOverGameHUD);
		
		logger->setItemValue("btnClick", buttonWasClicked);


		if(!buttonWasClicked && !wasPannedOrZoomed){
			if (pointerIsOverGameHUD && wasTapped){
				cancelCellSelection(true);
			}
			else if(!pointerIsOverGameHUD && wasTapped) {
				Cell* cell = getSelectedCell(pPressed, pReleased, pState, wasTapped);
				if (cell) {
					cellSelectionChanged(cell);
				}
			}
		}

	}
}

Cell* GamePlay::getSelectedCell(bool pPressed, bool pReleased, bool pState, bool wasTapped) {
	glm::vec2 ppos = getPointerPositionWorld();
	Cell* c = map.getCellAtLocation(ppos);

	if (c && wasTapped) {
		//c->processButtonState(pPressed, pReleased, pState);
		//if (c->getButtonState())
			return c;
		}
	return NULL;
}

void GamePlay::processCellSelection()
{
	if (selectedCell->getBuilding())
		cellWithBuildingIsSelected();
	else
		emptyCellIsSelected();
}

bool GamePlay::processCellSelectionMenu(bool pPressed, bool pReleased, bool pState, bool wasPannedOrZoomed, bool pointerIsOverGameHUD)
{
	bool userClickedButton = false;

	if (wasPannedOrZoomed) cellSelectionMenu->update();	
	else if (!pointerIsOverGameHUD) 
	{		
		cellSelectionMenu->processButtons(pPressed, pReleased, pState);
		userClickedButton = processCellSelectionMenuInput();	
	}
	
	return userClickedButton;
}

bool GamePlay::processGetUserTowerDirection(bool pPressed, bool pReleased, bool pState, bool wasPannedOrZoomed, bool pointerOverGameHUD)
{
	towerDirectionMenu->processButtons(pPressed, pReleased, pState);

	bool anyClicked = false;
	if (towerDirectionMenu->getButtonState("dirbtn1")) {
		selectedCell->getBuilding()->face(direction::east);
		anyClicked = true;
	}
	else if (towerDirectionMenu->getButtonState("dirbtn2")) {
		selectedCell->getBuilding()->face(direction::south);
		anyClicked = true;
	}
	else if (towerDirectionMenu->getButtonState("dirbtn3")) {
		selectedCell->getBuilding()->face(direction::west);
		anyClicked = true;
	}
	else if (towerDirectionMenu->getButtonState("dirbtn4")) {
		selectedCell->getBuilding()->face(direction::north);
		anyClicked = true;
	}

	if (anyClicked) {
		towerDirectionMenu->hide();
		towerDirectionMenu->disable();
		buildingControl.assignCellsInTowerRange(((Tower*)selectedCell->getBuilding()), &map);
	}

	return anyClicked;
}

bool GamePlay::processCellSelectionMenuInput() 
{
	int pressedButton = -1;
	int confirmedButton = -1;
	float cost = 0.0f;

	if (cellSelectionMenu->buttonWasPressed(&pressedButton)) {
		return true;
	}
	if (cellSelectionMenu->buttonWasConfirmed(&confirmedButton)) {

		if (!selectedCell->getBuilding()) {
			if (confirmedButton == 1) {
				Tower* t = buildingControl.addNewTower(selectedCell, &map);				
				startTowerDirectionSelection();
				cost = t->getDescriptor()->price;
			}
		}
		else {
			if(confirmedButton == 1000){
				startTowerDirectionSelection();
			}
			else if (confirmedButton == 1001) {
				buildingControl.upgradeTowerToMega(selectedCell, &map);
				megaCellisSelected();
			}
			else{
				Tower* t = buildingControl.upgradeTower(selectedCell, &map, confirmedButton);
				cost = t->getDescriptor()->price;
			}
		}		


		cellSelectionMenu->cancel();
		showTowerDataInFrame(hud.getTowerInfoFrame(), (Tower*)selectedCell->getBuilding());
		hud.towerUpgradeFrameAppear();

		money -= cost;
		return true;
	}
	return false;
	
}

/*
Brings up the cell selection marker
*/
void GamePlay::cellIsSelected()
{
	cellSelectionMarker->show();
	cellSelectionMarker->setScale(1.0f);
	cellSelectionMarker->setPosition(selectedCell->getPos() + CELL_SIZE / 2.0f);
	
	if (selectedCell->getBuilding())
	{
		if(selectedCell->getBuilding()->buildingSizeUpgrade() == building_size_upgrade::mega)
		{
			megaCellisSelected();
		}
	}
}

void GamePlay::megaCellisSelected()
{
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
	else {
		bool previousSelectedCellHasBuilding	= selectedCell->getBuilding() ? true : false;
		bool selectedCellHasBuilding			= newCell->getBuilding() ? true : false;

		bool duckFrames = previousSelectedCellHasBuilding && !selectedCellHasBuilding;

		cancelCellSelection(duckFrames);
		selectedCell = newCell;
		processCellSelection();
	}
}


void GamePlay::startCellSelectionMenu() {
	
	std::vector<int> buttonIDs;
	bool mega_upgrade_enabled = false;
	bool cell_is_empty = !selectedCell->getBuilding();
	bool is_megafied = cell_is_empty ? false : selectedCell->getBuilding()->buildingSizeUpgrade() == building_size_upgrade::mega;

	if(!cell_is_empty && selectedCell->getBuilding()->isTower()){
		buttonIDs = ((Tower*)selectedCell->getBuilding())->getDescriptor()->upgradable;
		buttonIDs.push_back(1000);	//direction button
		
		//Only add upgrade button is not already upgraded:
		if(!is_megafied){
			buttonIDs.push_back(1001);	//upgrade size button
			mega_upgrade_enabled = map.buildingCanUpgradeToMega(selectedCell->mapLocX(), selectedCell->mapLocY());
		}
	}
	else if (cell_is_empty) {
		buttonIDs.push_back(1);
	}

	cellSelectionMenu->display(selectedCell->getPos() + CELL_SIZE / 2.0f, buttonIDs);

	if(is_megafied || (!mega_upgrade_enabled && !cell_is_empty))
		cellSelectionMenu->setButtonInactive(1001);
}
void GamePlay::startTowerDirectionSelection() {
	towerDirectionMenu->show();
	towerDirectionMenu->enable();

	glm::vec2 cpos = selectedCell->posCenter();
	towerDirectionMenu->setPosition(cpos);
	towerDirectionMenu->setScale(glm::vec2(selectedCell->getBuilding()->getRange(), 1.0f));

	getUserTowerDirectionMode = true;
}


void GamePlay::emptyCellIsSelected()
{
	cellIsSelected();
	if(!selectedCell->isRoad() && selectedCell->valid())
		startCellSelectionMenu();
}



void GamePlay::cellWithBuildingIsSelected()
{
	if (cellSelectionMenu->isActive()) {
		cellSelectionMenu->cancel();
	}

	cellIsSelected();
	startCellSelectionMenu();
	hud.towerUpgradeFrameAppear();

	if (selectedCell->getBuilding()->isTower())
		showTowerDataInFrame(hud.getTowerInfoFrame(), (Tower*)selectedCell->getBuilding());
}


void GamePlay::cancelCellSelection(bool duckFrames)
{
	cellSelectionMarker->hide();
	if (cellSelectionMenu->isActive()) cancelCellSelectionMenu();
	else if (towerDirectionMenu->isActive()){
		cancelTowerDirectionMenu();		
	}

	if (duckFrames) hud.towerUpgradeFrameDuck();

	selectedCell = NULL;
}

void GamePlay::cancelCellSelectionMenu()
{
	cellSelectionMenu->cancel();
}

void GamePlay::cancelTowerDirectionMenu()
{
	towerDirectionMenu->hide();
	towerDirectionMenu->disable();
}

/****************************************************/
void GamePlay::hide()
/****************************************************/
{
	hud.hide();
	map.hide();
	logger->hide();
	/*cellSelectionMenu->hide();*/
	cellSelectionMenu->cancel();
	towerDirectionMenu->hide();
	cellSelectionMarker->hide();
	agk::SetVirtualJoystickVisible(JOYSTICK_ID, false);
}
/****************************************************/
void GamePlay::show()
/****************************************************/
{
	agk::SetVirtualResolution(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
	hud.show();
	map.show();
	logger->show();
	agk::SetVirtualJoystickVisible(JOYSTICK_ID, true);
}
/****************************************************/
void GamePlay::pause(bool showPauseSign)
/****************************************************/
{
	if(showPauseSign) hud.showPauseSign();
}
/****************************************************/
void GamePlay::unpause()
/****************************************************/
{
	hud.hidePauseSign();
}
/****************************************************/
void GamePlay::initCellSelectionMarker()
/****************************************************/
{
	cellSelectionMarker = new Sprite(GETIMG(CELL_SELECT_MARK), glm::vec2(0), glm::vec2(CELL_SIZE));
	cellSelectionMarker->setDepth(4);
	cellSelectionMarker->hide();
}


/****************************************************/
void GamePlay::initCellSelectionMenu()
/****************************************************/
{
	std::vector<int> buttonIDs;
	std::vector<AgkImage*> btnImgsbtn, btnImgsPressed, btnImgsInactive;	
	for (TowerDescriptor* td = buildingControl.firstTowerDescriptor(); td; td = buildingControl.nextTowerDescriptor()) {
		buttonIDs.push_back(td->ID);
		btnImgsbtn.push_back(td->images.buttons[0]);
		btnImgsPressed.push_back(td->images.buttons[1]);
		btnImgsInactive.push_back(td->images.buttons[2]);
	}

	buttonIDs.push_back(1000);
	btnImgsbtn.push_back(GETIMG(SET_BUILDING_DIR_BUTTON_IMAGE));
	btnImgsPressed.push_back(GETIMG(SET_BUILDING_DIR_BUTTON_P_IMAGE));
	btnImgsInactive.push_back(GETIMG(SET_BUILDING_DIR_BUTTON_I_IMAGE));
	buttonIDs.push_back(1001);
	btnImgsbtn.push_back(GETIMG(UPGRADE_TOWER_BUTTON_IMAGE));
	btnImgsPressed.push_back(GETIMG(UPGRADE_TOWER_BUTTON_P_IMAGE));
	btnImgsInactive.push_back(GETIMG(UPGRADE_TOWER_BUTTON_I_IMAGE));

	cellSelectionMenu = new CellSelectionMenu2(buttonIDs, btnImgsbtn, btnImgsPressed, btnImgsInactive);
}

bool GamePlay::levelIsCompleted() {
	return false;
}

void GamePlay::showTowerDataInFrame(InfoFrameList* frame, Tower* tower)
{
	frame->setListItemValue("Name", tower->getDescriptor()->name);
	frame->setListItemValue("ID", tower->getDescriptor()->ID);

	frame->setListItemValue("Range", tower->getDescriptor()->range);
	frame->setListItemValue("Damage", tower->getDescriptor()->bulletStats.damage);
	frame->setListItemValue("Speed", tower->getDescriptor()->bulletStats.speed);
	frame->setListItemValue("Reload t.", tower->getDescriptor()->reload_time);
	frame->setListItemValue("Rot. speed.", tower->getDescriptor()->rotate_speed);
	frame->setListItemValue("Xpos :", tower->getLocX());
	frame->setListItemValue("Ypos :", tower->getLocY());
}


/*	glm::vec2 cpos = cellSelectionMenu->getCurrentCell()->posCenter();
	glm::vec2 ppos = screenToWorld(getPointerPosition());

	float ang = agk::ATan2(ppos.y - cpos.y, ppos.x - cpos.x);
	float range = 4.0f*25.0f;

	std::string cposstr = "cpos: " + TOSTR(cpos.x, 2) + ", " + TOSTR(cpos.y, 2);
	std::string pposstr = "ppos: " + TOSTR(ppos.x, 2) + ", " + TOSTR(ppos.y, 2);
	std::string angstr  = "ang : " + TOSTR(ang, 1);

	agk::Print(&cposstr[0]);
	agk::Print(&pposstr[0]);
	agk::Print(&angstr[0]);

	towerDirectionIndicator->setScale(range * agk::GetViewZoom());
	towerDirectionIndicator->setRotation(ang);

	towerRangeIndicator->setScale(range * agk::GetViewZoom());

*/