#include "OnScreenLogger.h"

void OnScreenLogger::resizeBackground() {
	glm::vec2 size;
	size.x = txtOffset.x + dx1 + dx2 + dx3;
	size.y = txtOffset.y + dy1 * rows + dy2;
	background->setSize(size);
	background->fixToScreen();
	background->setDepth(4);
}

void OnScreenLogger::setPosition(const std::string& name, int row) {
	float ypos = pos.y + dy1 * (float)row + txtOffset.y;
	dataMap[name].nameSpr->setPosition(pos.x + txtOffset.x, ypos);
	dataMap[name].valueSpr->setPosition(pos.x + dx1 + txtOffset.x, ypos);
	dataMap[name].timerSpr->setPosition(pos.x + dx1 + dx2 + txtOffset.x, ypos);

	dataMap[name].nameSpr->realign();
	dataMap[name].valueSpr->realign();
	dataMap[name].timerSpr->realign();
}

void OnScreenLogger::setVisibleState(bool _state) {
	state = _state;
	for (std::map<std::string, LoggedItem>::iterator it = dataMap.begin(); it != dataMap.end(); it++)
		it->second.setState(state);
	background->setState(state);
}

void OnScreenLogger::hide() {
	setVisibleState(false);
}
void OnScreenLogger::show() {
	setVisibleState(true);
}

void OnScreenLogger::setItemValue(const std::string& name, const std::string& val) {
	if (dataMap.find(name) == dataMap.end()) {
		addItem(name);
	}
	std::string vstr = val;
	std::string prevString = dataMap[name].valueSpr->getText();

	if (vstr != prevString){
		dataMap[name].timer = 0.0f;
	}
	dataMap[name].valueSpr->setText(&vstr[0]);
	
	if (!state)
		dataMap[name].setState(false);
}
void OnScreenLogger::setItemValue(const std::string& name, float val, int prec) {
	std::string str = agk::Str(val, prec);
	setItemValue(name, str);
}
void OnScreenLogger::setItemValue(const std::string& name, int val) {
	std::string str = agk::Str(val);
	setItemValue(name, str);
}
void OnScreenLogger::setItemValue(const std::string& name, UINT val) {
	setItemValue(name, (int)val);
}
void OnScreenLogger::setItemValue(const std::string& name, bool val) {
	std::string str = agk::Str(val);
	setItemValue(name, str);
}
void OnScreenLogger::addItem(const std::string& name) {
	if (dataMap.find(name) == dataMap.end()) {
		LoggedItem newitem;
		dataMap[name] = newitem;
		std::string nstr = name;
		dataMap[name].nameSpr = new TextSprite(&nstr[0], textColor, glm::vec2(0), textSize);
		dataMap[name].valueSpr = new TextSprite("", textColor, glm::vec2(0), textSize);
		dataMap[name].timerSpr = new TextSprite("", textColor, glm::vec2(0), textSize);

		dataMap[name].nameSpr->fixToScreen();
		dataMap[name].valueSpr->fixToScreen();
		dataMap[name].timerSpr->fixToScreen();

		dataMap[name].nameSpr->setDepth(3);
		dataMap[name].valueSpr->setDepth(3);
		dataMap[name].timerSpr->setDepth(3);
		
		setPosition(name, rows);
		rows++;
		resizeBackground();
	}
}

void OnScreenLogger::setPosition(const glm::vec2& _pos) {
	pos = _pos;
	background->setPosition(pos);

}

void OnScreenLogger::process() {

	if(state){
		for (std::map<std::string, LoggedItem>::iterator it = dataMap.begin(); it != dataMap.end(); it++)
		{
			it->second.timer += 0.1f;
			it->second.timerSpr->setText(agk::Str(it->second.timer, 2));
		
			if(it->second.timer < 0.8f){
				it->second.nameSpr->setColor(glm::vec3(1, 1, 0));
				it->second.valueSpr->setColor(glm::vec3(1, 1, 0));
				it->second.timerSpr->setColor(glm::vec3(1, 1, 0));
				it->second.flashed = true;
			}
			else if (it->second.flashed && it->second.timer >= 0.8f) {
				it->second.nameSpr->setColor(textColor);
				it->second.valueSpr->setColor(textColor);
				it->second.timerSpr->setColor(textColor);
				it->second.flashed = false;
			}
		}
	}
}