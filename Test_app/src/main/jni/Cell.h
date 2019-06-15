#pragma once
#include "global.h"
#include "UI/Sprite.h"
#include "UI/UIButton.h"
#include "MediaBank.h"

#include "Mob.h"
#include "Building.h"

#include <vector>
#include <deque>

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

	void assignCellDirection(direction in, direction out);
	direction dirIn() { return dir_in; }
	direction dirout() { return dir_out; }

	bool hasTurn(); /*remove, use type() == road*/
	glm::vec2 posCenter();
	glm::vec2 posIn();

	glm::vec2 getLocalPosInCell(const glm::vec2& pos);
	bool positionIsPassedCellCenter(const glm::vec2& pos);
	bool positionIsPassedCellDiagonal(const glm::vec2& pos);

	void setBuildingRefOnly(Building* bld);
	void setBuilding(Building* bld);
	Building* getBuilding() { return building; }
	void deleteBuilding();

	Mob* firstMob();
	Mob* nextMob();
	std::vector<Mob*>* getMobs();
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

private:
	int maploc[2];
	int roadID = -1;

	direction dir_in = direction::unset;
	direction dir_out = direction::unset;
	bool _valid = false;
	cell_type _type = cell_type::grass;

	Building* building = NULL;
	std::vector<Mob*> mobs;
	int mobsIterator = 0;

	void setCellImage();
};


bool cellCompareRoadIDgreater(Cell* c1, Cell* c2);

/*class CellCmpr {
public:
	bool operator()(const Cell* lhs, const Cell* rhs) {
		return (lhs->getRoadID() < rhs->getRoadID());
	}
};*/