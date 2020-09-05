#pragma once
#include "JSONReader.h"
#include "UI/Sprite.h"
#include "global.h"
#include <map>

class MediaJSONReader : public JSONReader {
public:
	MediaJSONReader() {};
	bool readMedia(std::map<std::string, int>* shaders, char* jsonfile, std::string& errmsg);
	
private:
	bool readShaders(std::map<std::string, int>* shaders, char* jsonfile, std::string& errmsg);
};