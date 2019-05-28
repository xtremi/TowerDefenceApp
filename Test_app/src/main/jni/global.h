#pragma once
#include "agk.h"
#include "glm/glm.hpp"
#include <iostream>
#include <string>

#define CELL_SIZE 25.0f

#define TOSTR(f,p) std::string(agk::Str(f,p))


extern const int VIRTUAL_WIDTH;
extern const int VIRTUAL_HEIGHT;
extern const float ASPECT;

enum class direction {
	north, south, east, west, unset
};


#define JOYSTICK_ID 1

void agkPrintVec2(const std::string& prefix, const glm::vec2& v);
template<typename T>
void agkPrint(const std::string& prefix, T f) {
	std::string str = prefix + TOSTR(f,2);
	agk::Print(&str[0]);
}

glm::vec2 getPointerPosition();
glm::vec2 getPointerPositionWorld();
glm::vec2 getJoystickDirection();
glm::vec2 screenToWorld(const glm::vec2& spos);
glm::vec2 worldToScreen(const glm::vec2& wpos);

glm::vec2 getOrthogonalDirectionAsVector(direction d);
glm::vec2 getDirectionAsVector(direction d);
direction oppositeDirectionOf(direction d);
bool isAnyOf(direction refd1, direction refd2, direction d1, direction d2);
bool isNorthSouth(direction d);
bool isEastWest(direction d);

bool isLeftOfUnitLine(const glm::vec2& endpoint, const glm::vec2& p);

std::string toStdStr(const uString& ustr);
uString padZeros_ustr(int val, int ndigits);
std::string padZeros_stdstr(int val, int ndigits);