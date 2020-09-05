#pragma once
#include "MobDescriptor.h"
//#include "MobEffect.h"
//#include "Tower.h"
#include "UI/Sprite.h"
#include "UI/TextSprite.h"
#include "MediaBank.h"

//#include <unordered_map>
#include <deque>


#define LIFE_BAR_WIDTH	7.5f
#define LIFE_BAR_HEIGHT 1.5f
#define LIFE_BAR_OFFSET 8.0f



class Mob : public Sprite
{
public:
	Mob(MobDescriptor* descriptor, const glm::vec2& pos) : Sprite(descriptor->image, pos, descriptor->size) {
		mobDescriptor = descriptor;
		
		lifebar = new Sprite(glm::vec3(0, 1, 0), glm::vec2(0), glm::vec2(LIFE_BAR_WIDTH, LIFE_BAR_HEIGHT));
		lifebar->setOffset(glm::vec2(LIFE_BAR_WIDTH/2.0f, LIFE_BAR_OFFSET));
		lifebar->hide();

		debugText = new TextSprite("x", glm::vec3(1,0,0), glm::vec2(pos), 5.0f);
		debugText->hide();

		totalLife = mobDescriptor->health;
		speed = mobDescriptor->speed_min;

		_pathPos = pos;
	}
	virtual void update(td::direction dir, float offs);
	//virtual void processEffects();
	//virtual void addEffect(MobEffect* effect, int effectID);
	//bool effectExist(int effectID);
	virtual void deleteSprite();

	virtual void setX(float x);
	virtual void setY(float y);
	virtual void setPosition(float x, float y);
	virtual void setState(bool state);

	void enable() { active = true; }
	void disable() { active = false; }
	bool isActive() { return active; }

	float takeDamage(float dmg);
	void die();
	bool isDead() { return _isDead; }
	bool isAlive() { return !_isDead; }

	float getSpeed() { return speed; }
	void setSpeed(float _speed) { speed = _speed; }

	void setDebugText(const uString& txt) { debugText->setText((char*)txt.GetStr()); }


	UINT queueIndex() { return _queueIndex; }
	void setQueueIndex(UINT index) { _queueIndex = index; }

	MobDescriptor* getDescriptor() { return mobDescriptor; }

	float travelledDistance() { return _travelledDistance; }
	glm::vec2 pathPos() { return _pathPos; }

protected:
	MobDescriptor*	mobDescriptor = NULL;
	Sprite*			lifebar = NULL;
	bool			active = false;
	td::direction		currentDirection;
	float			speed = 1.0f;
	float			lifePercent = 1.0f;
	float			totalLife = 100.0f;
	
	UINT			_queueIndex = -1;
	bool			_isDead = false;
	float			_travelledDistance = 0.0f;
	glm::vec2		_pathPos;

	//std::unordered_map<int, MobEffect*>							effectMap;
	//std::unordered_map<mob_effect_type, int, MobEffectTypeHash>	appliedEffectTypeMap;

	TextSprite* debugText = NULL;
};

/*
class MobT1 : public Mob
{
public:
	MobT1(glm::vec2& pos) : Mob(pos, MEDIA_BANK.getImage(MOBT1_IMAGE), glm::vec2(10.0f, -1)) {
		setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	}
private:
};
*/