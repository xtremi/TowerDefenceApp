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
	
	for (int i = 0; i < debug_cellsInRange.size(); i++) {
		debug_cellsInRange[i]->deleteSprite();
		delete debug_cellsInRange[i];
	}
	debug_cellsInRange.clear();
	
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