#include "Map.h"

Map::Map(){
	setupProbabilities();
}
Map::~Map(){}


void Map::createMap(const glm::vec2& pos, int _dimx, int _dimy, int _startX, int _startY)
{
	dimx = _dimx;
	dimy = _dimy;
	mappos = pos;
	
	createDefaultCells();
	setMobStartcell(_startX, _startY);
	createTrailCells();
}

void Map::createDefaultCells()
{
	float maxY = dimy * CELL_SIZE;

	for (int i = 0; i < dimx; i++)
	{
		std::vector<Cell> row;
		cells.push_back(row);
		for (int j = 0; j < dimy; j++)
		{
			glm::vec2 pos(i * CELL_SIZE, maxY - j*CELL_SIZE - CELL_SIZE);
			Cell newCell(pos + mappos, i, j);
			cells[i].push_back(newCell);
		}
	}
}



/*
[north]
	[north] [PN, PS, PE, PW]
	[south] [PN, PS, PE, PW]
	[east]  [PN, PS, PE, PW]
	[west]  [PN, PS, PE, PW]
[south]
	[north][PN, PS, PE, PW]
	...
*/
void Map::setupProbabilities() {
	std::map<td::direction, probabilities> pFromNorth;
	pFromNorth[td::direction::north]	= probabilities(0, 0, 0, 0);	
	pFromNorth[td::direction::south]	= probabilities(0, 60, 20, 20);
	pFromNorth[td::direction::east]		= probabilities(0, 30, 70, 0);
	pFromNorth[td::direction::west]		= probabilities(0, 30, 0, 70);

	std::map<td::direction, probabilities> pFromSouth;
	pFromSouth[td::direction::north] = probabilities(0, 20, 40, 40);
	pFromSouth[td::direction::south] = probabilities(0, 0, 0, 0);
	pFromSouth[td::direction::east] = probabilities(0, 30, 70, 0);
	pFromSouth[td::direction::west] = probabilities(0, 30, 0, 70);

	std::map<td::direction, probabilities> pFromEast;
	pFromEast[td::direction::north] = probabilities(0, 60, 20, 20);
	pFromEast[td::direction::south] = probabilities(60, 0, 20, 20);
	pFromEast[td::direction::east] = probabilities(0, 0, 0, 0);
	pFromEast[td::direction::west] = probabilities(0, 5, 0, 95); //.

	std::map<td::direction, probabilities> pFromWest;
	pFromWest[td::direction::north] = probabilities(0, 60, 20, 20);
	pFromWest[td::direction::south] = probabilities(60, 0, 20, 20);
	pFromWest[td::direction::east] = probabilities(0, 5, 95, 0); //.
	pFromWest[td::direction::west] = probabilities(0, 0, 0, 0);

	probs[td::direction::north] = pFromNorth;
	probs[td::direction::south] = pFromSouth;
	probs[td::direction::east] = pFromEast;
	probs[td::direction::west] = pFromWest;
}


void incrementCellIndex(td::direction dirout, int& x, int& y) {
	switch (dirout){
		case td::direction::north:
			y++; break;
		case td::direction::south:
			y--; break;
		case td::direction::east:
			x++; break;
		case td::direction::west:
			x--; break;
		case td::direction::unset:
		default:
			break;
	}
}

void Map::createTrailCells()
{
	mobStartcell->assignCellDirection(td::direction::south, td::direction::north);
	mobStartcell->setRoadID(0);

	Cell* previousCell = mobStartcell;

	int currentX = mobStartcell->mapLocX();
	int currentY = mobStartcell->mapLocY();
	int counter = 0;

	while (currentY < (dimy - 1)) {
		counter++;

		td::direction newDirIn, newDirOut;
		newDirIn = oppositeDirectionOf(previousCell->dirout());
		incrementCellIndex(previousCell->dirout(), currentX, currentY);

		probabilities* p = &probs[previousCell->dirIn()][previousCell->dirout()];
		bool validMove = false;
		while(!validMove){
			float r = (float)agk::Random(1, 100);
						
			if (r <= p->go_south)
				newDirOut = td::direction::south;
			else if (r <= p->go_north)
				newDirOut = td::direction::north;
			else if (r <= p->go_east)
				newDirOut = td::direction::east;				
			else
				newDirOut = td::direction::west;
			
			if ((isNearEastBorder(currentX) && newDirOut == td::direction::east) ||
				(isNearWestBorder(currentX) && newDirOut == td::direction::west))
				validMove = false;
			else if (previousCell->dirIn() == td::direction::east && newDirOut == td::direction::east)
				validMove = false;
			else if (previousCell->dirIn() == td::direction::west && newDirOut == td::direction::west)
				validMove = false;
			else
				validMove = true;

			if (currentY == 0)
				validMove = false;

			if (currentX < 0 || currentX == dimx)
				validMove = false;

			if (newDirIn == newDirOut)
				validMove = false;
		}
		
		Cell* newCell = &cells[currentX][currentY];
		newCell->setRoadID(counter);
		newCell->setType(Cell::cell_type::road);
		newCell->assignCellDirection(newDirIn, newDirOut);
		newCell->setPrevCell(previousCell);
		previousCell->setNextCell(newCell);
		//agk::Sync();
		previousCell = newCell;

	}
}

void Map::setCellRowsValidState(int rowFrom, int rowTo, bool state) {
	for (int i = rowFrom; i < (rowTo + 1); i++)
		setCellRowValidState(i, state);
}

void Map::setCellRowValidState(int row, bool state) {
	for (int i = 0; i < dimx; i++) {
		cells[i][row].setValid(state);
	}
}

void Map::hide()
{
	setVisibilityState(false);
}

void Map::show()
{
	setVisibilityState(true);
}

void Map::setVisibilityState(bool state)
{
	int realDimX = cells.size();
	int realDimY = realDimX > 0 ? cells[0].size() : 0;

	for (int i = 0; i < realDimX; i++)
	{
		for (int j = 0; j < realDimY; j++)
		{
			cells[i][j].setState(state);
		}
	}
}


Cell* Map::getCellAtLocation(glm::vec2 loc) {
	loc -= mappos;
	loc.y = dimy * CELL_SIZE - loc.y; //invert y

	if (loc.y >= 0.0f) {
		int i = loc.x / CELL_SIZE;
		int j = loc.y / CELL_SIZE;

		if (i < 0 || j < 0 || i >(this->dimx - 1) || j >(this->dimy - 1))
			return NULL;
		return &cells[i][j];
	}
	return NULL;
}

Cell* Map::getCell(int x, int y) {
	if (isInBounds(x, y))
		return &cells[x][y];
	return NULL;
}
bool Map::isInBounds(int x, int y) {
	return !(x < 0 || x >= dimx || y < 0 || y >= dimy);
}

int Map::numberOfCellsWithMobs() {
	return cellContainingMobs.size();
}
void Map::clearCellsContainingMobs() {
	for (Cell* c : cellContainingMobs)
		c->clearMobs();
	cellContainingMobs.clear();
}
void Map::addCellContainingMobs(Cell* c) {
	//cellContainingMobs.push_back(c);
	cellContainingMobs.insert(c);
}

Cell* Map::firstCellWithMobs() {
	cellWithMobsIter = cellContainingMobs.begin();
	return nextCellWithMobs();
}
Cell* Map::nextCellWithMobs() {
	if (cellWithMobsIter != cellContainingMobs.end()){
		Cell* c = (*cellWithMobsIter);
		cellWithMobsIter++;
		return c;
	}
	return NULL;
}

Cell* Map::getPreviousCellOf(Cell* c) {
	if (!c) return nullptr;

	td::direction dirIn = c->dirIn();
	int x = c->mapLocX();
	int y = c->mapLocY();
	incrementCellIndex(dirIn, x, y);
	return getCell(x, y);
}

Cell* Map::getNextRoadCellOf(Cell* c) {
	if (!c) return nullptr;

	td::direction dirOut = c->dirout();
	int x = c->mapLocX();
	int y = c->mapLocY();
	incrementCellIndex(dirOut, x, y);
	return getCell(x, y);
}
Cell* Map::getNextRoadCellOf(int x, int y) {
	return getNextRoadCellOf(getCell(x, y));
}
Cell* Map::getPreviousCellOf(int x, int y) {
	return getPreviousCellOf(getCell(x, y));
}

Cell* Map::getMobEndCell() {
	return mobEndcell;
}
void Map::setMobEndCell(UINT x, UINT y) {
	setMobEndCell(getCell(x, y));
}
void Map::setMobEndCell(Cell* c) {
	if(c){
		if (mobEndcell)
			mobEndcell->setType(Cell::cell_type::road);

		mobEndcell = c;
		mobEndcell->setType(Cell::cell_type::exit);
	}
}
void Map::setMobEndCellFromCell(int nBlocks, Cell* fromCell) {
	Cell* nextCell = fromCell;
	for (int i = 0; i < nBlocks; i++) {
		nextCell = getNextRoadCellOf(nextCell);
		if (!nextCell) {
			nextCell = fromCell;
			break;
		}
	}
	setMobEndCell(nextCell);
}
Cell* Map::getMobStartCell() {
	return mobStartcell;
}
void Map::setMobStartcell(UINT x, UINT y) {
	mobStartcell = getCell(x, y);
	mobStartcell->setType(Cell::cell_type::entrance);
}

bool Map::isNearWestBorder(int x) {
	return x == (1 + N_BORDER_CELL);
}
bool Map::isNearEastBorder(int x) {
	return x == (dimx - 2 - N_BORDER_CELL);
}
bool Map::isNotOnWestBorder(int x) {
	return !isOnWestBorder(x);
}
bool Map::isNotOnEastBorder(int x) {
	return !isOnEastBorder(x);
}
bool Map::isOnWestBorder(int x) {
	return x == N_BORDER_CELL;
}
bool Map::isOnEastBorder(int x) {
	return x == (dimx - 1 - N_BORDER_CELL);
}

std::vector<Cell*> Map::getNeighbouringCells(int x, int y) {

	std::vector<Cell*> neighbours;
	static const int ddx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	static const int ddy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	for (int i = 0; i < 8; i++) {
		int nx = x + ddx[i];
		int ny = y + ddy[i];
		neighbours.push_back(getCell(nx, ny));
	}
	return neighbours;
}

bool Map::buildingCanUpgradeToMega(int x, int y) {
	Cell* cell = getCell(x, y);
	if (!cell || !cell->getBuilding())
		return false;

	int buildingTypeID = cell->getBuilding()->getBuildingID();

	std::vector<Cell*> neighbourCells = getNeighbouringCells(x, y);
	for (Cell* c : neighbourCells) {
		if (!c || !c->getBuilding())
			return false;
		if (c->getBuilding()->getBuildingID() != buildingTypeID)
			return false;
	}

	return true;
}


void Map::roadCellCount(Cell* c, int range, int& north, int& south, int& west, int& east) {

	north = 0; south = 0; west = 0; east = 0;

	std::vector<Cell*> neighbours;
	static const int ddx[4] = { 0,  0, -1, 1 };
	static const int ddy[4] = { 1, -1,  0, 0 };
	int* neighbourCount[4] = { &north, &south, &west, &east };
	int x = c->mapLocX();
	int y = c->mapLocY();

	for(int r = 1; r <= range; r++){
		for (int i = 0; i < 4; i++) {
			int nx = x + r*ddx[i];
			int ny = y + r*ddy[i];
			Cell* nc = getCell(nx, ny);
			if (nc && nc->isRoad())
				(*neighbourCount[i])++;
		}
	}
}

void Map::getRoadCellsInStraightLine(Cell* c, td::direction dir, float range, std::vector<Cell*>& cellsInRange) {
	
	int			mx = c->mapLocX();
	int			my = c->mapLocY();

	int ncellsInRange = range + 1;

	int i = mx, j = my;
	for (int k = 0; k < ncellsInRange; k++) {
		switch (dir)
		{
		case td::direction::north: j++;	break;
		case td::direction::south: j--; break;
		case td::direction::east:  i++; break;
		case td::direction::west:  i--;	break;
		case td::direction::unset:
		default: break;
		}

		Cell* c = getCell(i, j);
		if (c) {
			if (c->isRoad() || c->type() == Cell::cell_type::exit)
				cellsInRange.push_back(c);
		}
	}
}

void Map::getRoadCellsAlongLine(const glm::vec2& p1, const glm::vec2& p2, std::vector<Cell*>& cellsInRange) {

	glm::vec2	dL		 = p2 - p1;	
	//If straight line, use other solution:
	if (glm::abs(dL.x) < CELL_SIZE / 1000.0f)
		return getRoadCellsInStraightLine(getCellAtLocation(p1), glm::sign(dL.y) ? td::direction::south : td::direction::north, glm::length(dL) / CELL_SIZE, cellsInRange);
	else if (glm::abs(dL.y) < CELL_SIZE / 1000.0f)
		return getRoadCellsInStraightLine(getCellAtLocation(p1), glm::sign(dL.x) ? td::direction::west : td::direction::east, glm::length(dL) / CELL_SIZE, cellsInRange);

	
	int			nsteps;
	float		slope;
	int			stepXsign = glm::sign(dL.x);
	int			stepYsign = glm::sign(dL.y);	
	float		local     = 0.0f;

	//lines moves more in X:
	bool line_is_more_in_X_dir = false;
	if (glm::abs(dL.x) > glm::abs(dL.y)){
		line_is_more_in_X_dir = true;
		nsteps = glm::abs(dL.x) / CELL_SIZE + 2;
		slope = std::fabs(dL.y / dL.x) * CELL_SIZE;
	}
	else {
		nsteps = glm::abs(dL.y) / CELL_SIZE + 2;
		slope = std::fabs(dL.x / dL.y) * CELL_SIZE;
	}
	

	Cell* previousCell = nullptr, *currentCell = nullptr;
	glm::vec2 currentCoord = p1;

	for (int i = 0; i < nsteps; i++) {
	
		currentCell = getCellAtLocation(currentCoord);
		if (currentCell != previousCell) {
			if (currentCell->isRoad() || currentCell->type() == Cell::cell_type::exit)
				cellsInRange.push_back(currentCell);
		}

		if(line_is_more_in_X_dir)
			currentCoord.x += (float)stepXsign * CELL_SIZE;
		else
			currentCoord.y += (float)stepYsign * CELL_SIZE;
		
		local += slope;
		if (local > CELL_SIZE) {

			if (line_is_more_in_X_dir)
				currentCoord.y += (float)stepYsign * CELL_SIZE;
			else
				currentCoord.x += (float)stepXsign * CELL_SIZE;
			local -= CELL_SIZE;
		}


	}
}

void Map::getRoadCellsInRange(Cell* c, float range, std::vector<Cell*>& cellsInRange) {

	int			mx = c->mapLocX();
	int			my = c->mapLocY();
	glm::vec2 tpos(mx, my);

	int dx = 1;
	int dy = (int)range;
	while (dy > 0) {

		bool inRange = true;
		while (inRange) {
			glm::vec2 cpos(mx + dx, my + dy);
			float d = glm::distance(tpos, cpos);

			if (d < (range + 1.0f)) {
				Cell* cq1 = getCell(mx + dx, my + dy);
				Cell* cq2 = getCell(mx + dx, my - dy);
				Cell* cq3 = getCell(mx - dx, my + dy);
				Cell* cq4 = getCell(mx - dx, my - dy);
				if (cq1 && (cq1->isRoad() || cq1->type() == Cell::cell_type::exit)) cellsInRange.push_back(cq1);
				if (cq2 && (cq2->isRoad() || cq2->type() == Cell::cell_type::exit)) cellsInRange.push_back(cq2);
				if (cq3 && (cq3->isRoad() || cq3->type() == Cell::cell_type::exit)) cellsInRange.push_back(cq3);
				if (cq4 && (cq4->isRoad() || cq4->type() == Cell::cell_type::exit)) cellsInRange.push_back(cq4);
			}
			else
				inRange = false;
			dx++;
		}
		dx = 1;
		dy--;
	}

	for (int i = 1; i < (range + 1); i++) {
		Cell* cN = getCell(mx + 0, my + i);
		Cell* cS = getCell(mx + 0, my - i);
		Cell* cE = getCell(mx + i, my + 0);
		Cell* cW = getCell(mx - i, my - 0);
		if (cN && (cN->isRoad() || cN->type() == Cell::cell_type::exit)) cellsInRange.push_back(cN);
		if (cS && (cS->isRoad() || cS->type() == Cell::cell_type::exit)) cellsInRange.push_back(cS);
		if (cE && (cE->isRoad() || cE->type() == Cell::cell_type::exit)) cellsInRange.push_back(cE);
		if (cW && (cW->isRoad() || cW->type() == Cell::cell_type::exit)) cellsInRange.push_back(cW);
	}


	/*
	[][][][][][]
	[][][][][][]
	[][][][][][]
	[][][][][][]
	*/

}