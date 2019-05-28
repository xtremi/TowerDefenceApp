#pragma once
#include "Mob.h"
#include "MobJSONReader.h"
#include "Map.h"
#include "AnimationController.h"
#include <algorithm>
#include <map>
#include "PerlinNoise.h"

#define TIMER_STEP 0.01f
#define RELEASE_INTERVAL 0.2f 

#define MOB_DESCRIPTOR_JSON_FILE "json/mob_defs.json"


class MobController {

public:
	MobController(){
		initPerlinNoiseValues();		
	}
	void loadMobDescriptors();
	void setAnimationController(AnimationController* _animCtrl) { 
		animCtrl = _animCtrl; 
		animCtrl->getSprite("red-explode")->setSize(glm::vec2(40.0f));
		animCtrl->getSprite("red-explode")->setDepth(3);
	}

	void addMobs(Cell* c, int amount, int type);
	void update(Map* map);
	void hide();
	void show();

	Mob* getFirstMob() {
		if(activeMobs.size() > 0)
			return activeMobs[0];
		return NULL;
	}

	bool allMobsDead() { 
		return (activeMobs.size() == 0);
	}
	int liveMobs() { return activeMobs.size(); }
	int mobQueueSize() { return mobQueue.size(); }

	
	MobDescriptor* firstMobDescriptor() {
		mobDescrIt = mobDescriptors.begin();
		return nextMobDescriptor();
	}
	MobDescriptor* nextMobDescriptor() {
		if (mobDescrIt != mobDescriptors.end()) {
			MobDescriptor* td = &mobDescrIt->second;
			mobDescrIt++;
			return td;
		}
		return NULL;
	}

	void setReleaseInterval(float interval) { currentReleaseInterval = interval; }

private:
	AnimationController* animCtrl = NULL;

	std::deque<Mob*> mobQueue;
	std::deque<Mob*> activeMobs;

	float timer = 0.0f;
	float currentReleaseInterval = 1.0f;

	std::map<int, MobDescriptor> mobDescriptors;
	std::map<int, MobDescriptor>::iterator mobDescrIt;

	void initPerlinNoiseValues();
	std::vector<float> offsetPNvalues;
	int offsetPNiter = 0;



	std::map<std::string, Sprite*> explosionAnimations;
	std::map<std::string, std::vector<AgkImage*>> explosionImages;
	void loadExplosionAnimations();

};