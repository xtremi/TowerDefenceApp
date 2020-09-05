#pragma once
#include "InfoFrame.h"
#include "global.h"
#include "MediaBank.h"
#include <iostream>
#include <map>


class InfoFrameList : public InfoFrame {
public:
	InfoFrameList(const glm::vec2& size, int depth, int headerCorner = 0, char* name = NULL)
	 : InfoFrame(size, depth, headerCorner, name){	};

public:
	void setListItemValue(const uString& key, const uString& val);
	void setListItemValue(const uString& key, float val, int prec = 2);	
	void setListItemValue(const uString& key, int val);
	void setListItemValue(const uString& key, bool val);
	void setFixedSizeBackground(bool fixed = true) {
		fixedSizeBackground = fixed;
	}

protected:
	/*struct LoggedItem {
		TextSprite* nameSpr = NULL;
		TextSprite* valueSpr = NULL;
	};*/
	bool fixedSizeBackground = false;

	glm::vec2 txtOffset = glm::vec2(12.5f); //textOffset.x is the offset before the name text / textOffset.y is the y offset from background top to first item
	float textSize = 15.0f;	
	float dx1 = 120.0f;						//dx1 is the offset from the beginning of name text to the beg of value text
	float dx2 = 60.0f;						//dx2 is the offset from the begining of value to edge to background
	float dy1 = 10.0f;						//dy1 is the offset between each listem item
	float dy2 = 10.0f;						//dy2 is the offset between the last item and the background bottom edge
	int rows = 0;
	glm::vec3 nameTextColor = colours::cyan;
	glm::vec3 valueTextColor = colours::white;
	glm::vec3 backgroundColor = glm::vec3(0.0f);
	float backgroundAlpha = 0.5f;

	virtual void addListItem(const uString& name);
	void resizeBackground();
	//std::map<const char*, LoggedItem> dataMap;


	
};

