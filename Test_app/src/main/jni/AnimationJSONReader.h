#pragma once
#include "JSONReader.h"
#include "UI/Sprite.h"
#include "global.h"
#include <map>

class AnimationJSONReader : public JSONReader {
public:
	AnimationJSONReader() {};
	bool readAnimations(
		std::map<std::string, Sprite*>* animSprites, 
		std::map<std::string, std::vector<AgkImage*>>* animImages,
		char* jsonfile, std::string& errmsg);
	
private:
	bool createAnimationSprite(
		std::map<std::string, Sprite*>* animSprites,
		std::map<std::string, std::vector<AgkImage*>>* animImages,
		JSONObject* obj, std::string& errmsg);
	
	Sprite* loadImagesAndCreateAnimationSprite(
		std::vector<AgkImage*>* imgs, const std::string &prefix, 
		int start_n, int end_n, int npadded_0);

	Sprite* loadImageAndCreationAnimationSprite(
		std::vector<AgkImage*>* imgs, const std::string& path,
		int w, int h, int count
	);

};