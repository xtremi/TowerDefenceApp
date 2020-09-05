#pragma once
#include "MediaBank.h"
#include "logger.h"

#include "ViewController/ViewController.h"

#include "GameHUD.h"
#include "CellSelectionMenu.h"
#include "TowerDirectionMenu.h"
//#include "AnimationController.h"
#include "MobController.h"
#include "BuildingController.h"
#include "WaveController.h"

#include "OnScreenLogger.h"
#include "PerlinNoise.h"

#define DEFAULT_GRAVITY 80.0f
#define DEBUG_LOGGER_POS glm::vec2(5.0f, 250.0f)
#define ZOOM_MIN 0.1f
#define ZOOM_MAX 5.0f
#define START_MONEY 1000.0f
#define START_LIVES 100

#define RANGE_INDICATOR_DEPTH 11
#define CELL_SELECTION_MARKER_DEPTH 10

struct window_interaction_state
{
	bool pointerOverGameHUD;
	bool wasTapped;
	bool wasPannedOrZoomed;
	bool pPressed;
	bool pReleased;
	bool pState;
};


/***********************************************/
class GamePlay
/***********************************************/
{
public:
	GamePlay();
	
	void init();
	void reset(int lvl);
	void update();
	void hide();
	void show();
	void unpause();
	void pause(bool showPauseSign);
	void pauseBackgroundMusic();
	void unpauseBackgroundMusic();
	void toogleBackgroundMusic();
	void toogleDebugLoggerFrame();
	bool levelIsCompleted();
	
	GameHUD* getHud() {	return &hud; }

	void setPointerState(bool pPressed, bool pReleased, bool pState);



private:
	window_interaction_state wis;
	
	GameHUD					hud;
	Map						map;
	
	MobController			mobControl;	
	BuildingController		buildingControl;
	WaveController			waveControl;	
	//AnimationController		animationControl; 
	ViewController*			viewControl			= NULL;
	
	TowerDirectionMenu*		towerDirectionMenu	= NULL;
	CellSelectionMenu2*		cellSelectionMenu	= NULL;
	Sprite*					cellSelectionMarker = NULL;
	Sprite*					rangeIndicator      = NULL;
	
	Cell*					selectedCell		= NULL;
	OnScreenLogger*			debugLogger			= NULL;

	std::vector<AgkMusic*>	bgMusics;

	float					money = 0.0f;
	int						lives = 0;

private:
	Cell* getCellAtPointerSelection();
	
	void initCellSelectionMenu();
	void initCellSelectionMarker();
	void initRangeIndicator();
	void initBackgroundMusic();

	void processUserSelection();
	void processCellSelection();
	bool processCellSelectionMenuInput();
	bool processCellSelectionMenu();
	void processPressedCellSelectionMenuButtonPressed(int pressedButton);
	void processConfirmedCellSelectionMenuButtonPressed(int confirmedButton);
	bool processGetUserTowerDirection();
	void processWaves();
	void processMoney();
	void processDebugLoggerData();

	void cancelCellSelectionMenu();
	void cancelTowerDirectionMenu();
	void cancelCellSelection(bool duckFrames);
	
	void showCellSelectionMenu();
	void showTowerDirectionSelections();
	void showCellSelectionMarker();
	void showRangeIndicator(float radius);

	void scaleCellSelectionMarkerToMega();
	void cellWithBuildingIsSelected();	
	void emptyCellIsSelected();
	void cellSelectionChanged(Cell* newCell);
	

	void updateTowerFrameData(InfoFrameList* frame, Tower* tower);

	void sendFirstWave();
	void playBackgroundMusic();

private: /*testing*/
	void setTestTowers1();
	void setTestTowers2();
	void createTowerRow(int startX, int nTowers, int y, int towerID);

	std::vector<float> xpn;
	std::vector<float> ypn;
	void initPerlinNoise();
};




