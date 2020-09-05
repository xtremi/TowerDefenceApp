#include "LazerBullet.h"

LazerBullet::LazerBullet(const glm::vec2& pos, float _range, float _timer_max, int _shaderID)
	: Bullet(GETIMG(ASSET::IMG_BLANK_128x128), pos, glm::vec2(_range, LAZER_SPRITE_WIDTH))
{	
	range		= _range;
	timer_max	= _timer_max;
	shaderID	= _shaderID;
	
	agk::SetSpriteShader(id, shaderID);
	setAlignment(AgkHAlign::left, AgkVAlign::center);
}

void LazerBullet::update(Map* map){
	timer += DELTA_TIME;
	updateOrientation();
}

void LazerBullet::updateOrientation() {
	glm::vec2 d = targetMob->getPos() - getPos();	
	//setSize(glm::vec2(range, CHAIN_SPRITE_WIDTH));	
	float ang = agk::ATanFull(d.x, d.y);
	setRotation(ang - 90.0f);
}

bool LazerBullet::isDone(){
	if (!targetMob || (targetMob && targetMob->isDead()))
		return true;
	//if out of range
	return timer > timer_max;
}
void LazerBullet::reset(){
	timer = 0.0f;
	targetMob = nullptr;
}