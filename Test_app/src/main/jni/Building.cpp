#include "Building.h"

Building::Building() {
	orientation = glm::vec2(0.0f, 1.0f);
	_buildingSizeUpgrade = building_size_upgrade::normal;	
}

void Building::init(Cell* cell) {	
	loadBuildingSprites();
	initDebugTextSprites();
	cell->setBuilding(this);
}

void Building::loadBuildingSprites() {
	loadStaticSprites();
	loadRotatableSprites();
}

void Building::hideDebugSprites() {
	debugSpriteON = false;
	for (SpriteAndText* ts : debug_buildingStats)
		ts->hide();
	for (AgkSprite* s : debug_cellsInRange)
		s->hide();
}
void Building::showDebugSprites() {
	debugSpriteON = true;
	for (SpriteAndText* ts : debug_buildingStats)
		ts->show();
	for (AgkSprite* s : debug_cellsInRange)
		s->show();
}

void Building::setPosition(const glm::vec2& pos, int locx, int locy) {
	cpos = pos;
	maploc[0] = locx;
	maploc[1] = locy;
	for (AgkSprite* s : staticSprites)
		s->setPosition(pos);
	for (SpriteAndText* ts : debug_buildingStats)
		ts->setPosition(pos);
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

void Building::deleteStaticSprites() {
	for (AgkSprite* s : staticSprites) {
		s->deleteSprite();
		delete s;
	}
	staticSprites.clear();
}

void Building::deleteRotatableSprites(bool clearOnly) {
	if(!clearOnly){
		for (AgkSprite* s : rotSprites) {
			s->deleteSprite();
			delete s;
		}
	}
	rotSprites.clear();
}

void Building::deleteBuilding() {
	deleteAllDebugSprites();
	deleteStaticSprites();
	deleteRotatableSprites(true);
}

void Building::setCellsInRange(CellRange* _cellRange) {
	cellRange = _cellRange;
	debug_setupHighlightCellsInRange();
	postSetCellsInRange();
}


void Building::debug_setupHighlightCellsInRange() {
	
	deleteDebugSprites();
	
	for (Cell* c = cellRange->firstCellInRange(); c; c = cellRange->nextCellInRange()) {
		Sprite* s = new Sprite(glm::vec3(1, 0, 0), c->posCenter(), glm::vec2(CELL_SIZE));
		s->setColorAlpha(0.15f);
		debug_cellsInRange.push_back(s);
		if (!debugSpriteON) s->hide();
	}
}


void Building::setDirection(const glm::vec2& dir) {
	orientation = dir;
	
	float ang = agk::ATan2(dir.x, -dir.y);
	for (AgkSprite* s : staticSprites)
		s->setRotation(ang);	
}

void Building::face(td::direction dir) {
	setDirection(getDirectionAsVector(dir));
}

void Building::megafy() {
	for (AgkSprite* s : staticSprites) {
		s->setSize(glm::vec2(CELL_SIZE * 3.0f));
	}
	_buildingSizeUpgrade = building_size_upgrade::mega;
}