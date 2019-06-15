#include "Tower.h"

int BULLET_COUNT = 0;

Tower::Tower(TowerDescriptor* _towerDescriptor) : Building() {
	
	upgradeTo(_towerDescriptor);
	initDebugTextSprites();
	currentAimAngle = 0.0f;
	//aimDir = orientation;

#ifdef NEW_BULLET_MODE
	glm::vec2 bsize(towerDescriptor->bulletStats.width, towerDescriptor->bulletStats.heigth);
	for (int i = 0; i < 5; i++) {
		CircleBullet* newBullet = new CircleBullet(towerDescriptor->bulletStats.bulletImage, cpos, bsize);
		idleBullets.push_back(newBullet);
		newBullet->hide();
	}
#endif
}

int Tower::getBuildingID() {
	return towerDescriptor->ID;
}



void Tower::upgradeTo(TowerDescriptor* newTowerDescriptor) {
	towerDescriptor = newTowerDescriptor;
	deleteBuildingSprites();
	loadBuildingSprites();

	setPosition(this->cpos, this->maploc[0], this->maploc[1]);
	range = towerDescriptor->range;
	setDirection(orientation);

	upgradeBullets();
}


void Tower::upgradeBullets() {

	for (auto& bit : activeBullets) {
		bit->setImage(towerDescriptor->bulletStats.bulletImage);
		bit->setSize(towerDescriptor->bulletStats.width, towerDescriptor->bulletStats.heigth);
	}
	for (auto& bit : idleBullets) {
		bit->setImage(towerDescriptor->bulletStats.bulletImage);
		bit->setSize(towerDescriptor->bulletStats.width, towerDescriptor->bulletStats.heigth);
	}

}

void Tower::loadBuildingSprites() {
	for (AgkImage* fimg : towerDescriptor->images.fixed) {
		Sprite* s = new Sprite(fimg, glm::vec2(0), glm::vec2(CELL_SIZE));
		s->setDepth(5);
		sprites.push_back(s);
	}
	for (AgkImage* rimg : towerDescriptor->images.rotatable) {
		Sprite* s = new Sprite(rimg, glm::vec2(0), glm::vec2(CELL_SIZE));
		s->setDepth(4);
		rotSprites.push_back(s);
		sprites.push_back(s);
	}
}

void Tower::deleteBuilding() {
	Building::deleteBuilding();
	deleteBulletSprites();
}

void Tower::deleteBulletSprites() {
	for (Bullet* b : idleBullets) {
		b->deleteSprite();
		delete b;
	}
	for (Bullet* b : activeBullets) {
		b->deleteSprite();
		delete b;
	}
	idleBullets.clear();
	activeBullets.clear();
}

void Tower::deleteBuildingSprites() {
	Building::deleteBuildingSprites();
	rotSprites.clear();
}

void Tower::process() {
	updateDebugTextSprites();

	reload();
	
	static const UINT colr = agk::MakeColor(255, 0, 0);
	float rad = range * CELL_SIZE * agk::GetViewZoom();
	//agk::DrawEllipse(agk::WorldToScreenX(cpos.x), agk::WorldToScreenY(cpos.y), rad, rad, colr, 0, 0);

	if (towerDescriptor->canRotate){
		chooseTarget();
		if(!isStationary)
			aim();
	}

	if (isLoaded) {
		if (!towerDescriptor->canRotate && hasMobInRange())	//aimless towers
			shoot();
		else if (towerDescriptor->canRotate && isInSight)	//aiming towers
			shoot();
	}
	/*if(!towerDescriptor->canRotate || (towerDescriptor->canRotate && isInSight)){
		if (isLoaded && hasMobInRange())
			shoot();
	}*/

	checkBullets();
	updateBullets();
	processTowerMobInteration();
}



void Tower::reload(){
	if (!isLoaded) {
		reload_timer += DELTA_TIME;
		if (reload_timer >= towerDescriptor->reload_time) {
			isLoaded = true;
		}
	}
}

//find the current target and sets its position to aimPos
bool Tower::chooseTarget() {
	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {
			if(m->isAlive() && mobIsInRange(m)){
				currentTarget = m;
				isStationary = false;
				return true;
			}
		}
	}
	currentTarget = NULL;
	return false;
}

bool Tower::mobIsInRange(Mob* m) {
	float targetDistance = glm::distance(getPosition(), m->getPos());
	if (targetDistance < towerDescriptor->range * CELL_SIZE)
		return true;
	return false;
}
int Tower::nMobsInRange() {
	int nmobs = 0;
	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {
			if (m->isAlive() && mobIsInRange(m)) {
				nmobs++;
			}
		}
	}
	return nmobs;
}
bool Tower::hasMobInRange() {
	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {						
			if (m->isAlive() && mobIsInRange(m)) {
				return true;
			}			
		}
	}
	return false;
}

void Tower::aim() {
	
	glm::vec2 targetDir;
	if (currentTarget){
		targetDir = currentTarget->getPos() - cpos;
		targetDir = glm::normalize(targetDir);
	}
	else
		targetDir = orientation;
			
	float targetAngle  =  agk::ATanFull(targetDir.x, targetDir.y);
				
	if (targetAngle < 0.0f) targetAngle += 360.0f;
	if (currentAimAngle < 0.0f) currentAimAngle += 360.0f;

	float dAngle = targetAngle - currentAimAngle;
	if (dAngle < 0.0f) dAngle += 360.0f;

	if (dAngle < 180.0f) {
		currentAimAngle += towerDescriptor->rotate_speed;
	}
	else {
		currentAimAngle -= towerDescriptor->rotate_speed;
	}

	if (dAngle < 2.0f || dAngle > 358.0f){
		currentAimAngle = targetAngle;
		if (currentTarget)
			isInSight = true;
		else
			isStationary = true;
	}
	else{
		isInSight = false;
		isStationary = false;
	}

	for (Sprite* s : rotSprites) {
		s->setRotation(currentAimAngle);
	}
}


void Tower::shoot() {
	glm::vec2 bsize(towerDescriptor->bulletStats.width, towerDescriptor->bulletStats.heigth);
	glm::vec2 shootDirection;
	if(canAim()) shootDirection = glm::vec2(agk::Cos(currentAimAngle - 90.0f), agk::Sin(currentAimAngle - 90.0f));
	else shootDirection = orientation;
	//MEDIA_BANK.getSound(SHOOT_TICK)->play(100, 0);

#ifdef NEW_BULLET_MODE
	Bullet* blt = idleBullets.back();
	idleBullets.pop_back();
	blt->setDirection(shootDirection, towerDescriptor->bulletStats.speed);
	blt->show();
	activeBullets.push_back(blt);
#else
	CircleBullet* newBullet = new CircleBullet(MEDIA_BANK.getImage(STONE_BULLET_IMAGE), cpos, bsize, shootDirection, towerDescriptor->bulletStats.speed);
	bullets.push_back(newBullet);	
#endif	

	BULLET_COUNT++;
	isLoaded = false;
	reload_timer = 0.0f;

}
void Tower::checkBullets() {
#ifdef NEW_BULLET_MODE
	for (auto bit = activeBullets.begin(); bit != activeBullets.end();) {

		if ((*bit)->getNhits() > 0 ||
			glm::distance((*bit)->getPos(), this->cpos) > (CELL_SIZE * towerDescriptor->range + CELL_SIZE / 2.0f))
		{
			Bullet* blt = (*bit);
			blt->hide();						//hide and move back to tower
			blt->reset();
			blt->setPosition(cpos);
			bit = activeBullets.erase(bit);		//remove from active and add to idle
			idleBullets.push_back(blt);
		}
		else ++bit;
	}
#else
	for (auto bit = bullets.begin(); bit != bullets.end();) {

		if ((*bit)->getNhits() > 0 || glm::distance((*bit)->getPos(), this->cpos) > (CELL_SIZE * towerDescriptor->range + CELL_SIZE / 2.0f)) {
			(*bit)->deleteSprite();
			delete (*bit);
			bit = bullets.erase(bit);
		}
		else
			++bit;
	}
#endif // NEW_BULLET_MODE
}
void Tower::updateBullets() {
#ifdef NEW_BULLET_MODE
	for (Bullet* b : activeBullets)
		b->update();
#else
	for (Bullet* b : bullets)
		b->update();
#endif
}

void Tower::onBulletHitMob(Mob* mob, Bullet* b) {
	float takenDmg = mob->takeDamage(towerDescriptor->bulletStats.damage);
	b->addHit();
	
	if (mob->isDead()){
		this->roundProfit += mob->getDescriptor()->reward;
		this->nkills++;
		if (mob == currentTarget)
			currentTarget = NULL;
	}
	this->totalDmg = takenDmg;
}


void Tower::processTowerMobInteration()
{	
	roundProfit = 0.0f;
	checkMobBulletCollisions();	
	totalProfit += roundProfit;
}


bool Tower::collide(Bullet* b, Mob* m) {
	if (agk::GetSpriteCollision(b->getID(), m->getID()))
		return true;
	return false;
}

void Tower::checkMobBulletCollisions() {
	float nearestMobDistance = getRange() * CELL_SIZE * 2.0f;
	glm::vec2 nearestMobPosition(0.0f);
	glm::vec2 tpos = getPosition();

	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {
			if(m->isAlive()){
				for (Bullet* blt = firstBullet(); blt; blt = nextBullet()) {
					if (collide(blt, m)) {
						onBulletHitMob(m, blt);
					}
				}
			}
		}
	}
}


void Tower::setDirection(const glm::vec2& dir) {
	if (this->canAim()){
		orientation = dir;
		isStationary = false;
	}
	else
		Building::setDirection(dir);
}

/********************************************************************/

void Tower::updateDebugTextSprites() {
	int nmobs = 0;
	for (Cell* c : cellsInRange) {
		nmobs += c->getMobs()->size();
	}
	
#ifdef NEW_BULLET_MODE
	std::string bulletsStr = "B: " + std::to_string(activeBullets.size()) + "//" + std::to_string(idleBullets.size());
#else
	std::string bulletsStr = "B: " + std::to_string(bullets.size());
#endif
	std::string cellsInRangeStr = "C: " + std::to_string(cellsInRange.size());
	std::string nmobsInRangeStr = "M: " + std::to_string(nmobs);
	
	debug_buildingStats[0]->setText(&nmobsInRangeStr[0]);
	debug_buildingStats[1]->setText(&bulletsStr[0]);
	debug_buildingStats[2]->setText(&cellsInRangeStr[0]);

	for (SpriteAndText* ts : debug_buildingStats)
		ts->realign();
}
void Tower::initDebugTextSprites() {
	float w		= 10.0f;
	float h		= 2.2f;
	float offs	= -3.0f;

	for (int i = 0; i < 3; i++) {
		SpriteAndText* ts = new SpriteAndText(glm::vec3(1, 1, 0), "0", glm::vec3(0), glm::vec2(w, h), h);
		ts->getSprite()->setOffset(glm::vec2(w / 2.0f, offs));
		ts->getSprite()->setColorAlpha(0.04f);
		ts->getText()->setVertOffset(offs);
		ts->setDepth(3);
		offs += h;

		debug_buildingStats.push_back(ts);
	}
}