#pragma once
#include "Bullet.h"
#include "AreaSpawnBulletTypes.h"

/*
AreaSpawnBullet_targetControl

Inherited by specialized AreaSpawnBullet classes requiring target control.
TargetControl - checks new targets comming in to the targets cells. 

*/
class AreaSpawnBullet_targetControl {
public:
	AreaSpawnBullet_targetControl() {}

	//void setTarget(const std::vector<Cell*>& cells);
	void setTarget(CellRange* _targetCells);
	std::set<Mob*>* getNewTargets();

protected:
	void processTargets(Map* map);

	//std::vector<Cell*>	targetCells;
	CellRange*			targetCells;
	std::set<Mob*>		newTargets;
	std::set<Mob*>		allTargets;
};

/*
	Base class for area spawn bullets
*/
class AreaSpawnBullet : public Bullet {
public:
	AreaSpawnBullet(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max);
	AreaSpawnBullet(const std::string& animID, const glm::vec2& pos, const glm::vec2& size, float _timer_max);

	virtual void start(){}
	virtual void update(Map* map);
	virtual bool isDone();
	virtual void reset();

protected:
	area_spawn_bullet_type type;
	float timer		= 0.0f;
	float timer_max = 1.0f;
};

/*
	Expanding AreaSpawnBullet
*/
class AreaSpawnBullet_expanding : public AreaSpawnBullet {
public:
	AreaSpawnBullet_expanding(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max);

	virtual void update(Map* map);
	virtual void reset();

protected:
	float currentRadius = 0.0f;
	float minRadius     = 0.0f;
	float maxRadius     = 0.0f;
	float expandSpeed   = 0.0f;
};

/*
	Expanding AreaSpawnBullet with targets
*/
class AreaSpawnBulletTargeted_expanding : public AreaSpawnBullet_expanding, public AreaSpawnBullet_targetControl {
public:
	AreaSpawnBulletTargeted_expanding(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float _expandSpeed);
	virtual void update(Map* map);
};


/*
	Rotating AreaSpawnBullet
*/
class AreaSpawnBullet_rotating : public AreaSpawnBullet {
public:
	AreaSpawnBullet_rotating(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float rotSpeed);
	virtual void update(Map* map);
	
protected:
	float rotSpeed = 0.0f;
};

/*
	Rotating AreaSpawnBullet with targets
*/
class AreaSpawnBulletTargeted_rotating : public AreaSpawnBullet_rotating, public AreaSpawnBullet_targetControl {
public:
	AreaSpawnBulletTargeted_rotating(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float rotSpeed);
	virtual void update(Map* map);
};

/*
	Animated AreaSpawnBullet
*/
class AreaSpawnBullet_animated : public AreaSpawnBullet {
public:
	AreaSpawnBullet_animated(const std::string& animID, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float animFPS);	
	virtual void start();
	void setLoopAnimation(bool loopState);
protected:
	float animFPS = 0.0f;
	bool loopAnim = false;
};

/*
	Animated AreaSpawnBullet with targets
*/
class AreaSpawnBulletTargeted_animated : public AreaSpawnBullet_animated, public AreaSpawnBullet_targetControl {
public:
	AreaSpawnBulletTargeted_animated(const std::string& animID, const glm::vec2& pos, const glm::vec2& size, float _timer_max, float animFPS);
	virtual void update(Map* map);
};


/*
class AreaSpawnBullet_targeted : public AreaSpawnBullet {
public:
	AreaSpawnBullet_targeted(AgkImage* img, const glm::vec2& pos, const glm::vec2& size, float _timer_max);

	void setTarget(const std::vector<Cell*>& cells);
	void update(Map* map);
	void reset();

	std::set<Mob*>* getNewTargets();

private:
	std::vector<Cell*>	targetCells;
	std::set<Mob*>		newTargets;
	std::set<Mob*>		allTargets;
};
*/