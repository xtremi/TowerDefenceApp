#include "TowerAim.h"

TowerAim::TowerAim(TowerDescriptor* _towerDescriptor)
	: Tower(_towerDescriptor) 
{
	currentAimAngle = 0.0f;
	if (towerDescriptor->bulletStats.type == bullet_type::chain) {
		targetSelectionType = target_selection_type::last;
	}
}


bool TowerAim::canShoot() {
	return Tower::canShoot() && isInSight;
}

void TowerAim::chooseTarget(Map* map) {

	if (currentTarget) {
		if (currentTarget->isAlive() && cellRange->mobIsInRange(currentTarget))
			return;
	}

	if (targetSelectionType == target_selection_type::first)
		currentTarget = cellRange->getFirstMobInRange(true);
	else if (targetSelectionType == target_selection_type::last)
		currentTarget = cellRange->getLastMobInRange(true);

	if (currentTarget) {		
		isStationary = false;
	}

	if (towerDescriptor->bulletStats.type == bullet_type::path && currentTarget)
		currentCellTarget = map->getCellAtLocation(currentTarget->getPos());
}

float TowerAim::getTargetAngle() {
	glm::vec2 targetDir;
	if (currentTarget) {
		targetDir = currentTarget->getPos() - cpos;
		targetDir = glm::normalize(targetDir);
	}
	else
		targetDir = orientation;

	float targetAngle = agk::ATanFull(targetDir.x, targetDir.y);
	if (targetAngle < 0.0f) targetAngle += 360.0f;
	return targetAngle;
}

float TowerAim::processAimAngle(float targetAngle) {
	if (currentAimAngle < 0.0f)
		currentAimAngle += 360.0f;

	float dAngle = targetAngle - currentAimAngle;
	if (dAngle < 0.0f) dAngle += 360.0f;

	float stepSize = towerDescriptor->rotate_speed;
	if (stepSize > dAngle)
		stepSize = dAngle;

	if (dAngle < 180.0f)
		currentAimAngle += stepSize;
	else
		currentAimAngle -= stepSize;

	return dAngle;
}

void TowerAim::aim() {

	if (isStationary)
		return;

	float targetAngle = getTargetAngle();
	float dAngle = processAimAngle(targetAngle);

	if (dAngle < 2.0f || dAngle > 358.0f) {
		currentAimAngle = targetAngle;
		if (currentTarget)
			isInSight = true;
		else{
			isStationary = true;
			isInSight = false;
		}
	}
	else {
		isInSight = false;
		isStationary = false;
	}

	shootDirection = glm::vec2(agk::Cos(currentAimAngle - 90.0f), agk::Sin(currentAimAngle - 90.0f));

	for (AgkSprite* s : rotSprites) {
		s->setRotation(currentAimAngle);
	}


	if (towerDescriptor->bulletStats.type == bullet_type::chain){
		if (Bullet* b = firstBullet())
			b->setPosition(this->cpos + shootDirection * 10.0f);
	}

}


void TowerAim::onTowerKillMob(Mob* mob) {
	Tower::onTowerKillMob(mob);
	if (mob == currentTarget)
		currentTarget = NULL;
}

void TowerAim::setDirection(const glm::vec2& dir)
{
	orientation = dir;
	isStationary = false;
}