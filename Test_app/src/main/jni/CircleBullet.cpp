#include "CircleBullet.h"

CircleBullet::CircleBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& dir, float _speed)
	: Bullet(img, pos, size, dir, _speed) {}

CircleBullet::CircleBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size)
	: Bullet(img, pos, size) {}

void CircleBullet::update(Map* map) {
	move(direction * speed);
}

bool CircleBullet::isDone() { return nHits > 0; }