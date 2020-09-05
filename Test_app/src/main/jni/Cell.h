#pragma once
#include "global.h"
#include "UI/Sprite.h"
#include "UI/UIButton.h"
#include "MediaBank.h"

#include "Mob.h"
#include "Building.h"

#include <vector>
#include <deque>
#include <algorithm>


#define CELL_DEPTH 25

class Building;


class Cell : public Sprite {

public:
	Cell(const glm::vec2& pos, int locx, int locy);
	bool operator<(const Cell &c) const { 
		return (roadID < c.roadID); 
	}

	enum class cell_type {
		grass,
		road,
		entrance,
		exit
	};

	void assignCellDirection(td::direction in, td::direction out);
	td::direction dirIn() { return dir_in; }
	td::direction dirout() { return dir_out; }

	bool hasTurn(); /*remove, use type() == road*/
	glm::vec2 posCenter();
	glm::vec2 posIn();

	glm::vec2 getLocalPosInCell(const glm::vec2& pos);
	bool positionIsPassedCellCenter(const glm::vec2& pos);
	bool positionIsPassedCellDiagonal(const glm::vec2& pos);

	float distanceFromPath(const glm::vec2& pos);

	void setBuildingRefOnly(Building* bld);
	void setBuilding(Building* bld);
	Building* getBuilding() { return building; }
	void deleteBuilding();
	void removeBuildingReference();

	Mob* firstMob();
	Mob* lastMob();
	Mob* nextMob();
	Mob* prevMob();
	std::vector<Mob*>* getMobs();
	int getNumberOfMobs();
	Mob* getMob(int i);
	void clearMobs();
	void addMob(Mob* m);
	bool isRoad();

	int mapLocX() { return maploc[0]; }
	int mapLocY() { return maploc[1]; }

	void setValid(bool state);
	bool valid() { return _valid; }
	cell_type type() { return _type; }
	void setType(cell_type ct);

	void setRoadID(int id) { roadID = id; }
	int getRoadID() const { return roadID; }

	Cell* getNextCell() { return nextCell; }
	Cell* getPrevCell() { return prevCell; }
	void setNextCell(Cell* c) { nextCell = c; }
	void setPrevCell(Cell* c) { prevCell = c; }

	td::direction getMoveDirection(const glm::vec2& pos);

private:
	int					maploc[2];
	int					roadID = -1;
	Building*			building = NULL;
	std::vector<Mob*>	mobs;
	int					mobsIterator = 0;
	td::direction		dir_in = td::direction::unset;
	td::direction		dir_out = td::direction::unset;
	bool				_valid = false;
	cell_type			_type = cell_type::grass;
	Cell*				nextCell = nullptr;
	Cell*				prevCell = nullptr;

	void setCellImage();
};


bool cellCompareRoadIDgreater(Cell* c1, Cell* c2);

Mob* findTargetAlongPath(Cell* cell, int cellsToCheck, td::path_direction dir);

class CellRange {
public:
	CellRange();

	void assignCells(const std::vector<Cell*>& cells, const glm::vec2& _cpos, float _realRange);
	
	Mob* findTargetInSector(const glm::vec2& dir, float sectorAngle);
	
	Cell* firstCellInRange();
	Cell* lastCellInRange();
	Cell* nextCellInRange();
	Cell* prevCellInRange();

	Mob* firstMobInRange(bool checkRealRange);
	Mob* nextMobInRange(bool checkRealRange);

	Mob* getFirstMobInRange(bool checkRealRange);
	Mob* getLastMobInRange(bool checkRealRange);
	int nMobsInRange(bool checkRealRange);
	bool mobIsInRange(Mob* m);

	bool checkIfHasMobInRange();

	Cell* getCellInRange(int i);
	int getNumberOfCellsInRange();

	


private:
	std::vector<Cell*> allCells;
	int cellInRangeIterator = 0;
	Cell* cellIt;
	Mob* mobIt;
	int ncells = 0;
	float realRange = 0.0f;
	glm::vec2 cpos;
};


class QuadrantCellRange : public CellRange
{
public:
	QuadrantCellRange();

	void setQuadrantCells(const glm::vec2& cpos);

private:
	std::vector<Cell*> qNW;
	std::vector<Cell*> qNE;
	std::vector<Cell*> qSW;
	std::vector<Cell*> qSE;
	std::vector<Cell*> N;
	std::vector<Cell*> S;
	std::vector<Cell*> W;
	std::vector<Cell*> E;
	
};


/*class CellCmpr {
public:
	bool operator()(const Cell* lhs, const Cell* rhs) {
		return (lhs->getRoadID() < rhs->getRoadID());
	}
};*/