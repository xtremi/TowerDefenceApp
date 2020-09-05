#pragma once
#include "Bullet.h"

static const float LAZER_SPRITE_WIDTH = 4.0f;

class LazerBullet :public Bullet,  public TargetedBullet
{
public:
	LazerBullet(const glm::vec2& pos, float _range, float _timer_max, int _shaderID);

	//virtual void setTarget(Mob* m);
	virtual void update(Map* map);
	bool isDone();
	void reset();
	void updateOrientation();


private:
	int	  shaderID	= 0;
	float timer		= 0.0f;
	float timer_max	= 0.0f;
	float range		= 0.0f;
};

