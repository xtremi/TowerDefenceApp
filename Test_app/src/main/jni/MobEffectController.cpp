#include "MobEffectController.h"

void MobEffectController::process() {

	//TODO: optimize with another container faster iterating (std::vector):
	auto  it = mobEffectsMap.begin();
	while (it != mobEffectsMap.end()) {

		auto it_t = it->second.begin();
		while (it_t != it->second.end()) {

			auto it_eff = it_t->second.begin();
			while (it_eff != it_t->second.end()) {
				MobEffect* eff = it_eff->effect;
				if (eff->completed() || it_eff->mob->isDead()) {
					eff->revert(it_eff->mob);
					mobEffectIDs.erase(it_eff->id);
					it_eff = it_t->second.erase(it_eff);
					delete eff;
				}
				else{
					eff->process(it_eff->mob);
					it_eff++;
				}				
			}
			if (it_t->second.empty()) it_t = it->second.erase(it_t);
			else it_t++;
		}
		if (it->second.empty())	it = mobEffectsMap.erase(it);
		else it++;
	}
}

void MobEffectController::removeEffect(BulletValues* effectStats, Mob* mob, Tower* tower) {
		
	auto itm = mobEffectsMap.find(mob);
	if (itm != mobEffectsMap.end()) 
	{
		auto ite = itm->second.find(effectStats->mobEffectType());
		if (ite != itm->second.end()) {
			
			auto itt = ite->second.begin();
			while (itt != ite->second.end()) {
				if (itt->tower == tower) {
					
					itt->effect->revert(itt->mob);
					mobEffectIDs.erase(itt->id);
					delete itt->effect;
					itt = ite->second.erase(itt);
				}					
				else
					itt++;
			}

		}
	}

}

void MobEffectController::addEffect(BulletValues* effectStats, Mob* mob, Tower* tower) {
	
	bool mobHasNoEffects				= true;
	bool exactEffectApplicationExist	= false;
	bool existingEffectNeedCheck		= false;
	int id = -1;

	//if this mob has no effects, no need to check anything more:
	mobHasNoEffects		= mobEffectsMap.find(mob) == mobEffectsMap.end();

	//If this mob has effects, do a "quick" check if this exact effect (same mob, same tower, same effect):
	if (!mobHasNoEffects) {
		id = makeAppliedEffectID(effectStats, mob, tower);
		exactEffectApplicationExist = mobEffectIDs.find(id) != mobEffectIDs.end();

		if (exactEffectApplicationExist) return;

		//If this mob doesn't have the same effect from the same tower (unless it is stackable effect):
		if(!exactEffectApplicationExist && !mobEffectCanStack(effectStats->mobEffectType()))
			existingEffectNeedCheck = mobEffectsMap[mob].find(effectStats->mobEffectType()) != mobEffectsMap[mob].end();
	}

	if (id < 0)	
		id = makeAppliedEffectID(effectStats, mob, tower);

	/*
		This mob has no effects OR it has, but the existing effects doesn't need to be checked:
		Just create and apply the effect:
	*/
	if (mobHasNoEffects || !existingEffectNeedCheck) {		
		applyEffect(createEffect(effectStats), mob, tower, id);
	}
	/*
		This mob already has an effect of the same type, which is not stackable:
		Need to compare the two effects, to see which one to keep:
	*/
	else {
		applied_effect* existingAppliedEffect = &mobEffectsMap[mob][effectStats->mobEffectType()][0]; //this effect is not stackable so we know its on index = 0
		MobEffect*		existingEffect		  = existingAppliedEffect->effect;	
		MobEffect*		newEffect			  = createEffect(effectStats);

		mobEffectIDs.erase(existingAppliedEffect->id);
		mobEffectIDs.insert(id);

		//Existing effect is "stronger":
		if (existingEffect->strength() > newEffect->strength()) {		
			//Keep the old effect, but change its ID (next time it will not checked):			
			existingAppliedEffect->id = id;			
		}
		//New effect is "stronger":
		else{
			//remove the applied effect:			
			existingAppliedEffect->effect->revert(existingAppliedEffect->mob);
			delete existingAppliedEffect->effect;
			mobEffectsMap[mob][effectStats->mobEffectType()].clear(); //can clear instead of erase because here we know its not stackable
			applyEffect(newEffect, mob, tower, id);
		}

	}
}


void MobEffectController::applyEffect(MobEffect* effect, Mob* mob, Tower* tower, int id) {
	applied_effect newAppliedEffect;
	newAppliedEffect.effect = effect;
	newAppliedEffect.mob	= mob;
	newAppliedEffect.tower	= tower;
	newAppliedEffect.id		= id;

	mobEffectsMap[mob][effect->type()].push_back(newAppliedEffect);
	mobEffectIDs.insert(id);

	effect->apply(mob);
}

MobEffect* MobEffectController::createEffect(BulletValues* estats)
{
	switch (estats->mobEffectType())
	{
	case mob_effect_type::dmg_over_time:
		return new MobEffectDOT(estats->damage(), estats->debuffInterval(), estats->debuffDuration());
		break;
	case mob_effect_type::speed:
		return new MobEffectSpeed(estats->speedFactor(), estats->debuffDuration());
		break;
	case mob_effect_type::temp_health_reduction:
	default:
		return nullptr;
	}
}

int MobEffectController::makeAppliedEffectID(BulletValues* effectStats, Mob* mob, Tower* tower) {
	return tower->getLocX()*100 + tower->getLocY()*10 + mob->getID() + (int)effectStats->mobEffectType();
}