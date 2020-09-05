#pragma once
#include "ChainBullet.h"

static const float TIME_BEFORE_CHAIN_EXPAND = 0.5f;

class ChainBullet;

class ChainElement : public Sprite {
public:
	ChainElement(ChainBullet* _parentBullet, ChainElement* _parentElement, 
		Mob* _target, Sprite* _source, 
		float _range, int _nestedLevel, 
		int _maxChildren, int _nestingLimit, float _expandTime,
		int _shaderID);
	void update();
	bool isDone();
	bool isDisabled();

	void addNewChainTarget(Mob* m);
	void removeChainElementChild(ChainElement* ce);
	int needTargets();
	Mob* getTarget() { return target; }

	ChainElement* getParentElement();

	void resetExpandTimer();
	void disable();
	void dump();

protected:
	std::set<ChainElement*>	    children;
	Mob*						target = nullptr;
	Sprite*						source = nullptr;
	ChainBullet*				parentBullet = nullptr;
	ChainElement*				parentElement = nullptr;

	float range = 0.0f;
	float chain_length = 0.0f;
	float time_until_expand = TIME_BEFORE_CHAIN_EXPAND;
	float time = 0.0f;
	int   maxChildren = 2;
	bool  hasExpanded = false;
	bool  disabled = false;
	bool  needNextTargets = false;
	int	  nestedLevel = 0;
	int	  shaderID = 0;
	int	  maxNestedLevel = 0;

	void updateOrientationAndLength(const glm::vec2& p1, const glm::vec2& p2);
};