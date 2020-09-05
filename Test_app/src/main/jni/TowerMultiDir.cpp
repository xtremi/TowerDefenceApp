#include "TowerMultiDir.h"

TowerMultiDir::TowerMultiDir(TowerDescriptor* _towerDescriptor)
	: Tower(_towerDescriptor) 
{
	initAOEdirections();
	targets.resize(towerDescriptor->aoe_dir);
	cellTargets.resize(towerDescriptor->aoe_dir);
}

void TowerMultiDir::setDirection(const glm::vec2& dir) {}


void TowerMultiDir::initAOEdirections() {

	int nAoeDir = towerDescriptor->aoe_dir;

	float dangle = 360.0f / (float)nAoeDir;
	float angle = 180.0f;
	for (int i = 0; i < nAoeDir; i++) {
		//shootDirection = glm::vec2(agk::Cos(angle - 90.0f), agk::Sin(angle - 90.0f));
		//aoeDirections.push_back(shootDirection);
		aoeShootAngles.push_back(angle);
		angle += dangle;
	}
}

bool TowerMultiDir::canShoot() {
	return hasMobInRange();
}

void TowerMultiDir::chooseTarget(Map* map) {

	if(	towerDescriptor->bulletStats.type == bullet_type::path ||
		towerDescriptor->bulletStats.type == bullet_type::follow ||
		((towerDescriptor->bulletStats.type == bullet_type::chain) && activeBullets.empty()))
	{
		if (!idleBullets.empty() && isLoaded && hasMobInRange()) {
		
			for (int i = 0; i < towerDescriptor->aoe_dir; i++) {
				float		ang = aoeShootAngles[i] + currentRotation;
				glm::vec2	dir = glm::vec2(agk::Cos(ang - 90.0f), agk::Sin(ang - 90.0f));

				Mob* newTarget = nullptr;
				if(towerDescriptor->bulletStats.type == bullet_type::follow)
					newTarget = cellRange->findTargetInSector(dir, 40.0f);
				else 
					newTarget = cellRange->findTargetInSector(dir, 20.0f);
				targets[i] = newTarget;

				if (towerDescriptor->bulletStats.type == bullet_type::path && newTarget)
					cellTargets[i] = map->getCellAtLocation(newTarget->getPos());
				else
					cellTargets[i] = nullptr;
			}

		}
	}
}


void TowerMultiDir::shoot() {

	if (idleBullets.size() >= aoeShootAngles.size()) {

		for(int i = 0; i < aoeShootAngles.size(); i++){
			float ang			= aoeShootAngles[i] + currentRotation;
			shootDirection		= glm::vec2(agk::Cos(ang - 90.0f), agk::Sin(ang - 90.0f));
			currentTarget		= targets[i];
			currentCellTarget	= cellTargets[i];

			if (Tower::canShoot()){
				Bullet* blt = idleBullets.back();
				idleBullets.pop_back();			
				Tower::shoot(blt);
				blt->show();
				activeBullets.push_back(blt);
				BULLET_COUNT++;
			}
		}
		isLoaded = false;
		reload_timer = 0.0f;
	}	
	
}

void TowerMultiDir::aim() {

	if (towerDescriptor->bulletStats.type == bullet_type::chain) {
		if (!activeBullets.empty())
			return;
	}

	currentRotation += towerDescriptor->rotate_speed;
	if (currentRotation > 360.0f) currentRotation = 0.0f;

	for (AgkSprite* s : rotSprites) {
		s->setRotation(currentRotation);
	}
}

/*void TowerMultiDir::findTargets() {

	int i = 0;
	for (float ang : aoeShootAngles) {
		ang += currentRotation;
		glm::vec2 dir = glm::vec2(agk::Cos(ang - 90.0f), agk::Sin(ang - 90.0f));
		dir = glm::normalize(dir);

		glm::vec2 posToCheck = cpos + 0.7f*dir*realRangeDistance;
		float radiusToCheck = 0.4f*realRangeDistance;


		i++;
	}
}*/
