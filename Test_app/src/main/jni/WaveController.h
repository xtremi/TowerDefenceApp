#pragma once
#include "Wave.h"
#include "WaveJSONReader.h"

#include "MobController.h"

#define WAVE_DESCRIPTOR_JSON_FILE "json/wave_defs.json"
#define DELTA_TIME 0.01f

class WaveController {

public:
	WaveController(){}
	
	void loadWaveDescriptors();

	void setMobStartCell(Cell* cell) { mobStartCell = cell; }
	void update(MobController* mc);
	int hasNewWave();
	float timer() { return _timer; }
	float timeRemaining() { return (currentWaveDuration() - _timer); }


	void sendWave(MobController* mc, int waveNumber, Cell* cell);
	void sendNextWave(MobController* mc, Cell* cell);

	int currentWaveNumber() {
		return _currentWaveNumber;
	}

	float currentWaveDuration() {
		return waveDescriptors[_currentWaveNumber].duration;
	}

private:
	std::vector<WaveDescriptor> waveDescriptors;
	Cell* mobStartCell = NULL;
	int _currentWaveNumber = -1;
	float _timer = 0.0f;

	bool _hasNewWave = false;

};