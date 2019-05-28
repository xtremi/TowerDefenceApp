#pragma once
#include "Tower.h"
#include "TowerJSONReader.h"
#include "Map.h"
#include <vector>
#include <algorithm>
#include <sstream>

#define TOWER_DESCRIPTOR_JSON_FILE "json/tower_defs.json"
#define BULLET_DESCRIPTOR_JSON_FILE "json/bullet_defs.json"

class BuildingController {

public:
	BuildingController(){}
	void loadTowerDescriptors();

	Tower* addNewTower(Cell* cell, Map* map);
	Tower* upgradeTower(Cell* cell, Map* map, int towerID);

	void assignCellsInRange_circleArea(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange);
	void assignCellsInRange_straightLine(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange);
	void assignCellsInTowerRange(Tower* twr, Map* map);


	void update();

	Tower* firstTower() {
		towerIter = 0;
		return nextTower();
	}
	Tower* nextTower() {
		if(towerIter < towers.size())
			return towers[towerIter++];
		return NULL;
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

private:
	std::vector<Tower*>		towers;
	std::vector<Building*>	buildings;
	int towerIter = 0;
	float profit = 0.0f;

	std::map<int, TowerDescriptor> towerDescriptors;	
	std::map<int, TowerDescriptor>::iterator towerDescrIt;
};
