#pragma once
#include "Tower.h"

class TowerAimless : public Tower {

public:
	TowerAimless(TowerDescriptor* _towerDescriptor);
	void	init(Cell* cell);


protected:
	void setDirection(const glm::vec2& dir);
	bool canShoot();
	void postSetCellsInRange();
	void chooseTarget(Map* map);
	//void onTowerKillMob(Mob* mob);
};

