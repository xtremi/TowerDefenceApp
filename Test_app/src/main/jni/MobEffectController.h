#pragma once
#include "Tower.h"
#include "MobEffect.h"

#include <unordered_set>
#include <unordered_map>

struct applied_effect {
	MobEffect*	effect	= nullptr;
	Mob*		mob		= nullptr;
	Tower*		tower	= nullptr;
	int			id		= 0;
};

struct MobEffectTypeHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

typedef std::unordered_map<Mob*, std::unordered_map<mob_effect_type, std::vector<applied_effect>, MobEffectTypeHash>> mob_effect_map;

class MobEffectController {
public:
	MobEffectController(){}
	void process();
	void addEffect(BulletValues* effectStats, Mob* mob, Tower* tower);
	void removeEffect(BulletValues* effectStats, Mob* mob, Tower* tower);

	int getTotalNumberOfEffects() {
		return mobEffectIDs.size();
	}

	int getTotalNumberOfMobWithEffect() {
		return mobEffectsMap.size();
	}

private:
	MobEffect* createEffect(BulletValues* estats);
	void applyEffect(MobEffect*	effect, Mob* mob, Tower* tower, int id);
	int makeAppliedEffectID(BulletValues* effectStats, Mob* mob, Tower* tower);

	//std::vector<applied_effect> appliedEffects;
	std::unordered_set<int> mobEffectIDs;
	mob_effect_map			mobEffectsMap;
	/*
		Mob 1 : {
					effect_type1 : {
						[effect_11, effect_12]
					},		
					effect_type2 : {
						[effect_21, effect_22, effect_23]
					},
					effect_type3 : {
						[effect_31]
					}
				},
		Mob 2 : {
					effect_type1 : {
						[effect_11, effect_12]
					},
					effect_type4 : {
						[effect_21, effect_22]
					}
				},
	
	*/
	
};