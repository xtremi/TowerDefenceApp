#pragma once
#include "MediaBank.h"
#include "logger.h"

#include "ViewController/ViewController.h"

#include "GameHUD.h"
#include "CellSelectionMenu.h"
#include "TowerDirectionMenu.h"

#include "AnimationController.h"
#include "MobController.h"
#include "BuildingController.h"
#include "WaveController.h"

#include "OnScreenLogger.h"

#define DEFAULT_GRAVITY 80.0f

#include "PerlinNoise.h"

/***********************************************/
class GamePlay
/***********************************************/
{
public:
	GamePlay();
	
	void init();
	void reset(int lvl);
	void update(bool pPressed, bool pReleased, bool pState);
	void unpause();
	void pause(bool showPauseSign);
	void hide();
	void show();


	/*new*/ void processUserSelection(bool pPressed, bool pReleased, bool pState, bool wasPannedOrZoomed, bool wasTapped, bool pointerIsOverGameHUD);

	/*new*/ void processCellSelection();
	/*new*/ Cell* getSelectedCell(bool pPressed, bool pReleased, bool pState, bool wasTapped);

	bool processCellSelectionMenuInput();
	bool processCellSelectionMenu(bool pPressed, bool pReleased, bool pState, bool wasPannedOrZoomed, bool pointerIsOverGameHUD);
	bool processGetUserTowerDirection(bool pPressed, bool pReleased, bool pState, bool wasPannedOrZoomed, bool pointerOverGameHUD);
	
	GameHUD* getHud() {
		return &hud;
	}

	bool levelIsCompleted();

private:
	GameHUD					hud;
	Map						map;
	MobController			mobControl;
	BuildingController		buildingControl;
	WaveController			waveControl;
	ViewController*			viewControl = NULL;
	AnimationController		animationControl; 

	Cell*				selectedCell = NULL;

	TowerDirectionMenu* towerDirectionMenu = NULL;
	CellSelectionMenu2*	cellSelectionMenu = NULL;
	Sprite*				cellSelectionMarker = NULL;
	
	OnScreenLogger* logger = NULL;

	void initCellSelectionMenu();
	void initCellSelectionMarker();
	void cellIsSelected();
	void cellWithBuildingIsSelected();
	void cellSelectionChanged(Cell* newCell);
	void emptyCellIsSelected();
	void startCellSelectionMenu();
	void startTowerDirectionSelection();
	void cancelCellSelectionMenu();
	void cancelTowerDirectionMenu();
	void cancelCellSelection(bool duckFrames);

	bool getUserTowerDirectionMode = false;

	void showTowerDataInFrame(InfoFrameList* frame, Tower* tower);

	void sendWave(int waveNumber);

	float money = 0.0f;

	void setTestTowers();
	void createTowerRow(int startX, int nTowers, int y, int towerID);

};




