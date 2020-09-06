#include "Bullet.h"

Bullet::Bullet() {}

Bullet::Bullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, const glm::vec2& dir, float _speed)
	: Sprite(img, pos, size) 
{
	direction = dir;
	speed = _speed;
	setDepth(BULLET_SPRITE_DEPTH);
	hide();
}

Bullet::Bullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size)
	: Sprite(img, pos, size)
{
	setDepth(BULLET_SPRITE_DEPTH);
	hide();
}

Bullet::Bullet(const glm::vec4& col, const glm::vec2& pos, const glm::vec2& size)
	: Sprite(col, pos, size)
{
	setDepth(BULLET_SPRITE_DEPTH);
	hide();
}

Bullet::Bullet(const std::string& animID, const glm::vec2& pos, const glm::vec2& size)
	: Sprite(MEDIA_BANK.getAnimationSprite(animID))
{
	setSize(size);
	setAlignment(AgkHAlign::center, AgkVAlign::center);
	setPosition(pos);	
	setDepth(BULLET_SPRITE_DEPTH);
	hide();
}



void Bullet::setDirection(float ang, float _speed) {
	direction = glm::vec2(agk::Cos(ang - 90.0f), agk::Sin(ang - 90.0f));
	speed = _speed;
	setRotation(ang + 90.0f);
}

void Bullet::setDirection(const glm::vec2& _dir, float _speed) {
	speed = _speed;
	updateDirection(_dir);
}

void Bullet::updateDirection(const glm::vec2& _dir) {
	direction = _dir;
	float ang = agk::ATanFull(direction.x, direction.y);
	//if (ang < 0.0f) ang += 360.0f;
	setRotation(ang - 90.0f);
}


void Bullet::addHit() { nHits++; }
void Bullet::reset() {
	nHits = 0;
}


std::set<Mob*>* MultiTargetBullet::getCurrentTargets() {
	return &allTargets;
}
std::set<Mob*>* MultiTargetBullet::getNewTargets() {
	return &newTargets;
}
std::set<Mob*>* MultiTargetBullet::getReleasedTargets() {
	return &releasedTargets;
}

/*
	Check the cells in radius <rad> around location <loc> for roadcells.
	Then sets the first mob found in these roadcells as target.
	(First found should be first mob on path)
*/
Mob* findNewTargetAroundLocation(Map* map, const glm::vec2& loc, float rad)
{
	std::vector<Cell*> roadCells;
	map->getRoadCellsInRange(map->getCellAtLocation(loc), rad / CELL_SIZE, roadCells);

	for (Cell* c : roadCells) {
		if (Mob* m = c->firstMob()) {
			return m;
		}
	}
	return nullptr;
}

/*

*/
