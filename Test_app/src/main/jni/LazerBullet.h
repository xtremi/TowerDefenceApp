#pragma once
#include "Bullet.h"

static const float LAZER_SPRITE_WIDTH = 4.0f;

class LazerBullet :public Bullet,  public TargetedBullet, public MultiTargetBullet
{
public:
	LazerBullet(const glm::vec2& pos, float _range, float _timer_max, int _shaderID);

	//virtual void setTarget(Mob* m);
	virtual void update(Map* map);
	bool isDone();
	void reset();
	void updateDirection(const glm::vec2& _dir = glm::vec2(1.0f, 0.0));	
	void checkCollisions(Map* map);
	void setTarget(Mob* m);
	void setLazerNotFollowTarget() { lazerNotFollowingTarget = true; }



private:
	std::vector<Cell*> cellsInSight;
	bool cellsInSightNeedUpdate		= true;
	bool lazerNotFollowingTarget	= false;
	
	glm::vec2 shootLine;
	float currentMobDistance = 0.0f;
	int	  shaderID	= 0;
	float timer		= 0.0f;
	float timer_max	= 0.0f;
	float range		= 0.0f;

};

