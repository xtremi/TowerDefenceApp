#pragma once
#include "CircleBullet.h"


class PathBullet : public CircleBullet, public CellTargetedBullet
{
public:
	PathBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _speed);

	virtual void update(Map* map);
	virtual bool isDone();
	void reset();

	void setTarget(Cell* c);

private:
	Cell* currentCell = nullptr;
	float originalSpeed = 0.0f;
	float totalDistance = 0.0f;
	void checkIfOnRoad(Map* map);

	void updateBeforeReachPath();
	void updateWhileOnPath(Map* map);

	int maxHits = 10;

	static const float CELL_CENTER_DISTANCE_TOLERANCE;
	static const float MIN_SPEED;

};

