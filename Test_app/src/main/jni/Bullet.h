#pragma once
#include "UI/Sprite.h"
#include "global.h"

#define DELTA_TIME 0.01f
#define BULLET_DEPTH 3

class Bullet : public Sprite {

public:
	Bullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& dir, float _speed) 
		: Sprite(img, pos, size) {
		direction = dir;
		setDepth(BULLET_DEPTH);
		speed = _speed;
	}
	Bullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size)
		: Sprite(img, pos, size)
	{
		setDepth(BULLET_DEPTH);
	}

	void setDirection(const glm::vec2& _dir, float _speed) {
		direction = _dir;
		speed = _speed;

		float ang = agk::ATanFull(direction.x, direction.y);
		if (ang < 0.0f) ang += 360.0f;
		setRotation(ang + 90.0f);
	}

	virtual void update() = 0;

	int getNhits() { return nHits; }
	void addHit() { nHits++; }
	void reset() {
		nHits = 0;
	}

protected:
	glm::vec2 direction;
	float speed = 1.0f;
	int nHits = 0;
};

class CircleBullet : public Bullet {

public:
	CircleBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& dir, float _speed) 
		: Bullet(img, pos, size, dir, _speed) {}

	CircleBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size)
		: Bullet(img, pos, size) {}

	void update() {
		move(direction * speed);
	}


};