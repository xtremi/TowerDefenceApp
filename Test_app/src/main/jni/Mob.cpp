#include "Mob.h"

void Mob::update(direction dir, float offs) {
	
	currentDirection = dir;

	glm::vec2 pathVec = getDirectionAsVector(dir);
	glm::vec2 offsVec = getOrthogonalDirectionAsVector(dir);
	pathVec *= speed;
	offsVec *= offs;

	_pathPos += pathVec;
	glm::vec2 newPos = _pathPos + offsVec;
	setPosition(newPos.x, newPos.y);

	glm::vec2 moveVec = glm::normalize(pathVec);	
	float ang = agk::ATanFull(moveVec.x, moveVec.y);
	if (ang < 0.0f) ang += 360.0f;
		setRotation(ang + 90.0f);

	_travelledDistance += speed;
}

void Mob::setPosition(float x, float y) {
	Sprite::setPosition(x, y);
	lifebar->setPosition(x, y);
	debugText->setPosition(x, y);
	debugText->realign();
}
void Mob::setX(float x) {
	Sprite::setX(x);
	lifebar->setX(x);
	debugText->setX(x);
}
void Mob::setY(float y) {
	Sprite::setY(y);
	lifebar->setY(y);
	debugText->setY(y);
}
void Mob::setState(bool state) {
	Sprite::setState(state);
	lifebar->setState(state);
	debugText->setState(state);
}
void Mob::deleteSprite() {
	Sprite::deleteSprite();
	this->lifebar->deleteSprite();
	this->debugText->deleteSprite();
}


void Mob::die() {
	totalLife = 0.0f;
	_isDead = true;
}

float Mob::takeDamage(float dmg) {
	float damageTaken = 0.0f;
	
	float life = lifePercent * totalLife - dmg;
	if (life < 0.001f){
		damageTaken += life;
		die();		
	}
	else {
		lifePercent = life / totalLife;
		this->lifebar->setScale(glm::vec2(lifePercent, 1.0f));
		damageTaken = dmg;
	}
	return damageTaken;
}