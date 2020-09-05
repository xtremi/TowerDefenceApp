#include "Cell.h"


Cell::Cell(const glm::vec2& pos, int locx, int locy) : Sprite()//, UIButton()
{
	setPosition(pos);
	maploc[0] = locx;
	maploc[1] = locy;
	setSize(glm::vec2(CELL_SIZE, CELL_SIZE));
	setType(cell_type::grass);
	setDepth(CELL_DEPTH);
}

void Cell::assignCellDirection(td::direction in, td::direction out) {
	dir_in = in;
	dir_out = out;
}

void Cell::setType(cell_type ct) {
	_type = ct;
	setCellImage();
}

void Cell::setValid(bool state) 
{ 
	if(state != _valid){
		_valid = state;
		setCellImage();
	}
}

void Cell::setCellImage() {
	switch (_type)
	{
	case Cell::cell_type::grass:
		_valid ? setImage(GETIMG(ASSET::IMG_GRASS)) : setImage(GETIMG(ASSET::IMG_GRASS_INACTIVE));
		break;
	case Cell::cell_type::road:
		_valid ? setImage(GETIMG(ASSET::IMG_ROAD)) : setImage(GETIMG(ASSET::IMG_ROAD_INACTIVE));
		break;
	case Cell::cell_type::entrance:
		setImage(GETIMG(ASSET::IMG_ROAD_START)); break;
	case Cell::cell_type::exit:
		setImage(GETIMG(ASSET::IMG_ROAD_END)); 	break;
	default:
		break;
	}
}

bool Cell::hasTurn() {
	return !isAnyOf(td::direction::north, td::direction::south, dir_in, dir_out) || isAnyOf(td::direction::east, td::direction::west, dir_in, dir_out);
}

glm::vec2 Cell::posCenter() {
	return getPos() + CELL_SIZE / 2.0f;
}

glm::vec2 Cell::posIn() {
	glm::vec2 dpos;
	if (dir_in == td::direction::north)
		dpos = glm::vec2(CELL_SIZE / 2.0f, 0.0f);
	else if (dir_in == td::direction::south)
		dpos = glm::vec2(CELL_SIZE / 2.0f, CELL_SIZE);
	else if (dir_in == td::direction::east)
		dpos = glm::vec2(CELL_SIZE, CELL_SIZE / 2.0f);
	else
		dpos = glm::vec2(0.0f, CELL_SIZE / 2.0f);
	return dpos + getPos();
}



float Cell::distanceFromPath(const glm::vec2& pos) {
	glm::vec2 localPosition = getLocalPosInCell(pos);

	if (dir_in == td::direction::south || dir_in == td::direction::north)
		return std::abs(localPosition.x);
	return std::abs(localPosition.y);
}

glm::vec2 Cell::getLocalPosInCell(const glm::vec2& pos) {
	return (pos - getPos() - glm::vec2(CELL_SIZE / 2.0f));
}
bool Cell::positionIsPassedCellCenter(const glm::vec2& pos) {
	glm::vec2 localPosition = getLocalPosInCell(pos);
	return (
		dir_in == td::direction::north && localPosition.y > 0.0f ||
		dir_in == td::direction::south && localPosition.y < 0.0f ||
		dir_in == td::direction::east && localPosition.x < 0.0f ||
		dir_in == td::direction::west && localPosition.x > 0.0f
		);
}
bool Cell::positionIsPassedCellDiagonal(const glm::vec2& pos) {
	glm::vec2 localPosition = getLocalPosInCell(pos);	
	if (
		(dir_in == td::direction::south && dir_out == td::direction::east) ||
		(dir_in == td::direction::west && dir_out == td::direction::north)
	){
		return isLeftOfUnitLine(glm::vec2(1.0f, 1.0f), localPosition);
	}
	else if (
		(dir_in == td::direction::south && dir_out == td::direction::west) ||
		(dir_in == td::direction::east && dir_out == td::direction::north)
		) {
		return isLeftOfUnitLine(glm::vec2(1.0f, -1.0f), localPosition);
	}
	else if (
		(dir_in == td::direction::east && dir_out == td::direction::south) ||
		(dir_in == td::direction::north && dir_out == td::direction::west)
		) {
		return isLeftOfUnitLine(glm::vec2(-1.0f, -1.0f), localPosition);
	}
	else if (
		(dir_in == td::direction::west && dir_out == td::direction::south) ||
		(dir_in == td::direction::north && dir_out == td::direction::east)
		) {
		return isLeftOfUnitLine(glm::vec2(-1.0f, 1.0f), localPosition);
	}
	return false;
}

void Cell::removeBuildingReference() {
	building = NULL;
}

void Cell::deleteBuilding() {
	if(building){
		building->deleteBuilding();
		removeBuildingReference();
	}
}

//This should maybe be done in Map class?
void Cell::setBuildingRefOnly(Building* bld) {
	building = bld;
}
void Cell::setBuilding(Building* bld) {
	setBuildingRefOnly(bld);
	building->setPosition(this->posCenter(), maploc[0], maploc[1]);
}

Mob* Cell::firstMob() {
	mobsIterator = 0;
	return nextMob();
}
Mob* Cell::lastMob(){
	mobsIterator = mobs.size() - 1;
	return prevMob();
}
Mob* Cell::nextMob() {
	if (mobsIterator < mobs.size())
		return mobs[mobsIterator++];
	return NULL;
}
Mob* Cell::prevMob() {
	if (mobsIterator >= 0)
		return mobs[mobsIterator--];
	return NULL;
}
std::vector<Mob*>* Cell::getMobs() {
	return &mobs;
}
int Cell::getNumberOfMobs() {
	return mobs.size();
}
Mob* Cell::getMob(int i) {
	return mobs[i];
}
void Cell::clearMobs() {
	mobs.clear();
}
void Cell::addMob(Mob* m) {
	mobs.push_back(m);
}

bool Cell::isRoad() {
	return (_type == cell_type::road);
}

td::direction Cell::getMoveDirection(const glm::vec2& pos) {
	td::direction movedir;
	if (!hasTurn()) {
		return dirout();
	}
	else {
		if (!positionIsPassedCellDiagonal(pos))
			return oppositeDirectionOf(dirIn());
		else {
			return dirout();
		}
	}
}



bool cellCompareRoadIDgreater(Cell* c1, Cell* c2) { return (c1->getRoadID() > c2->getRoadID()); }


CellRange::CellRange() {}
QuadrantCellRange::QuadrantCellRange() : CellRange() {}

void CellRange::assignCells(const std::vector<Cell*>& cells, const glm::vec2& _cpos, float _realRange) {
	allCells	= cells;		
	realRange	= _realRange;
	cpos		= _cpos;

	ncells = allCells.size();
	std::sort(allCells.begin(), allCells.end(), cellCompareRoadIDgreater);
}


Mob* CellRange::firstMobInRange(bool checkRealRange){
	cellIt = firstCellInRange();
	mobIt = nullptr;
	if (!cellIt) return false;	
	return nextMobInRange(checkRealRange);
}
Mob* CellRange::nextMobInRange(bool checkRealRange) {
	
	if (mobIt) 
		mobIt = cellIt->nextMob();
	else
		mobIt = cellIt->firstMob();

	while (!mobIt) {
		mobIt = cellIt->nextMob();
		if (!mobIt) {
			cellIt = nextCellInRange();
			if (!cellIt) return nullptr;
			mobIt = cellIt->firstMob();
		}
	}
	
	if(mobIt->isAlive() && (!checkRealRange || mobIsInRange(mobIt)))
		return mobIt;
	return nextMobInRange(checkRealRange);
}

Cell* CellRange::firstCellInRange() {
	cellInRangeIterator = 0;
	return nextCellInRange();
}
Cell* CellRange::lastCellInRange() {
	cellInRangeIterator = allCells.size() - 1;
	return prevCellInRange();
}
Cell* CellRange::nextCellInRange() {
	if (cellInRangeIterator < allCells.size())
		return allCells[cellInRangeIterator++];
	return nullptr;
}
Cell* CellRange::prevCellInRange() {
	if (cellInRangeIterator >= 0)
		return allCells[cellInRangeIterator--];
	return nullptr;
}

Cell* CellRange::getCellInRange(int i) {
	if (i >= 0 && i < allCells.size())
		return allCells[i];
	return nullptr;
}
int CellRange::getNumberOfCellsInRange() {
	return ncells;
}


Mob* CellRange::getFirstMobInRange(bool checkRealRange) {
	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {
			if (m->isAlive() && (!checkRealRange || mobIsInRange(m))) {
				return m;
			}
		}
	}
	return nullptr;
}


Mob* CellRange::getLastMobInRange(bool checkRealRange) {
	for (Cell* ct = lastCellInRange(); ct; ct = prevCellInRange()) {
		for (Mob* m = ct->lastMob(); m; m = ct->prevMob()) {
			if (m->isAlive() && (!checkRealRange || mobIsInRange(m))) {
				return m;
			}
		}
	}
	return nullptr;
}

int CellRange::nMobsInRange(bool checkRealRange) {
	int nmobs = 0;
	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {
			if (m->isAlive() && (!checkRealRange || mobIsInRange(m))) {
				nmobs++;
			}
		}
	}
	return nmobs;
}
bool CellRange::mobIsInRange(Mob* m) {
	float targetDistance = glm::distance(cpos, m->getPos());
	return targetDistance < realRange;
}

bool CellRange::checkIfHasMobInRange() {
	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {
			if (m->isAlive() && mobIsInRange(m)) {
				return true;
			}
		}
	}
	return false;
}

/*
	Not a good implementation:
	If direction is directly south/north/east/west, this is not correct.
*/
Mob* CellRange::findTargetInSector(const glm::vec2& dir, float sectorAngle) {

	glm::ivec2 signVec(glm::sign(dir.x), glm::sign(dir.y));

	bool isWestDir = signVec.x < 0;
	bool isNorthDir = signVec.y < 0;

	float minAng = sectorAngle / 2.0f;

	for (Cell* c : allCells) {

		if (Mob* m = c->firstMob()) {

			bool cIsWest = c->getPos().x <= cpos.x;
			bool cIsNorth = c->getPos().y <= cpos.y;

			//Cell is in the correct quadrant:
			if (cIsWest == isWestDir && isNorthDir == cIsNorth) {
			
				glm::vec2 dirToCell = glm::normalize(m->getPos() - cpos);
				float ang = std::fabs(glm::angle(dirToCell, dir));
				if (ang < minAng) {
					return m;
				}
			}
		}
	}
	return nullptr;
}

/*
	Finds a new target:
	- starts at cell where current target is, then looks N number of cells (cellsToCheck)
	  in backwards, forward or both path direction, for a new target.
*/
Mob* findTargetAlongPath(Cell* cell, int cellsToCheck, td::path_direction dir){
	Cell* currentCell			= cell;
	bool  lookForNewTarget		= true;
	int   numberOfCheckedCells	= 0;
	
	td::path_direction pdir = dir;
	if (dir == td::path_direction::any)
		pdir = td::path_direction::forward;

	while (lookForNewTarget) {
		currentCell = dir == td::path_direction::forward ? currentCell->getNextCell() : currentCell->getPrevCell();
		
		Mob* m = cell->firstMob();

		if (Mob* m = cell->firstMob()) {
			return m;
			lookForNewTarget = false;
		}
		numberOfCheckedCells++;
		if (numberOfCheckedCells > cellsToCheck)
			lookForNewTarget = false;
	}

	if (dir == td::path_direction::any)
		return findTargetAlongPath(cell, cellsToCheck, td::path_direction::backward);

	return nullptr;
}

void QuadrantCellRange::setQuadrantCells(const glm::vec2& cpos) {

}