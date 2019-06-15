#pragma once
#include "TowerDescriptor.h"
#include "Building.h"
#include "Bullet.h"
#include "Mob.h"
#include "MediaBank.h"
#include "UI/SpriteAndText.h"


extern int BULLET_COUNT;

#define NEW_BULLET_MODE

class TowerAbility {
public:
	TowerAbility(){}
	virtual void process() = 0;

private:

};


class TowerFreeze : public TowerAbility 
{
public:
	TowerFreeze() : TowerAbility(){}
	void process() {

	}

private:


};

/*class TowerAimless : public Tower {

};
class TowerAim : public Tower {

};
class TowerMissile : public Tower {

};*/

/*NEW*/
/*class Tower : public Building 
{
public:
	Tower(TowerDescriptor* _towerDescriptor);	
	void process();
	void upgradeTo(TowerDescriptor* newTowerDescriptor);
	TowerDescriptor* getDescriptor() { return towerDescriptor; }
	void setDirection(const glm::vec2& dir);

	void deleteBuilding();

	int getBuildingID();
	bool isTower() { return true; }
	bool canAim() { return towerDescriptor->canRotate; }
	float getRoundProfit() { return roundProfit; }
	float getTotalProfit() { return totalProfit; }

protected:
	TowerDescriptor* towerDescriptor = NULL;
	std::vector<Sprite*> rotSprites;

	std::vector<Bullet*> activeBullets;
	std::vector<Bullet*> idleBullets;

	int bulletIterator = 0;

	void loadBuildingSprites();
	void deleteBuildingSprites();
	void deleteBulletSprites();

	bool mobIsInRange(Mob* m);
	bool hasMobInRange();
	int nMobsInRange();

	virtual void onBulletHitMob(Mob* mob, Bullet* b);
	virtual void reload();
	virtual bool chooseTarget();
	virtual void aim();
	virtual void shoot();
	virtual void checkBullets();
	virtual void updateBullets();
	virtual void upgradeBullets();

	virtual void checkMobBulletCollisions();
	virtual void processTowerMobInteration();
	virtual bool collide(Bullet* b, Mob* m);

	void updateDebugTextSprites();
	void initDebugTextSprites();

	Bullet* firstBullet() {
		bulletIterator = 0;
		return nextBullet();
	}
	Bullet* nextBullet() {
		std::vector<Bullet*>* bvec = &activeBullets;
		if (bulletIterator < bvec->size()) {
			return (*bvec)[bulletIterator++];
		}
		return NULL;
	}

protected:
	float	reload_timer	= 0.0f;
	bool	isLoaded		= false;
	float	dmgMulitplier	= 1.0f;
	int		nkills			= 0;
	float	totalDmg		= 0.0f;
	float currentAimAngle	= 0.0f;

	float roundProfit		= 0.0f;
	float totalProfit		= 0.0f;

	Mob* currentTarget = NULL;

	//bool isAiming		= false;		//The tower has a mob in range, and is aiming to it.
	bool isInSight		= false;		//The tower has a mob in sight, and can shoot at it.
	bool isStationary	= false;		//The tower has no mob in range, and has reached its "resting" direction (orientation)

};*/



class Tower : public Building
{
public:
	Tower(TowerDescriptor* _towerDescriptor);
	void process();
	int getBuildingID();
	void deleteBuilding();
	Bullet* firstBullet() {
		bulletIterator = 0;
		return nextBullet();
	}
	Bullet* nextBullet() {
		
		std::vector<Bullet*>* bvec = NULL;
#ifdef NEW_BULLET_MODE
		bvec = &activeBullets;
#else
		bvec = &bullets;
#endif
		if (bulletIterator < bvec->size()){
			return (*bvec)[bulletIterator++];
		}
		return NULL;
	}

	bool isTower() { return true; }
	void onBulletHitMob(Mob* mob, Bullet* b);
	void upgradeTo(TowerDescriptor* newTowerDescriptor);
	
	bool canAim() { return towerDescriptor->canRotate; }
	bool mobIsInRange(Mob* m);
	int nMobsInRange();
	bool hasMobInRange();

	TowerDescriptor* getDescriptor() { return towerDescriptor; }

	void setDirection(const glm::vec2& dir);

	float getRoundProfit() { return roundProfit; }
	float getTotalProfit() { return totalProfit; }

protected:
	TowerDescriptor* towerDescriptor = NULL;
	std::vector<Sprite*> rotSprites;

#ifdef NEW_BULLET_MODE
	std::vector<Bullet*> activeBullets;
	std::vector<Bullet*> idleBullets;
#else
	std::vector<Bullet*> bullets;	
#endif

	int bulletIterator = 0;

	void reload();
	bool chooseTarget();
	void aim();
	void shoot();
	void checkBullets();
	void updateBullets();
	void upgradeBullets();

	void checkMobBulletCollisions();
	void processTowerMobInteration();
	bool collide(Bullet* b, Mob* m);

	void loadBuildingSprites();
	void deleteBuildingSprites();
	void deleteBulletSprites();



protected:
	float	reload_timer	= 0.0f;
	bool	isLoaded		= false;
	float	dmgMulitplier	= 1.0f;
	int		nkills			= 0;
	float	totalDmg		= 0.0f;
	float currentAimAngle	= 0.0f;
	
	float roundProfit = 0.0f;
	float totalProfit = 0.0f;
	
	Mob* currentTarget = NULL;

	//bool isAiming		= false;		//The tower has a mob in range, and is aiming to it.
	bool isInSight		= false;		//The tower has a mob in sight, and can shoot at it.
	bool isStationary	= false;		//The tower has no mob in range, and has reached its "resting" direction (orientation)

protected:
	void updateDebugTextSprites();
	void initDebugTextSprites();
	

};

