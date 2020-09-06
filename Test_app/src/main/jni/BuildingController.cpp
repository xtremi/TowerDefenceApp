#include "BuildingController.h"

Tower* BuildingController::addNewTower(Cell* cell, Map* map) {
	TowerAim* twr = new TowerAim(&towerDescriptors[1]);
	twr->init(cell);
	towers.push_back(twr);
		
	td::direction dir = determineTowerFaceDirection(cell, map, (int)twr->rangeCells());
	twr->face(dir);
	assignCellsInTowerRange(twr, map);

	return twr;
}

td::direction BuildingController::determineTowerFaceDirection(Cell* cell, Map* map, int range) {
	int nNorth, nSouth, nWest, nEast;
	map->roadCellCount(cell, 2, nNorth, nSouth, nWest, nEast);

	if (nNorth > nSouth && nNorth > nWest && nNorth > nEast)
		return td::direction::north;
	else if (nSouth > nWest && nSouth > nEast)
		return td::direction::south;
	else if (nWest > nEast)
		return td::direction::west;
	return td::direction::east;
}


Tower* BuildingController::upgradeTower(Cell* cell, Map* map, int towerID) {
	Tower*		currentTower = (Tower*)cell->getBuilding();
	Tower*		newTower = NULL;
	tower_type	currentTowerType = currentTower->getDescriptor()->towerType;
	tower_type	newTowerType = towerDescriptors[towerID].towerType;
	glm::vec2 towerOrientation = currentTower->getDirection();


	cell->deleteBuilding();
	removeTower(currentTower);
	if (newTowerType == tower_type::aimless)
		newTower = new TowerAimless(&towerDescriptors[towerID]);		
	else if (newTowerType == tower_type::aim)
		newTower = new TowerAim(&towerDescriptors[towerID]);
	else if (newTowerType == tower_type::multi_dir)
		newTower = new TowerMultiDir(&towerDescriptors[towerID]);
	else if (newTowerType == tower_type::block)
		newTower = new TowerBlock(&towerDescriptors[towerID]);
	else if (newTowerType == tower_type::aoe)
		newTower = new TowerAOE(&towerDescriptors[towerID]);
	newTower->init(cell);
	newTower->setDirection(towerOrientation);
	
	//cell->setBuilding(newTower);
	towers.push_back(newTower);
	
	assignCellsInTowerRange(newTower, map);
	return newTower;
}

Tower* BuildingController::upgradeTowerToMega(Cell* cell, Map* map) {

	Tower* centerTower = (Tower*)cell->getBuilding();
	centerTower->megafy();

	std::vector<Cell*> neighbourCells = map->getNeighbouringCells(cell->mapLocX(), cell->mapLocY());
	for (Cell* c : neighbourCells) {
		
		if (Tower* tower = (Tower*)c->getBuilding()) {
			c->deleteBuilding();
			removeTower(tower);
		}
		c->setBuildingRefOnly(centerTower);
	}

	return centerTower;
}

float BuildingController::sellBuildingAtCell(Cell* c) {
	float value = 0.0f;
	if (Building* building = c->getBuilding()) {
		
		
		building->deleteBuilding();			//remove building sprites sprites
		value = building->value();	

		if (building->isTower())			//remove from building collectors
			removeTower((Tower*)building);
		else
			removeBuilding(building);			

		c->removeBuildingReference();		//remove cell reference to building
	}
	return value;
}

void BuildingController::removeTower(Tower* t) {
	std::vector<Tower*>::iterator p = std::find(towers.begin(), towers.end(), t);	
	if (p != towers.end()) {
		towers.erase(p);		
	}
	delete t;
}

void BuildingController::removeBuilding(Building* b) {
	std::vector<Building*>::iterator p = std::find(buildings.begin(), buildings.end(), b);	
	if (p != buildings.end())
		buildings.erase(p);
	delete b;
}



void BuildingController::assignCellsInRange_circleArea(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange) {
	float		r = twr->rangeCells();
	map->getRoadCellsInRange(map->getCellAtLocation(twr->getPosition()), r, cellsInRange);
}

void BuildingController::assignCellsInRange_straightLine(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange) {
	int	ncellsInRange = (int)twr->rangeCells();
	glm::vec2	  dir = twr->getDirection();	
	td::direction rangeDirection;

	if (dir.x == 0.0f)
		rangeDirection = dir.y < 0.0f ? td::direction::north : td::direction::south;
	else
		rangeDirection = dir.x < 0.0f ? td::direction::west : td::direction::east;

	map->getRoadCellsInStraightLine(map->getCellAtLocation(twr->getPosition()), rangeDirection, ncellsInRange, cellsInRange);
}

void BuildingController::assignCellsInTowerRange(Tower* twr, Map* map) {

	std::vector<Cell*> cellsInRange;
	glm::vec2	dir = twr->getDirection();
	tower_type towerType = twr->getDescriptor()->towerType;

	CellRange* cellRange = nullptr;
	switch (towerType)
	{
	case tower_type::aim:
	case tower_type::aoe:
	case tower_type::block:	
		assignCellsInRange_circleArea(twr, map, cellsInRange);
		cellRange = new CellRange();
		break;
	case tower_type::multi_dir:
		assignCellsInRange_circleArea(twr, map, cellsInRange);
		cellRange = new QuadrantCellRange();
		break;
	case tower_type::aimless:
	default:
		cellRange = new CellRange();
		assignCellsInRange_straightLine(twr, map, cellsInRange);
		break;
	}
	cellRange->assignCells(cellsInRange, twr->getPosition(), twr->rangeDistance());
	//std::sort(cellsInRange.begin(), cellsInRange.end(), cellCompareRoadIDgreater);
	twr->setCellsInRange(cellRange);

}

void BuildingController::update(Map* map) {
	profit = 0.0f;
	for (Tower* t : towers) {
		t->update(map);

		std::vector<MobEffectFromTower>* effNew		= t->getNewMobEffects();
		std::vector<MobEffectFromTower>* effRemove  = t->getMobEffectsToRemove();

		for (int i = 0; i < effNew->size(); i++)
			mobEffectControl.addEffect((*effNew)[i].effectStats, (*effNew)[i].mob, t);
		
		for (int i = 0; i < effRemove->size(); i++)
			mobEffectControl.removeEffect((*effRemove)[i].effectStats, (*effRemove)[i].mob, t);

		profit += t->getRoundProfit();	
	}

	for (Tower* t : towers) {
		

	}


	for (Building* b : buildings) {
		b->update(map);
	}

	mobEffectControl.process();
}


void BuildingController::hideBuildingDebugInfo() {
	for (Tower* t : towers)
		t->hideDebugSprites();
	for (Building* b : buildings)
		b->hideDebugSprites();
}
void BuildingController::showBuildingDebugInfo() {
	for (Tower* t : towers)
		t->showDebugSprites();
	for (Building* b : buildings)
		b->showDebugSprites();
}

void BuildingController::loadTowerDescriptors() {
	TowerJSONReader towerReader;
	std::string errmsg = "";
	if (!towerReader.readBullets(BULLET_DESCRIPTOR_JSON_FILE, errmsg)) {
		bool err = true; //TODO: handle error
	}
	if (!towerReader.readTowers(&towerDescriptors, TOWER_DESCRIPTOR_JSON_FILE, errmsg)) {
		bool err = true; //TODO: handle error
	}
}

