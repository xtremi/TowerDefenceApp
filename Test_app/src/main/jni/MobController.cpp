#include "MobController.h"

void MobController::addMobs(Cell* c, int amount, int type)
{
	glm::vec2 startpos = c->posIn();
	for (int i = 0; i < amount; i++) {
		Mob* m = new Mob(&mobDescriptors[type], startpos);
		m->disable();
		m->hide();
		mobQueue.push_back(m);
	}
}


void MobController::update(Map* map)
{
	timer += TIMER_STEP;
	numberOfMobsReadEnd = 0;
	
	if (timer > currentReleaseInterval) {
		timer = 0.0f;
		
		if(mobQueue.size() > 0){
			Mob* m = mobQueue[0];
			mobQueue.pop_front();

			m->enable();
			m->show();
			activeMobs.push_back(m);
		}		
	}
		
	map->clearCellsContainingMobs();
	int counter = 0;
	
	for (Mob* m : activeMobs) { //TODO: add another std::deque of dead mobs?

		if (!m->isDead())
		{
			Cell* c = map->getCellAtLocation(m->getPos());
			if (c == map->getMobEndCell()) {
				m->die();
				numberOfMobsReadEnd++;
			}
			else {
				c->addMob(m);
				map->addCellContainingMobs(c);

				td::direction movedir = c->getMoveDirection(m->pathPos());
				
				float y = 1.0;
				/*if(false){//set true to use offset created by perlin noise algh.
					float d = m->travelledDistance();
					d = fmodf(d, (float)offsetPNvalues.size());
					int dposi1 = (int)d;
					int dposi2 = d + 1;
					if (dposi1 == offsetPNvalues.size())
						dposi1 = 0;
					if (dposi2 == offsetPNvalues.size())
						dposi2 = 0;
					float y1 = offsetPNvalues[dposi1];
					float y2 = offsetPNvalues[dposi2];
					float dd = d - (int)d;
					float y = (y1 * (1.0f - dd) + y2 * dd);
				}*/

				m->update(movedir, y*1.0f);
				m->setQueueIndex(counter);

				uString str = "";
				m->setDebugText(str.AppendInt(counter++));
			}
		}
	}

	//remove dead:
	for (auto m = activeMobs.begin(); m != activeMobs.end();) {
		if ((*m)->isDead()) {
			MEDIA_BANK.playAnimation("white-explode", (*m)->getPos(), 70);
			
			(*m)->deleteSprite();
			delete (*m);
			m = activeMobs.erase(m); //TODO: is this ok when m was deleted?
		}
		else
			++m;
	}

	/*offsetPNiter++;
	if (offsetPNiter == offsetPNvalues.size())
		offsetPNiter = 0;*/
}

void MobController::hide()
{
	for (Mob* m : activeMobs)
		m->hide();
}

void MobController::show()
{
	for (Mob* m : activeMobs)
		m->show();
}

void MobController::loadMobDescriptors() {
	MobJSONReader mobReader;
	std::string errmsg = "";
	if (mobReader.readMobs(&mobDescriptors, MOB_DESCRIPTOR_JSON_FILE, errmsg)) {
		//handle error:
	}
	//loadExplosionAnimations();
}
/*void MobController::loadExplosionAnimations() {
	std::string explPath = "images/explosions/";

	std::string whExplPrefix = explPath + "white-explode/explodewhite";
	std::vector<AgkImage*> whExplImgs;
	int whExplNstart = 3, whExplNend = 46;
	for (int i = whExplNstart; i <= whExplNend; i++) {
		std::string path_i = whExplPrefix + padZeros_stdstr(i, 4) + ".png";
		AgkImage* img = new AgkImage(&path_i[0]);
		whExplImgs.push_back(img);
	}
	explosionImages["white-explosion"] = whExplImgs;

	Sprite* whExplSprite = new Sprite();
	for (int i = 0; i < whExplImgs.size(); i++) {
		whExplSprite->addSpriteAnimationFrame(whExplImgs[i]);
	}
	whExplSprite->setColor((UINT)255, 218, 185, 255);
	whExplSprite->setDepth(3);
	whExplSprite->setSize(45.0f, -1.0f);
	whExplSprite->setAlignment(AgkHAlign::center, AgkVAlign::center);
	explosionAnimations["white-explosion"] = whExplSprite;
}*/




/*void MobController::initPerlinNoiseValues() {
	std::vector<float> tmpxvals;
	siv::PerlinNoise pn;
	float amplitude = 10.0f;
	float scaling = 0.0085f;
	int octaves = 4;
	int nvalues = 1000;

	float step = 1.0f;

	for (int i = 0; i < nvalues; i++) {
		if(i < (nvalues/2))
			tmpxvals.push_back(step*(float)i);
		else{
			int j = nvalues - i;
			tmpxvals.push_back(step*(float)j);
		}
		offsetPNvalues.push_back(amplitude*pn.octaveNoise(tmpxvals[i] * scaling, octaves));
	}
}*/