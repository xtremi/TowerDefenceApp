#pragma once
#include "UI/Sprite.h"
#include "global.h"
#include <set>
#include <string>
#include "Map.h"

#define BULLET_SPRITE_DEPTH		 8

class Bullet : public Sprite {

public:
	Bullet();
	Bullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& dir, float _speed);
	Bullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size);
	Bullet(const glm::vec4& col, const glm::vec2& pos, const glm::vec2& size);
	Bullet(const std::string& animID, const glm::vec2& pos, const glm::vec2& size);

	void setDirection(float ang, float _speed);
	void setDirection(const glm::vec2& _dir, float _speed);
	void updateDirection(const glm::vec2& _dir);

	virtual void update(Map* map) = 0;
	virtual bool isDone() = 0;	
	virtual void reset();

	void addHit();
protected:
	glm::vec2 direction;
	float speed = 1.0f;
	int nHits = 0;
};

class TargetedBullet {
public:
	TargetedBullet() {}

	Mob* getTarget() { return targetMob; }
	virtual void setTarget(Mob* m) { targetMob = m; }

protected:
	Mob* targetMob = nullptr;
};

class CellTargetedBullet {
public:
	CellTargetedBullet() {}

	Cell* getTarget() { return targetCell; }
	virtual void setTarget(Cell* c) { targetCell = c; }

protected:
	Cell* targetCell = nullptr;
};

Mob* findNewTargetAroundLocation(Map* map, const glm::vec2& loc, float rad);
//Mob* findNewTargetAlongPath(Map* map, Mob* targetMob, int cellsToCheck);
//Mob* findNewTargetAlongLine(Map* map, std::vector<Cell*>* cells, const glm::vec2& origin, const glm::vec2& dir);
