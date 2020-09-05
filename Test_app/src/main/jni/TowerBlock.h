#pragma once
#include "Tower.h"

class TowerBlock : public Tower {

public:
	TowerBlock(TowerDescriptor* _towerDescriptor);

protected:
	void setDirection(const glm::vec2& dir);
	bool canShoot();
};

