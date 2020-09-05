#include "WaveController.h"

void WaveController::update(MobController* mc) {
	
	_timer += DELTA_TIME;
	_hasNewWave = false;
	if (_timer > currentWaveDuration()) {
		sendNextWave(mc, mobStartCell);
		_timer = 0.0f;
		_hasNewWave = true;
	}
}

int WaveController::hasNewWave() {
	if (_hasNewWave)
		return _currentWaveNumber;
	return 0;
}

void WaveController::loadWaveDescriptors() {
	WaveJSONReader waveReader;
	std::string errmsg = "";
	if (!waveReader.readWaves(&waveDescriptors, WAVE_DESCRIPTOR_JSON_FILE, errmsg)) {
		//handle error:
	}
}

void  WaveController::sendNextWave(MobController* mc, Cell* cell) {
	sendWave(mc, _currentWaveNumber + 1, cell);
}
void WaveController::sendWave(MobController* mc, int waveNumber, Cell* cell) {

	_currentWaveNumber = waveNumber;
	if (_currentWaveNumber >= waveDescriptors.size())
		_currentWaveNumber = waveDescriptors.size() - 1;

	WaveDescriptor* wd = &waveDescriptors[_currentWaveNumber];

	for (WaveDescriptor::MobSet ms : wd->mobSets) {
		mc->addMobs(cell, ms.amount, ms.type);
	}

	mc->setReleaseInterval(wd->release_time);
}