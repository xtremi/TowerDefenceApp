#pragma once
#include "InfoFrame.h"
#include <iostream>
#include <map>


class InfoFrameList : public InfoFrame {
public:
	InfoFrameList(const glm::vec2& size, int headerCorner = 0, char* name = NULL)
	 : InfoFrame(size, headerCorner, name){	};

public:
	void setListItemValue(const uString& key, const uString& val);
	void setListItemValue(const uString& key, float val, int prec = 2);
	void setListItemValue(const uString& key, int val);
	void setListItemValue(const uString& key, bool val);

private:
	/*struct LoggedItem {
		TextSprite* nameSpr = NULL;
		TextSprite* valueSpr = NULL;
	};*/

	glm::vec2 txtOffset = glm::vec2(10.0f);
	float textSize = 15.0f;
	float dx1 = 120.0f;
	float dx2 = 60.0f;
	float dy1 = 10.0f;
	float dy2 = 10.0f;
	int rows = 0;
	glm::vec3 textColor = glm::vec3(0.22f, 1.0f, 0.8f);
	glm::vec3 backgroundColor = glm::vec3(0.0f);
	float backgroundAlpha = 0.5f;

	void addListItem(const uString& name);
	void resizeBackground();
	//std::map<const char*, LoggedItem> dataMap;
	
	

	
};

