#include "AreaSpawnBullet.h"


AreaSpawnBullet::AreaSpawnBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max)
	: Bullet(img, pos, size)
{
	timer_max = _timer_max;
}

AreaSpawnBullet::AreaSpawnBullet(const std::string& animID, const glm::vec2& pos, const glm::vec2& size, float _timer_max)
	: Bullet(animID, pos, size)
{
	timer_max = _timer_max;
}

void AreaSpawnBullet::update(Map* map) {
	timer += DELTA_TIME;
}
bool AreaSpawnBullet::isDone() {
	return timer > timer_max;
}
void AreaSpawnBullet::reset() {
	timer = 0.0f;
}

/************************************************************
	Expanding
************************************************************/
AreaSpawnBullet_expanding::AreaSpawnBullet_expanding(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max)
	: AreaSpawnBullet(img, pos, size, _timer_max)
{
	maxRadius = size.x;
	expandSpeed = (maxRadius - minRadius) / timer_max;
}

void AreaSpawnBullet_expanding::update(Map* map) {
	AreaSpawnBullet::update(map);
	//float expFactor = std::pow(timer / timer_max, 2.0);
	//setSize(glm::vec2(expFactor * maxRadius));
	currentRadius = minRadius + expandSpeed * timer;
	if (currentRadius > maxRadius)
		currentRadius = minRadius;
}

void AreaSpawnBullet_expanding::reset() {
	AreaSpawnBullet::reset();
	setSize(0.0f);
}

AreaSpawnBulletTargeted_expanding::AreaSpawnBulletTargeted_expanding(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float _expandSpeed)
	: AreaSpawnBullet_expanding(img, pos, size, _timer_max), AreaSpawnBullet_targetControl() 
{
	expandSpeed = _expandSpeed;
}

void AreaSpawnBulletTargeted_expanding::update(Map* map) {
	AreaSpawnBullet_expanding::update(map);
	processTargets(map);
}

/************************************************************
	Rotating
************************************************************/
AreaSpawnBullet_rotating::AreaSpawnBullet_rotating(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float _rotSpeed)
	: AreaSpawnBullet(img, pos, size, _timer_max) 
{
	rotSpeed = _rotSpeed;
}
void AreaSpawnBullet_rotating::update(Map* map) {
	AreaSpawnBullet::update(map);
	rotate(rotSpeed);
}

AreaSpawnBulletTargeted_rotating::AreaSpawnBulletTargeted_rotating(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float rotSpeed) 
	: AreaSpawnBullet_rotating(img, pos, size, _timer_max, rotSpeed), AreaSpawnBullet_targetControl(){}

void AreaSpawnBulletTargeted_rotating::update(Map* map){
	AreaSpawnBullet_rotating::update(map);
	processTargets(map);
}

/************************************************************
	Animated
		- AreaSpawnBullet_animated         - animation once
		- AreaSpawnBulletTargeted_animated - animation looped
************************************************************/
AreaSpawnBullet_animated::AreaSpawnBullet_animated(const std::string& animID, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float _animFPS)
	: AreaSpawnBullet(animID, pos, size, _timer_max) 
{
	animFPS = _animFPS;
}

void AreaSpawnBullet_animated::start() {
	playSprite(animFPS, (int)loopAnim);
}
void AreaSpawnBullet_animated::setLoopAnimation(bool loopState) {
	loopAnim = loopState;
}

AreaSpawnBulletTargeted_animated::AreaSpawnBulletTargeted_animated(const std::string& animID, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float animFPS)
	: AreaSpawnBullet_animated(animID, pos, size, _timer_max, animFPS), AreaSpawnBullet_targetControl() 
{
	setLoopAnimation(true);
}

void AreaSpawnBulletTargeted_animated::update(Map* map) {
	AreaSpawnBullet_animated::update(map);
	processTargets(map);
}

/****************/
/****************/
void AreaSpawnBullet_targetControl::processTargets(Map* map) {
	newTargets.clear();
	for (Mob* m = targetCells->firstMobInRange(false); m; m = targetCells->nextMobInRange(false)) {
		if (allTargets.find(m) == allTargets.end()) {
			newTargets.insert(m);
			allTargets.insert(m);
		}
	}

	/*for (Cell* c : targetCells) {
		for (Mob* m : (*c->getMobs())) {
			if (m->isAlive()) {
				if (allTargets.find(m) == allTargets.end()) {
					newTargets.insert(m);
					allTargets.insert(m);
				}
			}
		}
	}*/
}

std::set<Mob*>* AreaSpawnBullet_targetControl::getNewTargets() {
	return &newTargets;
}
void AreaSpawnBullet_targetControl::setTarget(CellRange* _targetCells) {
	targetCells = _targetCells;
	//setPosition(cells[0]->getPos() + glm::vec2(CELL_SIZE / 2.0f));
	//timer = 0.0f;
}




/*

AreaSpawnBullet_targeted::AreaSpawnBullet_targeted(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max)
	: AreaSpawnBullet(img, pos, size, _timer_max) {}

void AreaSpawnBullet_targeted::setTarget(const std::vector<Cell*>& cells) {
	targetCells = cells;
	setPosition(cells[0]->getPos() + glm::vec2(CELL_SIZE / 2.0f));
	timer = 0.0f;
}

void AreaSpawnBullet_targeted::update(Map* map) {
	AreaSpawnBullet::update(map);
	rotate(10.0f);
	
	newTargets.clear();
	for (Cell* c : targetCells) {
		for (Mob* m : (*c->getMobs())) {
			if (m->isAlive()) {
				if (allTargets.find(m) == allTargets.end()) {
					newTargets.insert(m);
					allTargets.insert(m);
				}
			}
		}
	}

}

std::set<Mob*>* AreaSpawnBullet_targeted::getNewTargets() {
	return &newTargets;
}

void AreaSpawnBullet_targeted::reset() {
	AreaSpawnBullet::reset();
	allTargets.clear();
	newTargets.clear();	
}

*/