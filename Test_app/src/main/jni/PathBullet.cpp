#include "PathBullet.h"

const float PathBullet::CELL_CENTER_DISTANCE_TOLERANCE = CELL_SIZE * 0.1f;
const float PathBullet::MIN_SPEED = 0.15f;

PathBullet::PathBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _speed)
	: CircleBullet(img, pos, size, glm::vec2(0.0f),  _speed)
{
	maxHits = 10;	
	originalSpeed = speed;
}

void PathBullet::setTarget(Cell* c){
	CellTargetedBullet::setTarget(c);
	totalDistance = targetCell->distanceFromPath(getPos());	
}

void PathBullet::reset() {
	CircleBullet::reset();
	currentCell = nullptr;
}

void PathBullet::update(Map* map) {

	if (!currentCell)
		updateBeforeReachPath();
	else
		updateWhileOnPath(map);
	CircleBullet::update(map);
}

void PathBullet::updateBeforeReachPath() {
	float distToCellPath = targetCell->distanceFromPath(getPos());

	if (distToCellPath < CELL_CENTER_DISTANCE_TOLERANCE) {
		currentCell = targetCell;
		glm::vec2 cpos = currentCell->getPos() + glm::vec2(CELL_SIZE / 2.0f);
		if (isNorthSouth(targetCell->dirIn()))
			setX(cpos.x);
		else
			setY(cpos.y);
	}
	else {
		if (speed > MIN_SPEED) {
			float ratio = std::sqrt(distToCellPath / totalDistance);
			speed = ratio * originalSpeed;
			if (speed < MIN_SPEED) speed = MIN_SPEED;
		}
	}
}

void PathBullet::updateWhileOnPath(Map* map) {
	td::direction dir = currentCell->getMoveDirection(getPos());
	dir = oppositeDirectionOf(dir);
	direction = getDirectionAsVector(dir) * speed;

	currentCell = map->getCellAtLocation(getPos());

	if (speed < originalSpeed) {
		speed *= 1.05f;
	}
}





void PathBullet::checkIfOnRoad(Map* map) {
	if (Cell* c = map->getCellAtLocation(getPos())) {
		if (c->isRoad()) {
			currentCell = c;
		}
	}
}

bool PathBullet::isDone() {
	return (nHits >= maxHits);
}