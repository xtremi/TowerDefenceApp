#pragma once
#include "Wave.h"
#include "WaveJSONReader.h"

#include "MobController.h"

#define WAVE_DESCRIPTOR_JSON_FILE "json/wave_defs.json"


class WaveController {

public:
	WaveController(){}
	
	void loadWaveDescriptors();

	void setMobStartCell(Cell* cell) { mobStartCell = cell; }
	void update(MobController* mc);
	int hasNewWave();
	float timer() { return _timer; }
	float timeRemaining() { return (currentWaveDuration() - _timer); }

	bool firstWaveHasStarted() {
		return _currentWaveNumber > -1;
	}

	void sendWave(MobController* mc, int waveNumber, Cell* cell);
	void sendNextWave(MobController* mc, Cell* cell);

	WaveDescriptor* getWaveDescriptor(int i) {
		return &waveDescriptors[i];
	}

	WaveDescriptor* getCurrentWaveDescriptor() {
		return &waveDescriptors[_currentWaveNumber];
	}
	WaveDescriptor* getNextWaveDescriptor() {
		if((_currentWaveNumber + 1) > (waveDescriptors.size() - 1))
			return &waveDescriptors[_currentWaveNumber - 1];
		return &waveDescriptors[_currentWaveNumber + 1];
	}

	int currentWaveNumber() {
		return _currentWaveNumber;
	}

	float currentWaveDuration() {
		return waveDescriptors[_currentWaveNumber].duration;
	}

	void goToWaveEnd() {
		_timer = currentWaveDuration() - DELTA_TIME;
	}

private:
	std::vector<WaveDescriptor> waveDescriptors;
	Cell* mobStartCell = NULL;
	int _currentWaveNumber = -1;
	float _timer = 0.0f;

	bool _hasNewWave = false;

};