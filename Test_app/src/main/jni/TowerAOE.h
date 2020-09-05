#pragma once
#include "Tower.h"

class TowerAOE : public Tower
{
public:
	TowerAOE(TowerDescriptor* _towerDescriptor);

	void init(Cell* cell);

protected:
	void setDirection(const glm::vec2& dir);
	bool canShoot();

	void aim();
	void chooseTarget(Map* map);
	
protected:
	//std::vector<Mob*> targetsInRange;
};

