#pragma once
#include "GamePlay.h"


void GamePlay::initPerlinNoise()
{
	siv::PerlinNoise pn;
	for (int i = 0; i < 100; i++) {
		xpn.push_back(5.0f*(float)i);
		ypn.push_back(50.0f*pn.octaveNoise(xpn[i] * 0.005f, 4) + 500.0f);
	}
}

void GamePlay::setTestTowers2() {
	createTowerRow(14, 3, 6, 202);
	createTowerRow(14, 3, 7, 202);
	createTowerRow(14, 3, 8, 202);

	createTowerRow(17, 3, 6, 102);
	createTowerRow(17, 3, 7, 102);
	createTowerRow(17, 3, 8, 102);
}

void GamePlay::setTestTowers1() {
	createTowerRow(15, 7, 4, 202);
	createTowerRow(13, 7, 6, 202);
	createTowerRow(5, 14, 8, 202);
	createTowerRow(7, 14, 10, 202);
	createTowerRow(8, 13, 11, 202);
	createTowerRow(10, 8, 14, 202);
	createTowerRow(10, 8, 16, 202);
	createTowerRow(8, 12, 18, 202);
	createTowerRow(8, 12, 20, 202);
	createTowerRow(8, 12, 21, 202);
	createTowerRow(8, 12, 23, 202);
	createTowerRow(8, 12, 24, 202);
	createTowerRow(8, 12, 26, 202);
	createTowerRow(8, 6, 28, 202);
	createTowerRow(15, 6, 28, 202);
	createTowerRow(7, 12, 30, 202);
	createTowerRow(7, 12, 31, 202);
	createTowerRow(7, 12, 32, 202);
}

void GamePlay::createTowerRow(int startX, int nTowers, int y, int towerID) {
	for (int i = 0; i < nTowers; i++) {
		buildingControl.addNewTower(map.getCell(startX + i, y), &map);
		buildingControl.upgradeTower(map.getCell(startX + i, y), &map, towerID);
	}
}

