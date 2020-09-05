#include "TowerAOE.h"

TowerAOE::TowerAOE(TowerDescriptor* _towerDescriptor) 
	: Tower(_towerDescriptor)
{	
}

void TowerAOE::init(Cell* cell) {
	Tower::init(cell);
	targetPosition = this->cpos;
}

void TowerAOE::setDirection(const glm::vec2& dir){
	return;
}
bool TowerAOE::canShoot(){
	return hasMobInRange();
}
void TowerAOE::aim(){
	return;
}
void TowerAOE::chooseTarget(Map* map){}
