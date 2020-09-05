#pragma once
#include <iostream>
#include <map>
#include "UI/TextSprite.h"
#include "UI/Sprite.h"

class OnScreenLogger {
public:
	OnScreenLogger() {
		background = new Sprite(backgroundColor, pos, glm::vec2(1.0f), AgkHAlign::left, AgkVAlign::top);
		background->setColorAlpha(backgroundAlpha);
	};

public:
	void setItemValue(const std::string& key, const std::string& val);
	void setItemValue(const std::string& key, float val, int prec = 2);
	void setItemValue(const std::string& key, int val);
	void setItemValue(const std::string& key, UINT val);
	void setItemValue(const std::string& key, bool val);

	void hide();
	void show();
	bool isVisible() { return state; }
	void setPosition(const glm::vec2& pos);
	void process();

private:
	bool state = true;
	void setVisibleState(bool _state);
	struct LoggedItem {
		TextSprite* nameSpr = NULL;
		TextSprite* valueSpr = NULL;
		TextSprite* timerSpr = NULL;
		float timer = 0.0f;
		bool flashed = false;

		void setState(bool state) {
			nameSpr->setState(state);
			valueSpr->setState(state);
			timerSpr->setState(state);
		}

	};
	Sprite* background = NULL;

	std::map<std::string, LoggedItem> dataMap;
	glm::vec2 txtOffset = glm::vec2(10.0f);
	float textSize = 15.0f;
	float dx1 = 120.0f;
	float dx2 = 60.0f;
	float dx3 = 60.0f;
	float dy1 = 10.0f;
	float dy2 = 10.0f;
	int rows = 0;

	glm::vec3 textColor = glm::vec3(1.0f);
	glm::vec3 backgroundColor = glm::vec3(0.0f);
	float backgroundAlpha = 0.5f;

	glm::vec2 pos = glm::vec2(0.0f);

	void addItem(const std::string& name);
	void setPosition(const std::string& name, int row);
	
	void resizeBackground();

	
};

