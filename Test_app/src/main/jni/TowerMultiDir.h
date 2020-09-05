#pragma once
#include "Tower.h"

class TowerMultiDir : public Tower
{
public:
	TowerMultiDir(TowerDescriptor* _towerDescriptor);

protected:
	void setDirection(const glm::vec2& dir);
	bool canShoot();
	void shoot();
	void chooseTarget(Map* map);

	void initAOEdirections();
	void aim();

private:
	//std::vector<glm::vec2> aoeDirections;
	std::vector<float> aoeShootAngles;
	std::vector<Mob*> targets;
	std::vector<Cell*> cellTargets;
	//bool atLeastOneTargetSet = false;
	float currentRotation = 0.0f;

	//void findTargets();

};