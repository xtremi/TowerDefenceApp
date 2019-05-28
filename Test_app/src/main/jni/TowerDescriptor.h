#pragma once
#include "UI/AgkImage.h"

class AoeStats {
public:
	AoeStats() {}
	float radius = 0.0f;
};

class BulletStats {
public:
	BulletStats() {}
	float width = 0.0f;
	float heigth = 0.0f;
	float damage = 0.0f;
	float speed = 0.0f;

	bool hasAoe = false;
	AoeStats aoeStats;
	bool followTarget = false;

	AgkImage* bulletImage = NULL;
};

class TowerImages {
public:
	TowerImages() {}
	std::vector<AgkImage*> fixed;
	std::vector<AgkImage*> rotatable;
	std::vector<AgkImage*> buttons;
};

class TowerDescriptor
{
public:
	TowerDescriptor() {}
	int ID = -1;
	uString name;
	TowerImages images;

	bool canRotate = false;

	float range = 0.0f;
	float shoot_speed = 0.0f;
	float rotate_speed = 0.0f;
	float reload_time = 0.0f;

	BulletStats bulletStats;

	std::vector<int> upgradable;

	float price = 0.0f;
};
