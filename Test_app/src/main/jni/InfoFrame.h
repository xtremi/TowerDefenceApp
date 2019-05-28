#pragma once
#include "UI/UIFrame.h"
#include "UI/SpriteAnimation.h"
#include "UI/UIButtonTypes.h"
#include "glm/glm.hpp"

class InfoFrame : public UIFrame, public SpriteAnimation
{
public:
	InfoFrame(const glm::vec2& size, int headerCorner = 0, char* name = NULL);

	bool isHoovered();
	void setHideOnDefaultState() {
		hideOnDefault = true;
	}

protected:
	bool hideOnDefault = false;

	glm::vec2 anim_getPos();
	void anim_setPos(const glm::vec2& pos);
	float anim_getRot() { return 0.f; }
	void anim_setRot(float r) {}
	float anim_getScale() { return 1.0f; }
	void anim_setScale(float s) {}
	void postProcessAnim() { realignSprites(); }
	void onAnimationEnd();
};
