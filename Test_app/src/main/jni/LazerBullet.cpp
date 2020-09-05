#include "LazerBullet.h"

LazerBullet::LazerBullet(const glm::vec2& pos, float _range, float _timer_max, int _shaderID)
	: Bullet(GETIMG(ASSET::IMG_BLANK_128x128), pos, glm::vec2(_range, LAZER_SPRITE_WIDTH)), MultiTargetBullet()
{	
	range		= _range;
	timer_max	= _timer_max;
	shaderID	= _shaderID;
	
	agk::SetSpriteShader(id, shaderID);
	setAlignment(AgkHAlign::left, AgkVAlign::center);
}

void LazerBullet::update(Map* map){
	timer += DELTA_TIME;
	newTargets.clear();
	releasedTargets.clear();
	if(!lazerNotFollowingTarget)
		updateOrientation();
	checkCollisions(map);
}

void LazerBullet::setTarget(Mob* m) {
	TargetedBullet::setTarget(m);	
	if (lazerNotFollowingTarget)
		updateOrientation();
}

//static std::vector<Cell*> tmpcells;
void LazerBullet::checkCollisions(Map* map) {
	std::vector<Cell*> cellsInSight;	
	glm::vec2 endpos = shootLine + getPos();
	map->getRoadCellsAlongLine(getPos(), endpos, cellsInSight);
	
	/*for (Cell* ctmp : tmpcells)
		ctmp->setColor(colours::white);	
	
	for (Cell* c : cellsInSight)
		c->setColor(colours::red);
	tmpcells = cellsInSight;*/

	std::set<Mob*> currentTargets;

	for (Cell* c : cellsInSight) {
		for (Mob* m = c->firstMob(); m; m = c->nextMob()) {
			if(m->isAlive()){
				if (agk::GetSpriteCollision(getID(), m->getID())) {
					if(allTargets.find(m) == allTargets.end()){
						newTargets.insert(m);
						allTargets.insert(m);
					}
					currentTargets.insert(m);
				}
			}
		}
	}

	for (auto it = allTargets.begin(); it != allTargets.end();) {
		if (currentTargets.find(*it) == currentTargets.end()){
			releasedTargets.insert(*it);
			it = allTargets.erase(it);
		}
		else
			it++;
	}

}

//TODO: check if this could be done Tower::updateBullet method?
//because it updates the orientation of normal bullets, this should be similar maybe...
void LazerBullet::updateOrientation() {
	shootLine = targetMob->getPos() - getPos();
	direction = glm::normalize(shootLine);
	shootLine = direction * range;
	//setSize(glm::vec2(range, CHAIN_SPRITE_WIDTH));	
	float ang = agk::ATanFull(shootLine.x, shootLine.y);
	setRotation(ang - 90.0f);
}

bool LazerBullet::isDone(){
	if(!lazerNotFollowingTarget){
		if (!targetMob || (targetMob && targetMob->isDead()))
			return true;
		//if out of range
	}
	
	return timer > timer_max;
}
void LazerBullet::reset(){
	timer = 0.0f;
	targetMob = nullptr;
	
	allTargets.clear();
	newTargets.clear();
	releasedTargets.clear();
}