#pragma once
#include "UI/AgkImage.h"
#include <vector>

class WaveDescriptor
{
public:
	struct MobSet{
		int amount = 0;
		int type = 0;
	};

public:
	WaveDescriptor() {}

	uString name;
	int ID;
	float duration = -1.0f;
	float release_time = -1.0f;
	std::vector<MobSet> mobSets;
};