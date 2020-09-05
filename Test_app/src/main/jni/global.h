#pragma once
#include "agk.h"
#include "glm/glm.hpp"
#include "colours.h"
#include <iostream>
#include <string>

#define CELL_SIZE 25.0f
#define DELTA_TIME 0.01f;

#define TOSTR(f,p) std::string(agk::Str(f,p))



extern int VIRTUAL_WIDTH;
extern int VIRTUAL_HEIGHT;
extern const float ASPECT;

//TODO: Looks like direction is used more as a Cell Face ID, not really a direction...
//Maybe look into that

namespace td{
enum class direction {
	north, south, east, west, unset
};

enum class path_direction {
	forward, backward, any
};

}

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

glm::vec2 getOrthogonalDirectionAsVector(td::direction d);
glm::vec2 getDirectionAsVector(td::direction d);
td::direction oppositeDirectionOf(td::direction d);
bool isAnyOf(td::direction refd1, td::direction refd2, td::direction d1, td::direction d2);
bool isNorthSouth(td::direction d);
bool isEastWest(td::direction d);

bool isLeftOfUnitLine(const glm::vec2& endpoint, const glm::vec2& p);

std::string toStdStr(const uString& ustr);
uString padZeros_ustr(int val, int ndigits);
std::string padZeros_stdstr(int val, int ndigits);

void splitStringNumeric(const std::string& orignial, std::string& prefix, int& suffix);