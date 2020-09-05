#include "TowerAimless.h"

TowerAimless::TowerAimless(TowerDescriptor* _towerDescriptor) 
	: Tower(_towerDescriptor) 
{	
}

void TowerAimless::setDirection(const glm::vec2& dir)
{
	Tower::setDirection(dir);
	shootDirection = dir;
}

bool TowerAimless::canShoot() {
	return Tower::canShoot() && hasMobInRange();
}

void TowerAimless::postSetCellsInRange() {
	if (Cell* c = cellRange->firstCellInRange())
		targetPosition = c->posCenter();
}

void TowerAimless::chooseTarget(Map* map) {
	
	if (towerDescriptor->bulletStats.type == bullet_type::chain ||
		towerDescriptor->bulletStats.type == bullet_type::follow ||
		towerDescriptor->bulletStats.type == bullet_type::path)
	{
		if (!idleBullets.empty() && isLoaded && hasMobInRange()) {
			if (targetSelectionType == target_selection_type::first)
				currentTarget = cellRange->getFirstMobInRange(true);
			else if (targetSelectionType == target_selection_type::last)
				currentTarget = cellRange->getLastMobInRange(true);

			if (towerDescriptor->bulletStats.type == bullet_type::path && currentTarget)
				currentCellTarget = map->getCellAtLocation(currentTarget->getPos());
		}
	}


}



