#include "Tower.h"

int BULLET_COUNT = 0;

Tower::Tower(TowerDescriptor* _towerDescriptor) 
	: Building() 
{	
	towerDescriptor = _towerDescriptor;
	realRangeDistance = CELL_SIZE * (towerDescriptor->range + 0.5f);	
}

void Tower::init(Cell* cell) {
	Building::init(cell);
	loadReloadBarSprite();
	initBullets();
	setDirection(orientation);
	shootDirection = orientation;

	if (towerDescriptor->bulletStats.effectVisualization == bullet_effect_vis_type::animation) {
		mobHitAnimSprite = MEDIA_BANK.createAnimationSpriteCopy(
			towerDescriptor->bulletStats.hitAnimationStats.animationID,
			glm::vec2(towerDescriptor->bulletStats.hitAnimationStats.size));
	}
}

void Tower::loadReloadBarSprite() {
	glm::vec2 p = getPosition() + glm::vec2(0.0f, CELL_SIZE / 2.2f);
	glm::vec2 s(0.5f*CELL_SIZE, 0.08f*CELL_SIZE);
	reloadBarSprite			= new Sprite(colours::green, p, s, AgkHAlign::center, AgkVAlign::bottom);
	reloadBarBGsprite	= new Sprite(colours::blue,  p, s, AgkHAlign::center, AgkVAlign::bottom);
	
	reloadBarSprite->setDepth(TOWER_RELOAD_BAR_DEPTH_FRONT);
	reloadBarBGsprite->setDepth(TOWER_RELOAD_BAR_DEPTH_BACK);
}
void Tower::deleteReloadBarSprites() {
	reloadBarSprite->deleteSprite();
	reloadBarBGsprite->deleteSprite();
	delete reloadBarSprite;
	delete reloadBarBGsprite;
}

void Tower::deleteBuilding() {
	Building::deleteBuilding();
	deleteBulletSprites();
	deleteReloadBarSprites();
}

void Tower::initBullets() {
	BulletStats* bs = &towerDescriptor->bulletStats;
	glm::vec2 bsize(bs->size);

	int totalBulletBufferSize = towerDescriptor->bullet_buffer_size * towerDescriptor->aoe_dir;
	for (int i = 0; i < totalBulletBufferSize; i++) {

		Bullet* newBullet = nullptr;
		switch (bs->type)
		{
		case bullet_type::follow:
			newBullet = new FollowBullet(bs->bulletImage, cpos, bsize, bs->data.speed(), rangeDistance());
			break;
		case bullet_type::area_spawn_long:
			newBullet = createAreaSpawnBullet(&towerDescriptor->bulletStats, cpos, bsize);
			break;
		case bullet_type::area_spawn_instant:
			newBullet = createAreaSpawnBullet(&towerDescriptor->bulletStats, cpos, glm::vec2(rangeDistance()*2.0f, -1.0f));
			break;
		case bullet_type::chain:
			newBullet = new ChainBullet(GETIMG(ASSET::IMG_TEST_CHAIN_MARKER), cpos, realRangeDistance, 
				bs->data.duration(), bs->data.chainMaxChildren(), bs->data.chainMaxNestedLevel(), bs->data.chainExpandTime(),
				MEDIA_BANK.getShader(bs->data.getShaderKey())/*, &cellsInRange*/);
			((ChainBullet*)newBullet)->setLazer(towerDescriptor->bulletStats.data.lazerEffectOnChainBullet());
			break;
		case bullet_type::lazer:
			newBullet = new LazerBullet(cpos, realRangeDistance, bs->data.duration(), MEDIA_BANK.getShader(bs->data.getShaderKey()));
			break;
		case bullet_type::normal:
			newBullet = new CircleBullet(bs->bulletImage, cpos, bsize);
			break;
		case bullet_type::path:
			newBullet = new PathBullet(bs->bulletImage, cpos, bsize, bs->data.speed());
			break;
		default:
			break;
		}

		idleBullets.push_back(newBullet);
		newBullet->hide();//TODO: move to CircleBullet or Bullet constructor
	}
}

float Tower::value() {
	return towerDescriptor->price / 2.0f;
}
float Tower::price() {
	return towerDescriptor->price;
}
float Tower::rangeCells() {
	return towerDescriptor->range;
}
float Tower::rangeDistance() {
	return realRangeDistance;
}

int Tower::getBuildingID() {
	return towerDescriptor->ID;
}

void Tower::loadStaticSprites() {
	for (AgkImage* fimg : towerDescriptor->images.fixed) {
		Sprite* s = new Sprite(fimg, glm::vec2(0), glm::vec2(CELL_SIZE));
		s->setDepth(TOWER_SPRITE_DEPTH_BACK);
		staticSprites.push_back(s);
	}
}

void Tower::loadRotatableSprites(){
	for (AgkImage* rimg : towerDescriptor->images.rotatable) {
		Sprite* s = new Sprite(rimg, glm::vec2(0), glm::vec2(CELL_SIZE));
		s->setDepth(TOWER_SPRITE_DEPTH_FRONT);
		rotSprites.push_back(s);
		staticSprites.push_back(s);
	}
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

void Tower::update(Map* map) {
	updateDebugTextSprites();

	_hasMobInRange = cellRange->checkIfHasMobInRange();

	newMobEffects.clear();
	mobEffectsToRemove.clear();
	roundProfit = 0.0f;
	
	reload();
	chooseTarget(map);
	aim();
	if (isLoaded && canShoot()) {
		shoot();


	}
		

	checkBulletStatus();
	updateBullets(map);
	processTowerMobInteration(map);

	totalProfit += roundProfit;
}


void Tower::reload() {
	if (!isLoaded && !idleBullets.empty()) {
		reload_timer += DELTA_TIME;
		if (reload_timer >= towerDescriptor->reload_time) {
			isLoaded = true;
		}
		float ratio = reload_timer / towerDescriptor->reload_time;
		reloadBarSprite->setScale(ratio, 1.0f);
	}
}
void Tower::aim() {}
void Tower::chooseTarget(Map* map) {}
bool Tower::canShoot() { 

	if (towerDescriptor->bulletStats.type == bullet_type::path && !currentCellTarget)
		return false;
	else if (towerDescriptor->bulletStats.type == bullet_type::chain && !currentTarget)
		return false;
	return true; 
}

void Tower::shoot(Bullet* blt) {

	switch (towerDescriptor->bulletStats.type){
	case bullet_type::normal:
		blt->setPosition(cpos + shootDirection * 10.0f);
		blt->setDirection(shootDirection, towerDescriptor->bulletStats.data.speed());
		break;
	case bullet_type::follow:
		blt->setPosition(cpos + shootDirection * 10.0f);
		((FollowBullet*)blt)->setTarget(currentTarget);
		blt->updateDirection(shootDirection);
		break;
	case bullet_type::area_spawn_long:
	case bullet_type::area_spawn_instant:
		shootAreaSpawnBullet(blt);
		break;
	case bullet_type::chain:
		blt->setPosition(cpos + shootDirection * 10.0f);
		((ChainBullet*)blt)->setTarget(currentTarget);
		//blt->updateDirection(shootDirection);
		break;
	case bullet_type::lazer:
		((LazerBullet*)blt)->setTarget(currentTarget);
		blt->updateDirection(shootDirection);
		break;
	case bullet_type::path:
		blt->setPosition(cpos + shootDirection * 10.0f);
		((PathBullet*)blt)->setTarget(currentCellTarget);
		blt->updateDirection(shootDirection);
	default:
		break;
	}

}

void Tower::shoot() {

	if(!idleBullets.empty()){

		Bullet* blt = idleBullets.back();
		idleBullets.pop_back();
		shoot(blt);		
		blt->show();
		activeBullets.push_back(blt);

		BULLET_COUNT++;
		isLoaded = false;
		reload_timer = 0.0f;
	}

}

void Tower::shootAreaSpawnBullet(Bullet* blt){
	((AreaSpawnBullet*)blt)->setPosition(targetPosition);
	switch (towerDescriptor->bulletStats.type)
	{
	case bullet_type::area_spawn_long:
		(dynamic_cast<AreaSpawnBullet_targetControl*>(blt))->setTarget(cellRange);				
		break;
	case bullet_type::area_spawn_instant:
		((AreaSpawnBullet*)blt)->start();
		break;
	default: break;
	}
}

void Tower::checkBulletStatus() {
	for (auto bit = activeBullets.begin(); bit != activeBullets.end();) {

		bool bulletDone = false;
		if (towerDescriptor->bulletStats.type == bullet_type::normal ||
			towerDescriptor->bulletStats.type == bullet_type::follow ||
			towerDescriptor->bulletStats.type == bullet_type::path) 
		{
			glm::vec2 bpos = (*bit)->getPos();
			float d = glm::distance((*bit)->getPos(), this->cpos);
			bulletDone = d > realRangeDistance;
		}
		
		bulletDone = bulletDone || (*bit)->isDone();

		if (bulletDone){
			Bullet* blt = (*bit);
			blt->hide();						//hide and move back to tower //TODO: move hide to bullet::reset()
			blt->reset();
			blt->setPosition(cpos);
			bit = activeBullets.erase(bit);		//remove from active and add to idle
			idleBullets.push_back(blt);
		}
		else ++bit;
	}
}

void Tower::updateBullets(Map* map) {
	for (Bullet* b : activeBullets){
		b->update(map);
	}
}

void Tower::processTowerMobInteration(Map* map)
{	
	if (!activeBullets.empty()) 
		checkMobBulletCollisions(map);
}

void Tower::checkMobBulletCollisions(Map* map) {

	if (towerDescriptor->bulletStats.type == bullet_type::chain || towerDescriptor->bulletStats.type == bullet_type::lazer)
	{
		for (Bullet* blt = firstBullet(); blt; blt = nextBullet()) {
			
			if(towerDescriptor->bulletStats.effectType == bullet_hit_effect_type::debuff){
				std::set<Mob*>* newTargets = dynamic_cast<MultiTargetBullet*>(blt)->getNewTargets();
				for (Mob* m : (*newTargets)) onBulletHitMob(map, m, blt, towerDescriptor->bulletStats.aoeStats.hasAoe);
				if(towerDescriptor->bulletStats.data.releaseDebuffOnChainEnd()){
					std::set<Mob*>* relTargets = dynamic_cast<MultiTargetBullet*>(blt)->getReleasedTargets();
					for (Mob* m : (*relTargets)) onBulletUnhitMob(m, blt);
				}
			}
			else if (towerDescriptor->bulletStats.effectType == bullet_hit_effect_type::damage_over_time) {
				std::set<Mob*>* curTargets = dynamic_cast<MultiTargetBullet*>(blt)->getCurrentTargets();
				for (Mob* m : (*curTargets)) onBulletHitMob(map, m, blt, towerDescriptor->bulletStats.aoeStats.hasAoe);
			}
			else if (towerDescriptor->bulletStats.effectType == bullet_hit_effect_type::damage) {
				std::set<Mob*>* curTargets = dynamic_cast<MultiTargetBullet*>(blt)->getNewTargets();
				for (Mob* m : (*curTargets)) onBulletHitMob(map, m, blt, towerDescriptor->bulletStats.aoeStats.hasAoe);
			}
		}		
	}
	else if (towerDescriptor->bulletStats.type == bullet_type::area_spawn_long) {
		for (Bullet* blt = firstBullet(); blt; blt = nextBullet()) {
			std::set<Mob*>* newTargets = (dynamic_cast<AreaSpawnBullet_targetControl*>(blt))->getNewTargets();				
			for (Mob* m : (*newTargets)) 
				onBulletHitMob(map, m, blt, towerDescriptor->bulletStats.aoeStats.hasAoe);
		}
	}
	else if (towerDescriptor->bulletStats.type == bullet_type::area_spawn_instant) {
		
		for (Bullet* blt = firstBullet(); blt; blt = nextBullet()) {
			if(blt->isDone()){
				for (Mob* m = cellRange->firstMobInRange(false); m; m = cellRange->nextMobInRange(false)) {
					onBulletHitMob(map, m, blt, towerDescriptor->bulletStats.aoeStats.hasAoe);
				}
			}
		}
	}
	else{
		for (Mob* m = cellRange->firstMobInRange(false); m; m = cellRange->nextMobInRange(false)) {
			for (Bullet* blt = firstBullet(); blt; blt = nextBullet()) {
				if (collide(blt, m)) {
					onBulletHitMob(map, m, blt, towerDescriptor->bulletStats.aoeStats.hasAoe);
				}
			}
		}
	}
}

bool Tower::collide(Bullet* b, Mob* m) {
	return agk::GetSpriteCollision(b->getID(), m->getID());
}

void Tower::onBulletUnhitMob(Mob* mob, Bullet* b) {

	if (towerDescriptor->bulletStats.effectType == bullet_hit_effect_type::debuff) {
		mobEffectsToRemove.push_back(MobEffectFromTower(&towerDescriptor->bulletStats.data, mob));
	}

}

void Tower::onBulletHitMobAoe(Map* map, Mob* mob, Bullet* b) {
	double rad = towerDescriptor->bulletStats.aoeStats.aoeRange;
	double red = towerDescriptor->bulletStats.aoeStats.aoeReduction;

	Cell* c = map->getCellAtLocation(mob->getPos());
	std::vector<Cell*> cells;
	map->getRoadCellsInRange(c, rad, cells);
	CellRange cr;
	cr.assignCells(cells, mob->getPos(), rad * CELL_SIZE);
	for (Mob* m = cr.firstMobInRange(true); m; m = cr.nextMobInRange(true)) {
		onBulletHitMob(map, m, b, false, red);
	}
}

void Tower::onBulletHitMob(Map* map, Mob* mob, Bullet* b, bool aoe, float reduction) {
	
	if (aoe)
		onBulletHitMobAoe(map, mob, b);		
	
	float takenDmg = 0.0f;
	
	if( towerDescriptor->bulletStats.type == bullet_type::normal || 
		towerDescriptor->bulletStats.type == bullet_type::follow)
	{
		b->addHit();
	}

	if (towerDescriptor->bulletStats.effectType == bullet_hit_effect_type::damage ||
		towerDescriptor->bulletStats.effectType == bullet_hit_effect_type::damage_over_time) 
	{
		takenDmg = mob->takeDamage(reduction * towerDescriptor->bulletStats.data.damage());
	}
	else if (towerDescriptor->bulletStats.effectType == bullet_hit_effect_type::debuff) 
	{				
		newMobEffects.push_back(MobEffectFromTower(&towerDescriptor->bulletStats.data, mob));
	}


	if (mobHitAnimSprite) {
		mobHitAnimSprite->setPosition(mob->getPos());
		mobHitAnimSprite->playSprite(towerDescriptor->bulletStats.hitAnimationStats.fps, 0);
		mobHitAnimSprite->show();
	}


	if (mob->isDead()) 
		onTowerKillMob(mob);
	
	totalDmg = takenDmg; //is this needed?
}

void Tower::onTowerKillMob(Mob* mob) {
	roundProfit += mob->getDescriptor()->reward;
	nkills++;
}


/*bool Tower::mobIsInRange(Mob* m) {
	float targetDistance = glm::distance(cpos, m->getPos());
	return targetDistance < realRangeDistance;		
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
}*/





Bullet* Tower::firstBullet() {
	bulletIterator = 0;
	return nextBullet();
}
Bullet* Tower::nextBullet() {
	std::vector<Bullet*>* bvec = &activeBullets;
	if (bulletIterator < bvec->size()) {
		return (*bvec)[bulletIterator++];
	}
	return NULL;
}


/********************************************************************/

void Tower::updateDebugTextSprites() {
	
	if(debugSpriteON){	
		int nmobs = 0;
		for (Cell* c = cellRange->firstCellInRange(); c; c = cellRange->nextCellInRange()) {
			nmobs += c->getMobs()->size();
		}
	
		std::string bulletsStr		= "B: " + std::to_string(activeBullets.size()) + "//" + std::to_string(idleBullets.size());
		std::string cellsInRangeStr = "C: " + std::to_string(cellRange->getNumberOfCellsInRange());
		std::string nmobsInRangeStr = "M: " + std::to_string(nmobs);
	
		debug_buildingStats[0]->setText(&nmobsInRangeStr[0]);
		debug_buildingStats[1]->setText(&bulletsStr[0]);
		debug_buildingStats[2]->setText(&cellsInRangeStr[0]);

		for (SpriteAndText* ts : debug_buildingStats)
			ts->realign();
	}
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
		if (!debugSpriteON) ts->hide();
		debug_buildingStats.push_back(ts);
	}
}

AreaSpawnBullet* createAreaSpawnBullet(BulletStats* bs, const glm::vec2& pos, const glm::vec2& size)
{
	float duration = bs->data.duration();
	if (bs->type == bullet_type::area_spawn_long) {
		switch (bs->data.areaSpawnBulletType())
		{
		case area_spawn_bullet_type::animation:
			return new AreaSpawnBulletTargeted_animated(bs->data.getAnimationKey(), pos, size, duration, bs->data.animFPS());
		case area_spawn_bullet_type::expansion:
			return new AreaSpawnBulletTargeted_expanding(bs->bulletImage, pos, size, duration, bs->data.expandSpeed());
		case area_spawn_bullet_type::rotation:
			return new AreaSpawnBulletTargeted_rotating(bs->bulletImage, pos, size, duration, bs->data.rotSpeed());
		case area_spawn_bullet_type::standard:
		default:break;
		}

	}
	else if (bs->type == bullet_type::area_spawn_instant) {
		switch (bs->data.areaSpawnBulletType())
		{
		case area_spawn_bullet_type::animation:		
			return new AreaSpawnBullet_animated(bs->data.getAnimationKey(), pos, size, duration, bs->data.animFPS());
		case area_spawn_bullet_type::expansion:
			return new AreaSpawnBullet_expanding(bs->bulletImage, pos, size, duration);
		case area_spawn_bullet_type::rotation:
			return new AreaSpawnBullet_rotating(bs->bulletImage, pos, size, duration, bs->data.rotSpeed());
		case area_spawn_bullet_type::standard:
		default:break;
		}
	}
	return nullptr;
}


/*Mob* Tower::getFirstMobInRange(){
	for (Cell* ct = firstCellInRange(); ct; ct = nextCellInRange()) {
		for (Mob* m = ct->firstMob(); m; m = ct->nextMob()) {
			if (m->isAlive() && mobIsInRange(m)) {
				return m;
			}
		}
	}
	return nullptr;
}


Mob* Tower::getLastMobInRange() {
	for (Cell* ct = lastCellInRange(); ct; ct = prevCellInRange()) {
		for (Mob* m = ct->lastMob(); m; m = ct->prevMob()) {
			if (m->isAlive() && mobIsInRange(m)) {
				return m;
			}
		}
	}
	return nullptr;
}*/