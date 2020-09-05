/*#pragma once
#include "AnimationJSONReader.h"



class AnimationController {

public:
	AnimationController(){}
	void loadAnimations() {
		AnimationJSONReader animationReader;
		std::string errmsg = "";
		if (!animationReader.readAnimations(&animSprites, &animImages, ANIMATIONS_JSON_FILE, errmsg)) {
			//handle error:
		}
	}

	void playAnimation(const std::string& id, const glm::vec2& pos, int fps) {
		Sprite* s = animSprites[id];
		s->setPosition(pos);
		s->playSprite((float)fps, 0);
		s->show();
	}
	Sprite* getSprite(const std::string& id) {
		return animSprites[id];
	}

	Sprite* getSpriteCopy(const std::string& id) {
		
	}

private:
	std::map<std::string, Sprite*>					animSprites;
	std::map<std::string, std::vector<AgkImage*>>	animImages;
};*/
