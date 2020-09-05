#pragma once
#include "InfoFrame.h"
#include "UI/NumberTextSprite.h"
#include "colours.h"
#include "MediaBank.h"	

namespace player_stats_hud 
{

}

class PlayerStatsHUDframe : public InfoFrame
{
public:
	PlayerStatsHUDframe(glm::vec2 size, int depth) : InfoFrame(size, depth, 0, "Player stats") {
		bgsize = size;
		createLifeSprites(depth - 1);
		createMoneySprites(depth - 1);
		createWaveSprites(depth - 1);
	}

	void createLifeSprites(int depth);
	void createMoneySprites(int depth);
	void createWaveSprites(int depth);

	void setLife(int life);
	void setMoney(int money);
	void setWave(int wave);

private:
	glm::vec2 bgsize;

	TextSprite* moneySprite = NULL;
	TextSprite* lifeSprite = NULL;
	TextSprite* waveSprite = NULL;

	const float moneyIconSize = 25.0f;
	const float moneyTextSize = 30.0f;
	const float lifeIconSize = 25.0f;
	const float lifeTextSize = 30.0f;
	const float waveTextSize = 18.0f;
	const float waveNumbTextSize = 30.0f;

};

