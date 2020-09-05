#pragma once
#include "Cell.h"
#include <unordered_set>
#include <set>

class Cell;

struct probabilities {
	probabilities() {};
	probabilities(int s, int n, int e, int w) {
		go_south = s; go_north = s + n;
		go_east = s + n + e;  go_west = s + n + e  + w;
	}
	int go_south	= 0;
	int go_north	= 0;
	int go_east		= 0;
	int go_west		= 0;
};

static const int N_BORDER_CELL = 5;
class Map
{
public:
	Map();
	~Map();

	void createMap(const glm::vec2& pos, int _dimx, int _dimy, int _startX, int _startY);
	void hide();
	void show();

	
	Cell* getNextRoadCellOf(Cell* c);
	Cell* getPreviousCellOf(Cell* c);
	Cell* getNextRoadCellOf(int x, int y);
	Cell* getPreviousCellOf(int x, int y);

	Cell* getCellAtLocation(glm::vec2 loc);
	Cell* getCell(int x, int y);
	bool isInBounds(int x, int y);

	Cell* firstCellWithMobs();
	Cell* nextCellWithMobs();
	int numberOfCellsWithMobs();
	void clearCellsContainingMobs();
	void addCellContainingMobs(Cell* c);

	Cell* getMobEndCell();
	void setMobEndCellFromCell(int nBlocks, Cell* fromCell);
	void setMobEndCell(UINT x, UINT y);
	void setMobEndCell(Cell* c);
	Cell* getMobStartCell();
	void setMobStartcell(UINT x, UINT y);
	
	void setCellRowValidState(int row, bool state);
	void setCellRowsValidState(int rowFrom, int rowTo, bool state);

	std::vector<Cell*> getNeighbouringCells(int x, int y);
	bool buildingCanUpgradeToMega(int x, int y);

	void roadCellCount(Cell* c, int range, int& north, int& south, int& west, int& east);

	void getRoadCellsInStraightLine(Cell* c, td::direction dir, float range, std::vector<Cell*>& cellsInRange);
	void getRoadCellsInRange(Cell* c, float range, std::vector<Cell*>& cellsInRange);
	void getRoadCellsAlongLine(const glm::vec2& p1, const glm::vec2& p2, std::vector<Cell*>& cellsInRange);


private:
	std::vector<std::vector<Cell>> cells;
	std::set<Cell*> cellContainingMobs;
	std::set<Cell*>::iterator cellWithMobsIter;

	Cell* mobStartcell = NULL;
	Cell* mobEndcell = NULL;

	int dimx = 1;
	int dimy = 1;
	glm::vec2 mappos = glm::vec2(0.0f);

	std::map<td::direction, std::map<td::direction, probabilities>> probs;
	void setupProbabilities();

	void createDefaultCells();
	void createTrailCells();

	bool isNearWestBorder(int x);
	bool isNearEastBorder(int x);
	bool isNotOnWestBorder(int x);
	bool isNotOnEastBorder(int x);
	bool isOnWestBorder(int x);
	bool isOnEastBorder(int x);
	void setVisibilityState(bool state);

};


