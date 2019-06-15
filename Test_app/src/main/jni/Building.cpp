#include "Building.h"


void Building::setPosition(const glm::vec2& pos, int locx, int locy) {
	cpos = pos;
	maploc[0] = locx;
	maploc[1] = locy;
	for (AgkSprite* s : sprites)
		s->setPosition(pos);
	for (SpriteAndText* ts : debug_buildingStats)
		ts->setPosition(pos);
}

void Building::deleteBuildingSprites() {
	for (AgkSprite* s : sprites) {
		s->deleteSprite();
		delete s;
	}
	sprites.clear();
}

void Building::deleteDebugTextSprites() {
	for (AgkSprite* s : debug_buildingStats) {
		s->deleteSprite();
		delete s;
	}
	debug_buildingStats.clear();
}
void Building::deleteDebugSprites() {
	for (AgkSprite* s : debug_cellsInRange) {
		s->deleteSprite();
		delete s;
	}
	debug_cellsInRange.clear();
}
void Building::deleteAllDebugSprites() {
	deleteDebugTextSprites();
	deleteDebugSprites();
}

void Building::deleteBuilding() {
	deleteAllDebugSprites();
	deleteBuildingSprites();
}

void Building::setCellsInRange(std::vector<Cell*> cells) {
	cellsInRange = cells;
	debug_setupHighlightCellsInRange();
}
Cell* Building::firstCellInRange() {
	cellInRangeIterator = 0;
	return nextCellInRange();
}
Cell* Building::nextCellInRange() {
	if (cellInRangeIterator < cellsInRange.size())
		return cellsInRange[cellInRangeIterator++];
	return NULL;
}

void Building::debug_setupHighlightCellsInRange() {
	
	deleteDebugSprites();
	
	for (Cell* c : cellsInRange) {
		Sprite* s = new Sprite(glm::vec3(1, 0, 0), c->posCenter(), glm::vec2(CELL_SIZE));
		s->setColorAlpha(0.05f);
		debug_cellsInRange.push_back(s);
	}
}


void Building::setDirection(const glm::vec2& dir) {
	orientation = dir;
	
	float ang = agk::ATan2(dir.x, -dir.y);
	for (AgkSprite* s : sprites)
		s->setRotation(ang);	
}

void Building::face(direction dir) {
	setDirection(getDirectionAsVector(dir));
}

void Building::megafy() {
	for (AgkSprite* s : sprites) {
		s->setSize(glm::vec2(CELL_SIZE * 3.0f));
	}
	_buildingSizeUpgrade = building_size_upgrade::mega;
}