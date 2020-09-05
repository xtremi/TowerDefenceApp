#pragma once
#include "UI/SpriteAndText.h"
#include "Map.h"
#include "global.h"
class Map;
class Cell;
class CellRange;

enum class building_size_upgrade {
	normal,
	mega
};

class Building
{
public:
	Building();
	~Building() {}

	virtual void init(Cell* cell);

	virtual void update(Map* map)		= 0;
	virtual bool isTower()		= 0;
	virtual int getBuildingID() = 0;
	virtual void deleteBuilding();
	virtual void setDirection(const glm::vec2& dir);
	virtual float rangeCells() = 0;
	virtual float rangeDistance() = 0;
	virtual float value() = 0;
	virtual float price() = 0;

	void hideDebugSprites();
	void showDebugSprites();

	void megafy();
	void setPosition(const glm::vec2& pos, int locx, int locy);
	//void setCellsInRange(std::vector<Cell*> cells);
	void setCellsInRange(CellRange* _cellRange);
	int getLocX() { return maploc[0]; }
	int getLocY() { return maploc[1]; }
	void debug_setupHighlightCellsInRange();
	glm::vec2 getPosition() { return cpos; }
	
	building_size_upgrade buildingSizeUpgrade() { return _buildingSizeUpgrade; }
	glm::vec2 getDirection() { return orientation; }
	
	void face(td::direction dir);
//	Cell* firstCellInRange();
//	Cell* lastCellInRange();
//	Cell* nextCellInRange();
//	Cell* prevCellInRange();
//
//	Cell* getCellInRange(int i);
//	int getNumberOfCellsInRange();
	

protected:
//	std::vector<Cell*>		cellsInRange;
	CellRange*				cellRange;
	building_size_upgrade	_buildingSizeUpgrade;

	glm::vec2	cpos;
	glm::vec2	orientation;
	int			maploc[2];	
	bool		debugSpriteON = false;

	std::vector<AgkSprite*>		staticSprites;
	std::vector<AgkSprite*>		rotSprites;
	std::vector<AgkSprite*>		debug_cellsInRange;
	std::vector<SpriteAndText*> debug_buildingStats;

	//int cellInRangeIterator = 0;

	virtual void loadBuildingSprites();
	virtual void loadStaticSprites() = 0;
	virtual void loadRotatableSprites(){}
	virtual void initDebugTextSprites(){}
	virtual void updateDebugTextSprites(){}
	
	virtual void postSetCellsInRange(){}

	void deleteStaticSprites();	
	void deleteRotatableSprites(bool clearOnly);
	void deleteDebugSprites();
	void deleteDebugTextSprites();
	void deleteAllDebugSprites();
};