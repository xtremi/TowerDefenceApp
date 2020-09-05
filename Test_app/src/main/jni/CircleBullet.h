#pragma once
#include "Bullet.h"

class CircleBullet : public Bullet {

public:
	CircleBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& dir, float _speed);
	CircleBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size);

	void update(Map* map);
	bool isDone();
};