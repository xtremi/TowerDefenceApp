#pragma once
#include "Bullet.h"

class FollowBullet : public Bullet, public TargetedBullet {
public:
	//FollowBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& dir, float _speed);
	FollowBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _speed, float _range);

	void setTarget(Mob* m);
	void update(Map* map);
	bool isDone();
	void reset();

private:
	glm::vec2 bulletOriginPos;
	float range = 0.0f;
	float currentSpeed = 0.0f;
	float maxSpeed = 0.0f;
	
	float direction_weight		= 0.0f;
	float max_direction_weight  = 0.2f;
	float dweight = 0.002f;

	int resetDirectionCounter = 0;
	int resetDirectionCounterMax = 5;

	

};

