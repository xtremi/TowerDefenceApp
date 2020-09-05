#pragma once
#include "Building.h"
#include "Map.h"
#include "TowerDescriptor.h"
#include "MediaBank.h"

#include "CircleBullet.h"
#include "AreaSpawnBullet.h"
#include "FollowBullet.h"
#include "ChainBullet.h"
#include "ExpandBullet.h"
#include "PathBullet.h"
#include "LazerBullet.h"

#include "UI/SpriteAndText.h"

#define TOWER_SPRITE_DEPTH_BACK  9
#define TOWER_SPRITE_DEPTH_FRONT 6
#define TOWER_RELOAD_BAR_DEPTH_BACK 8
#define TOWER_RELOAD_BAR_DEPTH_FRONT 7

extern int BULLET_COUNT;

struct MobEffectFromTower {
	MobEffectFromTower(BulletValues* _effectStats,	Mob* _mob){
		effectStats = _effectStats;
		mob			= _mob;
	}
	BulletValues* effectStats;
	Mob* mob;
};

enum class target_selection_type {
	first,
	last,
	nearest,
	random,
	highest_hp
};

AreaSpawnBullet* createAreaSpawnBullet(BulletStats* bs, const glm::vec2& pos, const glm::vec2& size);

class Tower : public Building 
{
public:
	Tower(TowerDescriptor* _towerDescriptor);	
	virtual void	init(Cell* cell);
	
	void	update(Map* map);

	bool	isTower() { return true; }
	int		getBuildingID();
	void	deleteBuilding();
	float	rangeCells();
	float	rangeDistance();
	float	price();
	float	value();
		
	TowerDescriptor* getDescriptor() { return towerDescriptor; }
	std::vector<MobEffectFromTower>* getNewMobEffects() { return &newMobEffects; }
	std::vector<MobEffectFromTower>* getMobEffectsToRemove() { return &mobEffectsToRemove; }

	bool canAim() { return towerDescriptor->canRotate; }
	float getRoundProfit() { return roundProfit; }
	float getTotalProfit() { return totalProfit; }

	virtual void onTowerKillMob(Mob* mob);

protected:
	TowerDescriptor*		towerDescriptor = NULL;
	std::vector<Bullet*>	activeBullets;
	std::vector<Bullet*>	idleBullets;
	Sprite*					reloadBarSprite;
	Sprite*					reloadBarBGsprite;
	
	Mob*					currentTarget		= nullptr;
	Cell*					currentCellTarget   = nullptr;
	glm::vec2				targetPosition;
	glm::vec2				shootDirection;
	Sprite*					mobHitAnimSprite	= nullptr;
	Sprite*					mobKilledAnimSprite = nullptr;

	target_selection_type targetSelectionType = target_selection_type::first;

	std::vector<MobEffectFromTower> newMobEffects;
	std::vector<MobEffectFromTower> mobEffectsToRemove;

	float	reload_timer		= 0.0f;
	bool	isLoaded			= false;
	float	dmgMulitplier		= 1.0f;
	int		nkills				= 0;
	float	totalDmg			= 0.0f;
	float	realRangeDistance	= 0.0f;
	float	roundProfit			= 0.0f;
	float	totalProfit			= 0.0f;

	void loadReloadBarSprite();
	void deleteReloadBarSprites();
	virtual void loadStaticSprites();
	virtual void loadRotatableSprites();

	void initBullets();
	void deleteBulletSprites();
	

	bool _hasMobInRange = false;
	bool hasMobInRange() { return _hasMobInRange; }

	void reload();	
	void checkBulletStatus();
	void updateBullets(Map* map);

	void checkMobBulletCollisions(Map* map);
	void processTowerMobInteration(Map* map);

	virtual void chooseTarget(Map* map);
	virtual void aim();
	virtual bool canShoot();
	virtual void shoot();
	virtual void shoot(Bullet* b);
	virtual bool collide(Bullet* b, Mob* m);
	virtual void onBulletHitMob(Map* map, Mob* mob, Bullet* b, bool aoe, float reduction = 1.0f);
	virtual void onBulletUnhitMob(Mob* mob, Bullet* b);
	void onBulletHitMobAoe(Map* map, Mob* mob, Bullet* b);

	virtual void shootAreaSpawnBullet(Bullet* b);

	int bulletIterator = 0;
	Bullet* firstBullet();
	Bullet* nextBullet();

	void updateDebugTextSprites();
	void initDebugTextSprites();
	
};
