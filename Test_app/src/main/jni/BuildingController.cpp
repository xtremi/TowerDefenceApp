#include "BuildingController.h"

Tower* BuildingController::addNewTower(Cell* cell, Map* map) {
	Tower* twr = new Tower(&towerDescriptors[1]);
	towers.push_back(twr);
	cell->setBuilding(twr);
	assignCellsInTowerRange(twr, map);
	return twr;
}

Tower* BuildingController::upgradeTower(Cell* cell, Map* map, int towerID) {
	Building* currentBuilding = cell->getBuilding();
	((Tower*)currentBuilding)->upgradeTo(&towerDescriptors[towerID]);
	assignCellsInTowerRange((Tower*)currentBuilding, map);

	return (Tower*)currentBuilding;
}

Tower* BuildingController::upgradeTowerToMega(Cell* cell, Map* map) {

	Tower* centerTower = (Tower*)cell->getBuilding();
	centerTower->megafy();

	std::vector<Cell*> neighbourCells = map->getNeighbouringCells(cell->mapLocX(), cell->mapLocY());
	for (Cell* c : neighbourCells) {
		Tower* tower = (Tower*)c->getBuilding();
		if (tower) {
			c->deleteBuilding();
			removeTower(tower);
		}
		c->setBuildingRefOnly(centerTower);
	}

	return centerTower;
}

void BuildingController::removeTower(Tower* t) {
	std::vector<Tower*>::iterator p = std::find(towers.begin(), towers.end(), t);
	if (p != towers.end()) 
		towers.erase(p);
	delete t;
}

void BuildingController::removeBuilding(Building* b) {
	std::vector<Building*>::iterator p = std::find(buildings.begin(), buildings.end(), b);
	if (p != buildings.end())
		buildings.erase(p);
	delete b;
}



void BuildingController::assignCellsInRange_circleArea(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange) {
	float		r = twr->getRange();
	int			mx = twr->getLocX();
	int			my = twr->getLocY();
	
	glm::vec2 tpos(mx, my);

	int dx = 1;
	int dy = r;
	while (dy > 0) {

		bool inRange = true;
		while(inRange){
			glm::vec2 cpos(mx + dx, my + dy);
			float d = glm::distance(tpos, cpos);

			if (d < (r + 1)) {
				Cell* cq1 = map->getCell(mx + dx, my + dy);
				Cell* cq2 = map->getCell(mx + dx, my - dy);
				Cell* cq3 = map->getCell(mx - dx, my + dy);
				Cell* cq4 = map->getCell(mx - dx, my - dy);
				if (cq1 && cq1->isRoad()) cellsInRange.push_back(cq1);
				if (cq2 && cq2->isRoad()) cellsInRange.push_back(cq2);
				if (cq3 && cq3->isRoad()) cellsInRange.push_back(cq3);
				if (cq4 && cq4->isRoad()) cellsInRange.push_back(cq4);
			}
			else
				inRange = false;
			dx++;
		}
		dx = 1;
		dy--;
	}

	for (int i = 1; i < (r + 1); i++) {
		Cell* cN = map->getCell(mx + 0, my + i);
		Cell* cS = map->getCell(mx + 0, my - i);
		Cell* cE = map->getCell(mx + i, my + 0);
		Cell* cW = map->getCell(mx - i, my - 0);
		if (cN && cN->isRoad()) cellsInRange.push_back(cN);
		if (cS && cS->isRoad()) cellsInRange.push_back(cS);
		if (cE && cE->isRoad()) cellsInRange.push_back(cE);
		if (cW && cW->isRoad()) cellsInRange.push_back(cW);
	}


	/*
	[][][][][][]
	[][][][][][]
	[][][][][][]
	[][][][][][]
	*/
}

void BuildingController::assignCellsInRange_straightLine(Tower* twr, Map* map, std::vector<Cell*>& cellsInRange) {
	float		r = twr->getRange();
	glm::vec2	dir = twr->getDirection();
	int			mx = twr->getLocX();
	int			my = twr->getLocY();

	int ncellsInRange = r + 1;
	direction rangeDirection;

	if (dir.x == 0.0f)
		rangeDirection = dir.y < 0.0f ? direction::north : direction::south;
	else
		rangeDirection = dir.x < 0.0f ? direction::west : direction::east;


	int i = mx, j = my;
	for (int k = 0; k < ncellsInRange; k++) {
		switch (rangeDirection)
		{
		case direction::north: j++;	break;
		case direction::south: j--; break;
		case direction::east:  i++; break;
		case direction::west:  i--;	break;
		case direction::unset:
		default: break;
		}

		Cell* c = map->getCell(i, j);
		if (c){
			if(c->isRoad())
				cellsInRange.push_back(c);
		}
	}

}

void BuildingController::assignCellsInTowerRange(Tower* twr, Map* map) {

	std::vector<Cell*> cellsInRange;
	glm::vec2	dir = twr->getDirection();

	if(!twr->canAim()){
		//Straight line: (easy)
		if (dir.x == 0.0f || dir.y == 0.0f) {
			assignCellsInRange_straightLine(twr, map, cellsInRange);
		}
	}
	else {
		assignCellsInRange_circleArea(twr, map, cellsInRange);
	}

	std::sort(cellsInRange.begin(), cellsInRange.end(), cellCompareRoadIDgreater);
	twr->setCellsInRange(cellsInRange);
}

void BuildingController::update() {
	profit = 0.0f;
	for (Tower* t : towers) {
		t->process();
		profit += t->getRoundProfit();
	}
	for (Building* b : buildings) {
		b->process();
	}
}

void BuildingController::loadTowerDescriptors() {
	TowerJSONReader towerReader;
	std::string errmsg = "";
	if (!towerReader.readBullets(BULLET_DESCRIPTOR_JSON_FILE, errmsg)) {
		//handle error:
	}
	if (!towerReader.readTowers(&towerDescriptors, TOWER_DESCRIPTOR_JSON_FILE, errmsg)) {
		//handle error:
	}

}

