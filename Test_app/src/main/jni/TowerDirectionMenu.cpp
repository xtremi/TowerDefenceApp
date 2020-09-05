#include "TowerDirectionMenu.h"


void TowerDirectionMenu::initSprites() {
	glm::vec4 btnCol(1, 0, 0, 0.5);
	glm::vec4 btnCol_p(1, 0, 0, 0.8);
	glm::vec4 btnCol_i(0.2, 0.2, 0.2, 1);

	static float angs[4]	= { 0.0f, 90.0f, 180.0f, 270.0f };
	static float pos_fx[4]	= { 1.0f, 0.0f, -1.0f, 0.0f };
	static float pos_fy[4]	= { 0.0f, 1.0f, 0.0f, -1.0f };
	for (int i = 0; i < 4; i++) {

		UISpriteButton_T2* dirBtn = new UISpriteButton_T2(btnCol, btnCol_p, btnCol_i, glm::vec2(CELL_SIZE), glm::vec2(0));

		dirBtn->setAlignment(AgkHAlign::left, AgkVAlign::center);
		dirBtn->setDepth(1);
		dirBtn->setRotation(angs[i]);
		std::string id = "dirbtn" + std::to_string(i + 1);
		addButton(dirBtn, id, 0.5f*CELL_SIZE * glm::vec2(pos_fx[i], pos_fy[i]));
	}

	hide();
	disable();
}


bool TowerDirectionMenu::directionButtonWasClicked(td::direction& dir) {

	if (getButtonState("dirbtn1")) {
		dir = td::direction::east;
		return true;
	}
	else if (getButtonState("dirbtn2")) {
		dir = td::direction::south;
		return true;
	}
	else if (getButtonState("dirbtn3")) {
		dir = td::direction::west;
		return true;
	}
	else if (getButtonState("dirbtn4")) {
		dir = td::direction::north;
		return true;
	}

	return false;
}