#include "ChainBullet.h"


/*
	CHAIN BULLET (main)
*/
ChainBullet::ChainBullet(AgkImage* img, const glm::vec2& pos, float _range, 
	float _timer_max, int _maxChildren, int _nestingLimit, float _expandTime, 
	int _shaderID/*, CellRange* _cellsRange*/)
	: Bullet(img, pos, glm::vec2(CELL_SIZE)), MultiTargetBullet()
{
	//cellRange		= _cellsRange;
	range			= _range;
	timer_max		= _timer_max;
	maxChildren		= _maxChildren;
	nestingLimit	= _nestingLimit;
	expandTime		= _expandTime;
	shaderID		= _shaderID;
}

bool ChainBullet::isDone() {
	return bulletIsDone;
}

void ChainBullet::deleteSprite() {
	Sprite::deleteSprite();
	for (ChainElement* ce : elements) {
		ce->deleteSprite();
		delete ce;
	}
	elements.clear();
}


void ChainBullet::reset() {
	bulletIsDone	= false;
	timer			= 0.0f;
	//cellRange		= nullptr;
	baseElement     = nullptr;
	elements.clear();
	allTargets.clear();
	newTargets.clear();
	releasedTargets.clear();
}


void ChainBullet::setTarget(Mob* m) {
	TargetedBullet::setTarget(m);
	if(targetMob){
		baseElement = new ChainElement(this, nullptr, targetMob, this, range, 0, maxChildren, nestingLimit, expandTime, shaderID);		
	}
	else {
		bulletIsDone = true;
	}
	firstUpdate = true;
}

void ChainBullet::update(Map* map) {

	releasedTargets.clear();
	if (!firstUpdate) newTargets.clear();
	else firstUpdate = false;

	timer += DELTA_TIME;
	if (timer > timer_max || baseElement->isDone()) {
		baseElement->disable();
		baseElement  = nullptr;
		bulletIsDone = true;
	}

	updateChildElements(map);
	rotate(1.0f);
	//dump();
}

void ChainBullet::updateChildElements(Map* map) {

	/*--FIRST ITERATION----------------------------------------------------------------------------------
	|
	|	First iteration "disables" any element that "isDone", and recursevely "disables" all its children
	|	 - "isDone" means either the target is Dead or out of range.
	|	 - "disabling" means to delete its sprite, remove the target, and clear its children
	|
	|	 - For the element that "isDone", we also remove its relation from its parent.
	|
	|	We can skip this iteration, if the baseElement (which is also in elements vec) is a nullptr,
	|	because then we know all elements have already been disabled, and will be deleted (in second iteration)
	|-----------------------------------------------------------------------------------------------------
	*/
	auto it = elements.begin();
	if(baseElement){
		while(it != elements.end())	
		{
			ChainElement* ce = (*it);		
			if (ce->isDone()){
				ce->getParentElement()->removeChainElementChild(ce);
				ce->disable();		
			}
			it++;
		}
	}

	/*--SECOND ITERATION----------------------------------------------------------------------------------
	|	Second iteration will delete any disabled elements
	|	and update non-disabled elements:
	|
	|	- Deleting means to remove its from the elements collection and deleting the object,
	|     other clean up is done when disabled.
    |	
	|   - Updating processes the ChainElement and looks for new targets if it is required
	|
	|-----------------------------------------------------------------------------------------------------
	*/
	it = elements.begin();
	while (it != elements.end()) {
		ChainElement* ce = (*it);
		if (ce->isDisabled()) {			
			it = elements.erase(it);
			delete ce;
		}
		else {
			ce->update();
			if (int nTargets = ce->needTargets())
				getNewTargetsForChildElement(ce, nTargets, map);
			
			if (_isLazer) {				
				addLazerTargets(ce, map);
			}
			
			it++;			
		}			
	}



}

void ChainBullet::addLazerTargets(ChainElement* ce, Map* map) {
	std::vector<Cell*> cellsAlongLine;
	map->getRoadCellsAlongLine(ce->getPos(), ce->getTarget()->getPos(), cellsAlongLine);
	for (Cell* c : cellsAlongLine) {
		for (Mob* m = c->firstMob(); m; m = c->nextMob()) {
			if (agk::GetSpriteCollision(ce->getID(), m->getID())) {
				addTarget(m);
			}
		}
	}
}

void ChainBullet::getNewTargetsForChildElement(ChainElement* ce, int nTargets, Map* map) {
	
	//Get cells in range of bullet:
	Cell* bulletElementCell = map->getCellAtLocation(ce->getPos());
	std::vector<Cell*> cellsInElementRange;
	map->getRoadCellsInRange(bulletElementCell, range/CELL_SIZE, cellsInElementRange);

	//Get mobs in range of bullet:
	std::vector<Mob*> allMobsInRange;
	for (Cell* c : cellsInElementRange) {
		for (Mob* mob = c->firstMob(); mob; mob = c->nextMob()) {
			if (allTargets.find(mob) == allTargets.end()) {
				float targetDistance = glm::distance(ce->getPos(), mob->getPos());
				if(targetDistance < range)
					allMobsInRange.push_back(mob);
			}
				
		}
	}

	//Get the targets:
	std::set<Mob*> newTargets; //this is already defined in class defintion.. try to remove later
	nTargets = allMobsInRange.size() < nTargets ? allMobsInRange.size() : nTargets;

	if (nTargets == 0) {
		ce->resetExpandTimer();
	}
	else{
		for (int i = 0; i < nTargets; i++) {

			bool foundNewMob = false;
			while (!foundNewMob) {
				int randomIndex = agk::Random(0, allMobsInRange.size() - 1);
				Mob* m = allMobsInRange[randomIndex];
				if ((newTargets.find(m) == newTargets.end())) {
					addTarget(m);
					ce->addNewChainTarget(m);
					foundNewMob = true;
				}
			}
		}
	}

}



void ChainBullet::removeElement(ChainElement* el) {
	elements.erase(el);
}
void ChainBullet::addElement(ChainElement* el) {
	elements.insert(el);
}
void ChainBullet::removeTarget(Mob* m) {
	allTargets.erase(m);
	releasedTargets.insert(m);
}
void ChainBullet::addTarget(Mob* m){
	allTargets.insert(m);
	newTargets.insert(m);
}


void ChainBullet::dump() {
	for (int i = 0; i < 14; i++)
		agk::Print("");
	
	std::string str = "";
	str += ("n elements: " + std::to_string(elements.size()) + "\n");
	str += ("n targets : " + std::to_string(allTargets.size()) + "\n");
	str += ("timer     : " + toStdStr(agk::Str(timer, 2)) + "/" + toStdStr(agk::Str(timer_max, 2)) + "\n");

	agk::PrintC(&str[0]);
	int j = 0;
	for (ChainElement* ce : elements) {
		ce->dump();
	}


}

