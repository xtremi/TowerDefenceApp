#include "FollowBullet.h"

FollowBullet::FollowBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _speed, float _range)
	: Bullet(img, pos, size) 
{
	speed = _speed;
	maxSpeed = 3.0f*speed;
	range = _range;
	bulletOriginPos = pos;
}

void FollowBullet::setTarget(Mob* m) {
	TargetedBullet::setTarget(m);
	reset();
}

void FollowBullet::reset() {
	Bullet::reset();
	resetDirectionCounter = 0;
	direction_weight = 0.0f;
	currentSpeed = speed;
}



void FollowBullet::update(Map* map) {
	resetDirectionCounter++;

	if (resetDirectionCounter < resetDirectionCounterMax)
	{ 
		/*if (findOwnTarget) {
			glm::vec2 locationToCheck = getPos() + 0.7f * range * direction;
			findNewTargetAroundLocation(map, locationToCheck, range*0.4f);
			findOwnTarget = false;
		}*/

		if (targetMob && !targetMob->isDead()) {
			glm::vec2 newDirection = targetMob->getPos() - getPos();
			newDirection = glm::normalize(newDirection);

			if(direction_weight < max_direction_weight)
				direction_weight += dweight;

			newDirection = direction_weight*newDirection + (1.0f - direction_weight) * direction;
			newDirection = glm::normalize(newDirection);
			updateDirection(newDirection);
			resetDirectionCounter = 0;

			float d = glm::distance(targetMob->getPos(), bulletOriginPos);
			if (d > range)
				targetMob = findTargetAlongPath(map->getCellAtLocation(targetMob->getPos()), 5, td::path_direction::any);
		}
	}
	
	if(currentSpeed < maxSpeed)
		currentSpeed *= 1.02f;
	move(direction * currentSpeed);
}

bool FollowBullet::isDone() { 
	return nHits > 0; 
}