#pragma once
#include "JSONReader.h"
#include "TowerDescriptor.h"
#include "MediaBank.h"
#include <map>

bool					getChainDebuffReleaseStateFromString(const std::string& typestr);
bool					getChainDebuffReleaseStateFromString(const uString& typestr);
bullet_effect_vis_type	getBulletEffectVisualizationFromString(const uString& typestr);
bullet_hit_effect_type	getBulletHitEffectFromString(const uString& typestr);
mob_effect_type			getMobEffectTypeFromString(const uString& typestr);
mob_effect_type			getMobEffectTypeFromString(const std::string& typestr);
bullet_type				getBulletTypeFromString(const uString& typestr);
tower_type				getTowerTypeFromString(const uString& typestr);
area_spawn_bullet_type	getAreaSpawnBulletType(const uString& typestr);
area_spawn_bullet_type	getAreaSpawnBulletType(const std::string& typestr);
int						towerIDstringToInt(const std::string& idstr);
bool					getBoolFromString(const std::string& str);


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
	bool readTowerMainData(TowerDescriptor* towerDescr, JSONObject* obj, std::string& errmsg);
	bool readTowerImageData(TowerDescriptor* towerDescr, JSONObject* obj, std::string& errmsg);
	bool readTowerBulletData(TowerDescriptor* towerDescr, JSONObject* obj, std::string& errmsg);


	bool getTowerDescriptorImagePaths(JSONObject* obj,
		std::vector<uString>* fixedImagePaths,
		std::vector<uString>* rotImagePaths,
		std::vector<uString>* iconImagePaths,
		std::string& errmsg);

	void loadTowerDescriptorImages(
		TowerImages*		  twrImages, 
		const uString&		  towerName,
		std::vector<uString>* fixedImagePaths,
		std::vector<uString>* rotImagePaths,
		std::vector<uString>* iconImagePaths
	);
};