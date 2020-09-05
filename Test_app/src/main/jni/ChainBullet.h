#pragma once
#include "Bullet.h"
#include "ChainElement.h"
#include "Cell.h"

static const float CHAIN_SPRITE_WIDTH		= 4.0f;

class ChainElement;

class ChainBullet : public Bullet, public TargetedBullet {

public:
	ChainBullet(AgkImage* img, const glm::vec2& pos, 
		float _range, float _timer_max, int _maxChildren, int _nestingLimit, float _expandTime,
		int _shaderID/*, CellRange* _cellsInRange*/);
	virtual void setTarget(Mob* m);
	virtual void update(Map* map);
	bool isDone();
	void reset();

	void deleteSprite();
	void removeElement(ChainElement* el);
	void addElement(ChainElement* el);
	void removeTarget(Mob* m);
	void addTarget(Mob* m);

	std::set<Mob*>* getCurrentTargets();
	std::set<Mob*>* getNewTargets();
	std::set<Mob*>* getReleasedTargets();

	void dump();

	void setLazer(bool state) { _isLazer = state; }
	bool isLazer() { return _isLazer; }
protected:
	ChainElement*			baseElement = nullptr;
	std::set<ChainElement*> elements;
	std::set<Mob*>			allTargets;
	std::set<Mob*>			newTargets;
	std::set<Mob*>			releasedTargets;
	bool _isLazer = false;
	//std::vector<Cell*>*		cellsInRange;	
	//CellRange*				cellRange;
	
	void updateChildElements(Map* map);
	void getNewTargetsForChildElement(ChainElement* ce, int nTargets, Map* map);

	void addLazerTargets(ChainElement* ce, Map* map);

	float timer			= 0.0f;
	float timer_max		= 0.0f;
	float range			= 0.0f;
	bool  bulletIsDone  = false;
	bool  firstUpdate   = false;

	int maxChildren = 0;
	int nestingLimit = 0;
	float expandTime = 0.0f;

	int shaderID = 0;
};