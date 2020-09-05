#include "TowerBlock.h"

TowerBlock::TowerBlock(TowerDescriptor* _towerDescriptor)
	: Tower(_towerDescriptor)
{
}

void TowerBlock::setDirection(const glm::vec2& dir){
	
}
bool TowerBlock::canShoot() {
	return false;
}
