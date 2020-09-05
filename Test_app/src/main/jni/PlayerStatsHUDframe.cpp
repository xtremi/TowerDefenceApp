#include "PlayerStatsHUDframe.h"

using namespace player_stats_hud;

void PlayerStatsHUDframe::createLifeSprites(int depth) {

	Sprite* lifeIcon = new Sprite(GETIMG(ASSET::IMG_HEART), glm::vec2(0), glm::vec2(lifeIconSize), AgkHAlign::left, AgkVAlign::bottom);
	lifeIcon->setDepth(depth);
	lifeIcon->fixToScreen();

	glm::vec2 lifeIconPos(10.0f, bgsize.y - 5.0f);
	addSprite(lifeIcon, "heart", lifeIconPos, true);

	lifeSprite = new TextSprite("100", colours::white, glm::vec2(0.0f), lifeTextSize);
	lifeSprite->setBold(true);
	lifeSprite->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	lifeSprite->setDepth(depth);
	lifeSprite->fixToScreen();
	lifeSprite->setAlignment(AgkHAlign::left, AgkVAlign::bottom);

	glm::vec2 lifeTextPos = lifeIconPos + glm::vec2(lifeIconSize + 10.0f, 0);
	addSprite(lifeSprite, "life", lifeTextPos, true);

}
void PlayerStatsHUDframe::createMoneySprites(int depth) {

	Sprite* moneyIcon = new Sprite(GETIMG(ASSET::IMG_COIN), glm::vec2(0), glm::vec2(moneyIconSize), AgkHAlign::left, AgkVAlign::bottom);
	moneyIcon->setDepth(depth);
	moneyIcon->fixToScreen();

	glm::vec2 moneyIconPos(bgsize.x - 120.0f, bgsize.y - 5.0f);
	addSprite(moneyIcon, "coin", moneyIconPos, true);

	moneySprite = new TextSprite(" ", colours::white, glm::vec2(0.0f), moneyTextSize);
	moneySprite->setBold(true);
	moneySprite->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	moneySprite->setDepth(depth);
	moneySprite->fixToScreen();
	moneySprite->setAlignment(AgkHAlign::left, AgkVAlign::bottom);

	glm::vec2 moneyTextPos = moneyIconPos + glm::vec2(moneyIconSize + 10.0f, 0);
	addSprite(moneySprite, "money", moneyTextPos, true);
}

void PlayerStatsHUDframe::createWaveSprites(int depth) {
	TextSprite* waveIcon = new TextSprite("WAVE", colours::cyan, glm::vec2(0.0f), waveTextSize, AgkHAlign::left, AgkVAlign::bottom);
	waveIcon->setDepth(depth);
	waveIcon->fixToScreen();
	waveIcon->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	waveIcon->setTextSpacing(-3.5f);
	waveIcon->setBold(true);

	glm::vec2 waveIconPos(bgsize.x/2.0f - 65.0f, bgsize.y - 8.0f);
	addSprite(waveIcon, "wave", waveIconPos, true);

	waveSprite = new TextSprite(" ", colours::white, glm::vec2(0.0f), waveNumbTextSize, AgkHAlign::left, AgkVAlign::bottom);
	waveSprite->setBold(true);
	waveSprite->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	waveSprite->setDepth(depth);
	waveSprite->fixToScreen();
	
	glm::vec2 waveTextPos = waveIconPos + glm::vec2(55.0f, 3.0f);
	addSprite(waveSprite, "waveNumber", waveTextPos, true);

	setWave(0);
}


void PlayerStatsHUDframe::setLife(int life) {
	lifeSprite->setText(agk::Str(life));
}
void PlayerStatsHUDframe::setMoney(int money) {
	moneySprite->setText(agk::Str(money));
}
void PlayerStatsHUDframe::setWave(int wave) {
	std::string waveStr = std::to_string(wave);
	if (wave < 10) waveStr = "0" + waveStr;
	if(wave < 100) waveStr = "0" + waveStr;
	waveSprite->setText(&waveStr[0]);
}