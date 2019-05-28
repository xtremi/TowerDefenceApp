#include "InfoFrame.h"

InfoFrame::InfoFrame(const glm::vec2& size, int headerCorner, char* name) : UIFrame() {

	static const float headerWidth = 140.0f;
	static const float headerHeight = 12.0f;
	static const float textSize = 11.0f;

	glm::vec2 headerPos = glm::vec2(0.0f, -headerHeight);
	float headerRot = 0.0f;
	if (headerCorner == 1) {
		headerPos = glm::vec2(0.0f, 0.0f);
		headerRot = 90.0f;
	}

	Sprite* bg = new Sprite(glm::vec3(0), glm::vec2(0), size, AgkHAlign::left, AgkVAlign::top);
	UISpriteButton_T2* top = new UISpriteButton_T2(
		glm::vec3(1, 0, 0), glm::vec3(0.7, 0, 0.7), glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec2(headerWidth, headerHeight), glm::vec2(0), UIButtonType::press_and_release);
	top->setAlignment(AgkHAlign::left, AgkVAlign::top);
	top->setRotation(headerRot);
	top->setScreenMode();

	bg->setColorAlpha(0.65f);
	top->setColorAlpha(0.85f);
	bg->setDepth(3);
	top->setDepth(3);
	bg->fixToScreen();
	top->fixToScreen();

	addSprite(bg, "bg", glm::vec2(0.0f, 0.0f));
	addButton(top, "top", headerPos);

	if (name) {
		TextSprite* nameSprite = new TextSprite(name, glm::vec3(1), glm::vec2(0), textSize);
		nameSprite->setRotation(-headerRot);
		nameSprite->setDepth(2);
		nameSprite->fixToScreen();
		if (headerCorner == 1)
			headerPos += glm::vec2(-headerHeight, headerWidth);

		addSprite(nameSprite, "name", headerPos);
	}

}

void InfoFrame::onAnimationEnd() {
	if (hideOnDefault && this->currentToStateIndex == 0) {
		this->hide();
		this->disable();
	}
}

glm::vec2 InfoFrame::anim_getPos() {
	return this->getPosition();
}
void InfoFrame::anim_setPos(const glm::vec2& _pos) {
	this->setLocalPosition(_pos);
}
bool InfoFrame::isHoovered() {
	return (this->sprites["bg"]->isHoovered() || this->sprites["top"]->isHoovered());

}
