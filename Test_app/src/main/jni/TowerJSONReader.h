#pragma once
#include "JSONReader.h"
#include "TowerDescriptor.h"
#include "MediaBank.h"
#include <map>

class TowerJSONReader : public JSONReader {
public:
	TowerJSONReader() {};
	bool readTowers(std::map<int, TowerDescriptor>* _towerDescriptors, char* jsonfile, std::string& errmsg);
	bool readBullets(char* jsonfile, std::string& errmsg);

private:
	std::map<int, TowerDescriptor>* towerDescriptors = NULL;

	bool loadBulletImage(JSONObject* obj, std::string& errmsg);

	bool checkTowerImagesJSONobject(JSONObject* el, std::string& errmsg);

	bool createTowerDescriptor(JSONObject* obj, std::string& errmsg);
	bool getTowerDescriptorImagePaths(JSONObject* obj,
		std::vector<uString>* fixedImagePaths,
		std::vector<uString>* rotImagePaths,
		std::vector<uString>* buttonsImagePaths,
		std::string& errmsg);
	void loadTowerDescriptorImages(TowerImages* twrImages, const uString& towerName,
		std::vector<uString>* fixedImagePaths,
		std::vector<uString>* rotImagePaths,
		std::vector<uString>* buttonsImagePaths);
};