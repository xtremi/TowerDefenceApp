#pragma once
#include "MobDescriptor.h"
#include "JSONReader.h"
#include "MediaBank.h"
#include <map>

class MobJSONReader : public JSONReader {
public:
	MobJSONReader() {};
	bool readMobs(std::map<int, MobDescriptor>* _mobDescriptors, char* jsonfile, std::string& errmsg);
	
private:
	std::map<int, MobDescriptor>* mobDescriptor = NULL;
	bool createMobDescriptor(JSONObject* obj, std::string& errmsg);


};