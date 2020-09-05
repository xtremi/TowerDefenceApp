#pragma once
#include "Mob.h"

bool mobEffectCanStack(mob_effect_type eff);




class MobEffect
{
public:
	MobEffect(mob_effect_type effType, float _time_max) {
		_type = effType;
		time_max = _time_max;
	}
	~MobEffect() {
		if(animationSprite){
			animationSprite->deleteSprite();
			delete animationSprite;
			animationSprite = nullptr;
		}
	}
	mob_effect_type type() { return _type; }
	
	virtual void apply(Mob* m) = 0;
	virtual void revert(Mob* m) = 0;
	virtual void process(Mob* m) = 0;
	bool canBeStacked() { return mobEffectCanStack(_type); }
	
	virtual bool completed() { return time > time_max; }	
	float strength() { return _strength; }

protected:
	mob_effect_type _type;
	float _strength = 0.0f;
	float time_max = 0.0f;
	float time = 0.0f;	

	Sprite* animationSprite = nullptr;

};

class MobEffectSpeed : public MobEffect
{
public:
	MobEffectSpeed(float _speedFactor, float _time_max) : MobEffect(mob_effect_type::speed,  _time_max) {
		speed_factor = _speedFactor;
		_strength = 1.0 / speed_factor;

		animationSprite = MEDIA_BANK.createAnimationSpriteCopy("ice-debuff", glm::vec2(20.0f, -1));		
	}
	void apply(Mob* m);
	void revert(Mob* m);
	void process(Mob* m);
	
private:
	float speed_factor = 1.0f;
	float original_speed = 0.0f;
};


class MobEffectDOT : public MobEffect
{
public:
	MobEffectDOT(float _tickDamage, float _tickLength, float _time_max) : MobEffect(mob_effect_type::dmg_over_time, _time_max) {
		tickDamage = _tickDamage;
		tickLength = _tickLength;

		_strength = (tickDamage / tickLength) * time_max;

		animationSprite = MEDIA_BANK.createAnimationSpriteCopy("fire-debuff", glm::vec2(20.0f, -1));
		//animationSprite->setColor(glm::vec3(0, 0, 1));
	}
	void apply(Mob* m);
	void revert(Mob* m);
	void process(Mob* m);
	
private:
	float tickDamage = 0.0f;
	float tickLength = 0.0f;
	float tickTime = 0.0f;
};