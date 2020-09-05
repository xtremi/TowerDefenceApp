#include "WaveHUDframe.h"

void WaveHUDframe::createNextWaveButton(const glm::vec2& pos, int depth) {
	std::string id = "button-next-wave";
	nextWaveButton = new NextWaveButton(glm::vec2(NEXT_WAVE_BUTTON_SIZE));
	nextWaveButton->setDepth(depth);
	nextWaveButton->fixToScreen();
	nextWaveButton->setScreenMode();
	addButton(nextWaveButton, id, pos);
	nextWaveButton->startBlink();
}

void WaveHUDframe::addMobSprite(const glm::vec2& pos, const glm::vec2& size,  int depth, const std::string& id) {
	Sprite* c = new Sprite(noMobImage, glm::vec2(0.0f), size, AgkHAlign::left, AgkVAlign::center);
	c->setDepth(depth);
	c->fixToScreen();
	//c->hide();
	addSprite(c, id, pos);	
}

void WaveHUDframe::addMobTextSprite(const glm::vec2& pos, float txtSize, int depth, const std::string& id) {
	TextSprite* cTxt = new TextSprite(" ", colours::white, glm::vec2(0.0f), /*mobIconSize.y * 1.1f*/txtSize, AgkHAlign::left, AgkVAlign::center);
	cTxt->setFont(MEDIA_BANK.getFont(ASSET::FONT_COUR));
	cTxt->setDepth(depth);
	cTxt->fixToScreen();
	//cTxt->hide();	
	addSprite(cTxt, id, pos);	
}

void WaveHUDframe::initMobSprites(int depth) 
{	
	noMobImage			= new AgkImage(1.0f, 1.0f, 1.0f, 0.0f);

	float ytop			= 35.0f;
	float ybot			= bgsize.y;
	float y_length		= ybot - ytop;
	float dy			= y_length / (float)(n_mobs_per_col);

	float xleft			= 5.0f;
	float xright		= bgsize.x * 0.70f;
	float x_length		= xright - xleft;
	float dX			= x_length / 2.0f;
	float dx			= 0.8f*dX / (float)n_col;

	createWaveDescriptorHeaders(depth, glm::vec2(xleft, 2.0f), dX);

	glm::vec2 mobIconSize(dy * 0.75f);
	mobSpriteSizeY = mobIconSize.y;
	float dx_text  = mobIconSize.x + 2.0f;

	glm::vec2 posCurrentMobsIcon(xleft, ytop);
	glm::vec2 posNextMobsIcon = posCurrentMobsIcon + glm::vec2(dX, 0.0f);
	
	std::string spriteIDcurrent = "";
	std::string spriteIDnext    = "";

	int mobIndex = 1;
	for (int i = 0; i < n_mobs_per_col; i++) {

		for(int j = 0; j < n_col; j++){
			spriteIDcurrent = "c1_" + std::to_string(mobIndex);
			spriteIDnext	= "c2_" + std::to_string(mobIndex);
			addMobSprite(posCurrentMobsIcon, mobIconSize, depth, spriteIDcurrent);
			addMobSprite(posNextMobsIcon, mobIconSize, depth, spriteIDnext);
			 
			addMobTextSprite(posCurrentMobsIcon + glm::vec2(dx_text, 0.0f), mobIconSize.y * 1.1f, depth, "txt_" + spriteIDcurrent);
			addMobTextSprite(posNextMobsIcon + glm::vec2(dx_text, 0.0f), mobIconSize.y * 1.1f, depth, "txt_" + spriteIDnext);

			posCurrentMobsIcon += glm::vec2(dx, 0.0f);
			posNextMobsIcon    += glm::vec2(dx, 0.0f);

			mobIndex++;
		}

		posCurrentMobsIcon += glm::vec2(-(float)n_col*dx, dy);
		posNextMobsIcon	   += glm::vec2(-(float)n_col*dx, dy);
	}


}

void WaveHUDframe::createWaveDescriptorHeaders(int depth, const glm::vec2& pos, float dX) 
{
	static const float textsize = 13.5f;
	TextSprite* cur = new TextSprite("Current wave", colours::white, glm::vec2(0.0f), textsize);
	cur->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	cur->setBold(true);
	cur->setDepth(depth);
	cur->fixToScreen();

	TextSprite* next = new TextSprite("Next wave", colours::white, glm::vec2(0.0f), textsize);
	next->setFont(MEDIA_BANK.getFont(ASSET::FONT_MAIN));
	next->setBold(true);
	next->setDepth(depth);
	next->fixToScreen();

	addSprite(cur,  "header-c", pos);
	addSprite(next, "header-n", pos + glm::vec2(dX, 0.0f));
}

void WaveHUDframe::setNextWaveMobInfo(WaveDescriptor* nextWaveDescr, MobController* mobController, bool swapCurrentAndNext)
{
	std::string colID = col1isCurrent ? "c2_" : "c1_";

	if (swapCurrentAndNext)
		setNextMobsAsCurrent();

	for (int i = 0; i < nextWaveDescr->mobSets.size(); i++) {
		
		int mobAmount = nextWaveDescr->mobSets[i].amount;
		int mobType   = nextWaveDescr->mobSets[i].type;

		AgkImage* img = mobController->getMobDescriptor(mobType)->image;

		std::string spriteID = colID + std::to_string(i + 1);

		Sprite* mobSprite = (Sprite*)sprites[spriteID];
		mobSprite->setImage(img);
		mobSprite->setSize(glm::vec2(mobSpriteSizeY, -1));
		
		((TextSprite*)sprites["txt_" + spriteID])->setText(agk::Str(mobAmount));
	}

//	if(isCurrent)
//		setNextMobsAsCurrent();

	col1isCurrent = !col1isCurrent;
}


void WaveHUDframe::setNextMobsAsCurrent() 
{	
	std::string colIDn = col1isCurrent ? "c2_" : "c1_";
	std::string colIDc = col1isCurrent ? "c1_" : "c2_";

	std::string mobSpriteIDc, mobSpriteIDn, txtSpriteIDc, txtSpriteIDn;
	for (int i = 0; i < n_mobs_per_col * n_col; i++) {
		mobSpriteIDc = colIDc + std::to_string(i + 1);
		mobSpriteIDn = colIDn + std::to_string(i + 1);
		txtSpriteIDc = "txt_" + mobSpriteIDc;
		txtSpriteIDn = "txt_" + mobSpriteIDn;
		
		Sprite*			mc    = ((Sprite*)sprites[mobSpriteIDc]);
		Sprite*			mn	  = ((Sprite*)sprites[mobSpriteIDn]);
		TextSprite*		mctxt = (TextSprite*)sprites[txtSpriteIDc];
		TextSprite*		mntxt = (TextSprite*)sprites[txtSpriteIDn];

		glm::vec2 posMn    = mn->getPos();
		glm::vec2 posMnTxt = mntxt->getPos();

		mn->setPosition(mc->getPos());
		mntxt->setPosition(mctxt->getPos());
		mntxt->realign();
		mc->setPosition(posMn);
		mctxt->setPosition(posMnTxt);
		mctxt->realign();

		mn->setImage(noMobImage);
		mntxt->setText(" ");

	}

}

/*
void WaveHUDframe::resetMobSprites()
{
	std::string mobSpriteIDc, mobSpriteIDn, txtSpriteIDc, txtSpriteIDn;

	for (int i = 0; i < n_mobs_per_col * n_col; i++) {

		mobSpriteIDc = "m_c_" + std::to_string(i);
		mobSpriteIDn = "m_n_" + std::to_string(i);
		txtSpriteIDc = "txt_" + mobSpriteIDc;
		txtSpriteIDn = "txt_" + mobSpriteIDn;

		((Sprite*)sprites["m_n_" + std::to_string(i + 1)])->setImage(noMobImage);
		((TextSprite*)sprites["txt_m_n_" + std::to_string(i + 1)])->setText("0");
	}
}
*/