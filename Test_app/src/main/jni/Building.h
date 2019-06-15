#pragma once
#include "UI/SpriteAndText.h"
#include "Cell.h"
#include "global.h"


class Cell;


enum class building_size_upgrade {
	normal,
	mega
};

class Building
{
public:
	Building() { 
		orientation = glm::vec2(0.0f, 1.0f);
		_buildingSizeUpgrade = building_size_upgrade::normal;
	}
	~Building() {}

	virtual void process() = 0;
	virtual bool isTower() = 0;
	virtual int getBuildingID() = 0;
	virtual void deleteBuilding();

	building_size_upgrade buildingSizeUpgrade() { return _buildingSizeUpgrade; }
	void megafy();

	void setPosition(const glm::vec2& pos, int locx, int locy);
	glm::vec2 getPosition() { return cpos; }

	void setCellsInRange(std::vector<Cell*> cells);
	Cell* firstCellInRange();
	Cell* nextCellInRange();

	void debug_setupHighlightCellsInRange();

	int getLocX() { return maploc[0]; }
	int getLocY() { return maploc[1]; }
	float getRange() { return range; }

	glm::vec2 getDirection() {
		return orientation;
	}
	virtual void setDirection(const glm::vec2& dir);
	void face(direction dir);


protected:
	std::vector<Cell*>	cellsInRange;
	int cellInRangeIterator = 0;

	std::vector<AgkSprite*>		sprites;
	std::vector<AgkSprite*>		debug_cellsInRange;
	std::vector<SpriteAndText*> debug_buildingStats;
	
	glm::vec2 cpos;
	int maploc[2];

	glm::vec2 orientation;

	virtual void loadBuildingSprites() = 0;
	virtual void deleteBuildingSprites();
	virtual void deleteDebugSprites();
	virtual void deleteDebugTextSprites();
	virtual void deleteAllDebugSprites();

protected:
	float range = 0.0f;
	float rangeMultiplier = 1.0f;
	building_size_upgrade _buildingSizeUpgrade;

	//bool _isUpgraded;
	//bool _isSold;


};