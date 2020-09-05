#pragma once
#include "Tower.h"



class TowerAim : public Tower 
{
public:
	TowerAim(TowerDescriptor* _towerDescriptor);
	
protected:
	void setDirection(const glm::vec2& dir);
	bool canShoot();
	void onTowerKillMob(Mob* mob);
	
	void aim();
	void chooseTarget(Map* map);
	float getTargetAngle();
	float processAimAngle(float targetAngle);
	
	bool isInSight			= false;			//The tower has a mob in sight, and can shoot at it.
	bool isStationary		= false;		//The tower has no mob in range, and has reached its "resting" direction (orientation)
	float currentAimAngle	= 0.0f;
	//Mob* currentTarget		= NULL;
	

};

