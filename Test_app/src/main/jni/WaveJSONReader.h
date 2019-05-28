#pragma once
#include "WaveDescriptor.h"
#include "JSONReader.h"
#include <map>

class WaveJSONReader : public JSONReader {
public:
	WaveJSONReader() {};
	bool readWaves(std::vector<WaveDescriptor>* _waveDescriptors, char* jsonfile, std::string& errmsg);

private:
	std::vector<WaveDescriptor>* waveDescriptors = NULL;
	bool createWaveDescriptor(JSONObject* obj, std::string& errmsg);

	bool parseMobSetVector(std::vector<WaveDescriptor::MobSet>* mobsets, uString mobTypes, uString mobAmounts, std::string& errmsg);

};