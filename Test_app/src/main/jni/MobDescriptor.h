#pragma once
#include "UI/AgkImage.h"

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
