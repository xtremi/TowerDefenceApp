#include "Mob.h"

void Mob::update(td::direction dir, float offs) {
	
	currentDirection = dir;

	glm::vec2 pathVec = getDirectionAsVector(dir);
	glm::vec2 offsVec = getOrthogonalDirectionAsVector(dir);
	pathVec *= speed;
	offsVec *= offs;

	_pathPos += pathVec;
	glm::vec2 newPos = _pathPos + offsVec;
	setPosition(newPos.x, newPos.y);

	//TODO: some optimization can be done here
	glm::vec2 moveVec = glm::normalize(pathVec);	
	float ang = agk::ATanFull(moveVec.x, moveVec.y);
	if (ang < 0.0f) ang += 360.0f;
		setRotation(ang + 90.0f);

	_travelledDistance += speed;

	//processEffects();
}

/*
	Check if an effect (debuff) of the specified ID exist already
	the effectID is a concatination of tower location and effect type.
	
	This should be called before addEffect(), to make sure a tower with a specific debuff effect
	is only applied once to a mob.
*/
/*bool Mob::effectExist(int effectID) {
	return effectMap.find(effectID) != effectMap.end();
}*/

/*
	Adds an effect to mob.

	If the type of effect (debuff) cannot be "stacked" a check is done
	to see if an effect of the same type is already applied (appliedEffectTypeMap).

	If it can't be stacked and it already has the effect type, the strength of the two effects
	are compared. The strongest effect is used.
*/
/*void Mob::addEffect(MobEffect* effect, int effectID) {
			
	//Checks if a debuff of same type (but different tower is already applied):
	if (effect->canBeStacked() || (appliedEffectTypeMap.find(effect->type()) == appliedEffectTypeMap.end())){
		effectMap[effectID] = effect;
		appliedEffectTypeMap[effect->type()] = effectID;
		effect->apply(this);
	}
	else {

		//Get the existing debuff of same type:
		int			existingEffectID	  = appliedEffectTypeMap[effect->type()];
		MobEffect*	existingEffect		  = effectMap[existingEffectID];
		float		currentEffectStrength = existingEffect->strength();
			
		//Revert the existing debuff effect, and remove it from the maps:
		existingEffect->revert(this);
		appliedEffectTypeMap.erase(effect->type());
		effectMap.erase(existingEffectID);

		//If the previous debuff is weaker, re-call addEffect function with the new effect:
		if (currentEffectStrength < effect->strength()){
			addEffect(effect, effectID);
			delete existingEffect;
		}
		//If the previous debuff is stronger, re-call addEffect function with the previous effect, but with new ID:
		//(The timer value is kept)
		else{
			addEffect(existingEffect, effectID);
			delete effect;
		}
	}
	
}*/


/*void Mob::processEffects() {
	
	auto  it = effectMap.begin();
	while (it != effectMap.end()) {
		MobEffect* effect = it->second;
		effect->process(this);		
		if (isDead()) return;

		if (effect->completed()) {
			effect->revert(this);						
			it = effectMap.erase(it);
			
			if(!effect->canBeStacked())
				appliedEffectTypeMap.erase(effect->type());

			delete effect;
		}
		else
			it++;
	}
}*/

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
		lifebar->setScale(glm::vec2(lifePercent, 1.0f));
		damageTaken = dmg;
	}
	return damageTaken;
}