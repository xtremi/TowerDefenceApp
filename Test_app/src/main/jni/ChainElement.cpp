#include "ChainElement.h"

ChainElement::ChainElement(ChainBullet* _parentBullet, ChainElement* _parentElement, Mob* _target, Sprite* _source, 
	float _range, int _nestedLevel, 
	int _maxChildren, int _nestingLimit, float _expandTime,
	int _shaderID)
	: Sprite(GETIMG(ASSET::IMG_BLANK_128x128), _source->getPos(), glm::vec2(50.0f, CHAIN_SPRITE_WIDTH))
{
	parentBullet	= _parentBullet;
	parentElement	= _parentElement;
	target			= _target;
	source			= _source;
	range			= _range;
	nestedLevel		= _nestedLevel;
	maxChildren		= _maxChildren;
	time_until_expand = _expandTime;
	maxNestedLevel = _nestingLimit;
	shaderID		= _shaderID;

	parentBullet->addElement(this);
	parentBullet->addTarget(_target);

	agk::SetSpriteShader(id, shaderID);
	setAlignment(AgkHAlign::left, AgkVAlign::center);
	setDepth(BULLET_SPRITE_DEPTH);
	show();
	update();
}

void ChainElement::update() {
	if (!hasExpanded && nestedLevel < maxNestedLevel) {
		time += DELTA_TIME;
		if (time > time_until_expand){
			needNextTargets = true;
		}
	}
	updateOrientationAndLength(source->getPos(), target->getPos());
}

ChainElement* ChainElement::getParentElement() {
	return parentElement;
}

int ChainElement::needTargets() {
	if (needNextTargets)
		return maxChildren;
	return 0;
}
void ChainElement::resetExpandTimer() {
	time			= 0.0f;
	hasExpanded		= false;
	needNextTargets = false;
}

void ChainElement::disable() {
	if (!disabled) {

		deleteSprite();
		parentBullet->removeTarget(target);

		disabled	 = true;		
		source		 = nullptr;
		target		 = nullptr;
		parentBullet = nullptr;
		
		auto it = children.begin();
		while (it != children.end())
		{
			(*it)->disable();
			it = children.erase(it);
		}
	}
}

bool ChainElement::isDisabled() {
	return disabled;
}

bool ChainElement::isDone() {
	if (disabled)
		return false;
	if (chain_length > range)
		return true;
	if (!target || (target && target->isDead()))
		return true;
	return false;
}

void ChainElement::updateOrientationAndLength(const glm::vec2& spos, const glm::vec2& tpos) {
	glm::vec2 d = tpos - spos;
	chain_length = glm::length(d);
	setSize(glm::vec2(chain_length, CHAIN_SPRITE_WIDTH));
	setPosition(spos);
	float ang = agk::ATanFull(d.x, d.y);
	setRotation(ang - 90.0f);
}

void ChainElement::removeChainElementChild(ChainElement* ce) {
	children.erase(ce);
}

void ChainElement::addNewChainTarget(Mob* m) {
	children.insert(new ChainElement(parentBullet, this, m, target, range, nestedLevel + 1, maxChildren, 
		maxNestedLevel, time_until_expand, shaderID));
	hasExpanded = true;
	needNextTargets = false;
}


void ChainElement::dump() {

	std::string elC = "";
	if (getTarget())
		elC += "[" + std::to_string(getTarget()->queueIndex()) + "]";
	else
		elC += "[--]";

	std::string timerC = "<T" + toStdStr(agk::Str(time, 2)) + "/" + toStdStr(agk::Str(time_until_expand, 2)) + ">";
	elC += timerC;
	elC += "<L" + std::to_string(nestedLevel) + ">";
	elC += "<" + toStdStr(agk::Str(chain_length / range, 1)) + ">";
	elC += (hasExpanded ? "<EXP>" : "<   >");
	elC += (disabled ? "<DIS>" : "<   >");
	elC += (needNextTargets ? "<NXT>" : "<   >");
	elC += "\n";

	agk::PrintC(&elC[0]);
}


/*
void ChainElement::findNextTargets() {

	std::vector<Mob*> allMobsInRange;
	for (Cell* c : (*cellsInRange)) {
		for (Mob* mob = c->firstMob(); mob; mob = c->nextMob()) {
			allMobsInRange.push_back(mob);
		}
	}

	std::set<Mob*> newTargets;
	int nNonAllowedMobs = nestedLevel > 0 ? 2 : 1; //if nestedLevel is 0, only target is a mob, if more than 0, source and target is a mob
	int nNewTargets = (allMobsInRange.size() - nNonAllowedMobs) < maxChildren ? (allMobsInRange.size() - nNonAllowedMobs) : maxChildren;
	for (int i = 0; i < nNewTargets; i++) {

		bool foundNewMob = false;
		while (!foundNewMob) {
			int randomIndex = agk::Random(0, allMobsInRange.size() - 1);
			Mob* m = allMobsInRange[randomIndex];
			if ((newTargets.find(m) == newTargets.end()) && (m != target) && (m != source)){
				newTargets.insert(m);
				foundNewMob = true;
			}
		}

	}

	for(Mob* m : newTargets){}

};*/


