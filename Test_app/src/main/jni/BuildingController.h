#pragma once
#include "TowerAimless.h"
#include "TowerAim.h"
#include "TowerMultiDir.h"
#include "TowerBlock.h"
#include "TowerAOE.h"
#include "TowerJSONReader.h"
#include "MobEffectController.h"
#include "Map.h"
#include <vector>
#include <algorithm>
#include <sstream>

#define TOWER_DESCRIPTOR_JSON_FILE "json/towers_blender.json"
#define BULLET_DESCRIPTOR_JSON_FILE "json/bullet_defs.json"

class BuildingController {

public:
	BuildingController(){}
	
	void loadTowerDescriptors();
	void update(Map* map);
	void hideBuildingDebugInfo();
	void showBuildingDebugInfo();

	Tower* addNewTower(Cell* cell, Map* map);
	Tower* upgradeTower(Cell* cell, Map* map, int towerID);
	Tower* upgradeTowerToMega(Cell* cell, Map* map);
	
	void assignCellsInTowerRange(Tower* twr, Map* map);
	
	Tower* firstTower() {
		towerIter = 0;
		return nextTower();
	}
	Tower* nextTower() {
		if(towerIter < towers.size())
			return towers[towerIter++];
		return NULL;
	}

	TowerDescriptor* getTowerDescriptor(int id) {
		return &towerDescriptors[id];
	}

	TowerDescriptor* firstTowerDescriptor() {
		towerDescrIt = towerDescriptors.begin();
		return nextTowerDescriptor();
	}
	TowerDescriptor* nextTowerDescriptor() {
		if (towerDescrIt != towerDescriptors.end()){
			TowerDescriptor* td = &towerDescrIt->second;
			towerDescrIt++;
			return td;
		}
		return NULL;
	}

	float getProfit() { return profit; }

	float sellBuildingAtCell(Cell* c);
	
	void getNumberOfMobEffects(int& totalEffects, int& totalMobsWithEffect) {
		totalEffects = mobEffectControl.getTotalNumberOfEffects();
		totalMobsWithEffect = mobEffectControl.getTotalNumberOfMobWithEffect();
	}

private:
	MobEffectController				mobEffectControl;
	std::vector<Tower*>				towers;
	std::vector<Building*>			buildings;
	std::map<int, TowerDescriptor>	towerDescriptors;
	std::map<int, TowerDescriptor>::iterator towerDescrIt;
	int towerIter	= 0;
	float profit	= 0.0f;

	void removeTower(Tower* t);
	void removeBuilding(Building* t);

	td::direction determineTowerFaceDirection(Cell* cell, Map* map, int range);
	void assignCellsInRange_circleArea(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange);
	void assignCellsInRange_straightLine(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange);
};
