#pragma once
#include "WaveDescriptor.h"

class Wave {
public:
	Wave(WaveDescriptor* _waveDescriptor){}

private:
	WaveDescriptor* waveDescriptor = NULL;
};