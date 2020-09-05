#pragma once
#include "UI/AgkImage.h"

/*
	mob_effect_type defines what sort of debuff (mob effect) is used
	if the bullet_hit_effect_type is "debuff"
*/
enum class mob_effect_type {
	none,
	speed,
	dmg_over_time,
	temp_health_reduction
};


class MobDescriptor
{
public:
	MobDescriptor() {}
	int ID = -1;
	uString name;
	AgkImage* image = NULL;

	float health = 0.0f;
	float armor1 = 0.0f;
	float armor2 = 0.0f;
	float armor3 = 0.0f;
	float speed_min = 0.0f;
	float speed_max = 0.0f;
	glm::vec2 size;
	float reward = 0.0f;
};
