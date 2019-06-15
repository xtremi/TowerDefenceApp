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
	std::map<direction, probabilities> pFromNorth;
	pFromNorth[direction::north]	= probabilities(0, 0, 0, 0);	
	pFromNorth[direction::south]	= probabilities(0, 60, 20, 20);
	pFromNorth[direction::east]		= probabilities(0, 30, 70, 0);
	pFromNorth[direction::west]		= probabilities(0, 30, 0, 70);

	std::map<direction, probabilities> pFromSouth;
	pFromSouth[direction::north] = probabilities(0, 20, 40, 40);
	pFromSouth[direction::south] = probabilities(0, 0, 0, 0);
	pFromSouth[direction::east] = probabilities(0, 30, 70, 0);
	pFromSouth[direction::west] = probabilities(0, 30, 0, 70);

	std::map<direction, probabilities> pFromEast;
	pFromEast[direction::north] = probabilities(0, 60, 20, 20);
	pFromEast[direction::south] = probabilities(60, 0, 20, 20);
	pFromEast[direction::east] = probabilities(0, 0, 0, 0);
	pFromEast[direction::west] = probabilities(0, 5, 0, 95); //.

	std::map<direction, probabilities> pFromWest;
	pFromWest[direction::north] = probabilities(0, 60, 20, 20);
	pFromWest[direction::south] = probabilities(60, 0, 20, 20);
	pFromWest[direction::east] = probabilities(0, 5, 95, 0); //.
	pFromWest[direction::west] = probabilities(0, 0, 0, 0);

	probs[direction::north] = pFromNorth;
	probs[direction::south] = pFromSouth;
	probs[direction::east] = pFromEast;
	probs[direction::west] = pFromWest;
}



void incrementCellIndex(direction dirout, int& x, int& y) {
	switch (dirout){
		case direction::north:
			y++; break;
		case direction::south:
			y--; break;
		case direction::east:
			x++; break;
		case direction::west:
			x--; break;
		case direction::unset:
		default:
			break;
	}
}

void Map::createTrailCells()
{
	mobStartcell->assignCellDirection(direction::south, direction::north);
	mobStartcell->setRoadID(0);

	Cell* previousCell = mobStartcell;

	int currentX = mobStartcell->mapLocX();
	int currentY = mobStartcell->mapLocY();
	int counter = 0;

	while (currentY < (dimy - 1)) {
		counter++;

		direction newDirIn, newDirOut;
		newDirIn = oppositeDirectionOf(previousCell->dirout());
		incrementCellIndex(previousCell->dirout(), currentX, currentY);

		probabilities* p = &probs[previousCell->dirIn()][previousCell->dirout()];
		bool validMove = false;
		while(!validMove){
			float r = agk::Random(1, 100);
						
			if (r <= p->go_south)
				newDirOut = direction::south;
			else if (r <= p->go_north)
				newDirOut = direction::north;
			else if (r <= p->go_east)
				newDirOut = direction::east;				
			else
				newDirOut = direction::west;
			
			if ((isNearEastBorder(currentX) && newDirOut == direction::east) ||
				(isNearWestBorder(currentX) && newDirOut == direction::west))
				validMove = false;
			else if (previousCell->dirIn() == direction::east && newDirOut == direction::east)
				validMove = false;
			else if (previousCell->dirIn() == direction::west && newDirOut == direction::west)
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
	if (x < 0 || x >= dimx || y < 0 || y >= dimy)
		return false;
	return true;
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

Cell* Map::getNextRoadCellOf(Cell* c) {
	if (!c)
		return NULL;

	direction dirOut = c->dirout();
	int x = c->mapLocX();
	int y = c->mapLocY();
	incrementCellIndex(dirOut, x, y);
	return getCell(x, y);
}
Cell* Map::getNextRoadCellOf(int x, int y) {
	return getNextRoadCellOf(getCell(x, y));
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