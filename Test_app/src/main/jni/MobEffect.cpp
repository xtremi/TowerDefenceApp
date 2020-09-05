#include "MobEffect.h"
#include "Tower.h"

bool mobEffectCanStack(mob_effect_type eff) {
	switch (eff)
	{
	case mob_effect_type::dmg_over_time:
		return true;
	case mob_effect_type::speed:
	case mob_effect_type::temp_health_reduction:
	default:
		return false;
	}
}

//MobEffectSpeed
void MobEffectSpeed::apply(Mob* m) {
	original_speed = m->getSpeed();
	m->setSpeed(original_speed * speed_factor);

	animationSprite->playSprite(12.0f, 1);
	animationSprite->show();
}
void MobEffectSpeed::revert(Mob* m) {
	m->setSpeed(original_speed);
	animationSprite->hide();
	animationSprite->stopSprite();
}
void MobEffectSpeed::process(Mob* m) {
	time += DELTA_TIME;
	animationSprite->setPosition(m->getPos());
}

//MobEffectDOT
void MobEffectDOT::apply(Mob* m) {
	animationSprite->playSprite(12.0f, 1);
	animationSprite->show();
}
void MobEffectDOT::revert(Mob* m) {
	animationSprite->hide();
	animationSprite->stopSprite();
}
void MobEffectDOT::process(Mob* m) {
	time += DELTA_TIME;
	tickTime += DELTA_TIME;
	
	animationSprite->setPosition(m->getPos());
	
	if (tickTime > tickLength) {
		tickTime = 0.0f;
		m->takeDamage(tickDamage);
	}
}

