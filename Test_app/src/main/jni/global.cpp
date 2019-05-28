#include "global.h"

const int VIRTUAL_WIDTH = 550;
const int VIRTUAL_HEIGHT = 1000;
const float ASPECT = 0.55;

void agkPrintVec2(const std::string& prefix, const glm::vec2& v) {
	std::string str = prefix;
	str += (TOSTR(v.x, 2) + ", " + TOSTR(v.y, 2));
	agk::Print(&str[0]);
}
/*template<typename T>
void agkPrint(const std::string& prefix, T f) {
	std::string str = prefix + std::to_string(f);
	agk::Print(&str[0]);
}*/


glm::vec2 getPointerPosition() {
	return glm::vec2(agk::GetPointerX(), agk::GetPointerY());
}
glm::vec2 getPointerPositionWorld() {
	return glm::vec2(agk::ScreenToWorldX(agk::GetPointerX()), agk::ScreenToWorldY(agk::GetPointerY()));
}
glm::vec2 getJoystickDirection() {
	return glm::vec2(agk::GetJoystickX(), agk::GetJoystickY());
}
glm::vec2 screenToWorld(const glm::vec2& spos) {
	return(glm::vec2(agk::ScreenToWorldX(spos.x), agk::ScreenToWorldY(spos.y)));
}
glm::vec2 worldToScreen(const glm::vec2& wpos) {
	return(glm::vec2(agk::WorldToScreenX(wpos.x), agk::WorldToScreenY(wpos.y)));
}

glm::vec2 getOrthogonalDirectionAsVector(direction d) {
	if (d == direction::north)		return glm::vec2(-1.0f, 0.0f);
	else if (d == direction::south) return glm::vec2(1.0f, 0.0f);
	else if (d == direction::east)	return glm::vec2(0.0f, -1.0f);
	else							return glm::vec2(0.0f, 1.0f);
}

glm::vec2 getDirectionAsVector(direction d) {
	if (d == direction::north)		return glm::vec2(0.0f, -1.0f);
	else if (d == direction::south) return glm::vec2(0.0f, 1.0f);
	else if (d == direction::east)	return glm::vec2(1.0f, 0.0f);
	else							return glm::vec2(-1.0f, 0.0f);
}
direction oppositeDirectionOf(direction d) {
	switch (d)
	{
	case direction::north:
		return direction::south;
	case direction::south:
		return direction::north;
	case direction::east:
		return direction::west;
	case direction::west:
		return direction::east;
	case direction::unset:
	default:
		return direction::unset;
	}
}

bool isAnyOf(direction refd1, direction refd2, direction d1, direction d2) {
	return ((d1 == refd1 && d2 == refd2) || (d1 == refd2 && d2 == refd1));
}

bool isNorthSouth(direction d) {
	switch (d)
	{
	case direction::north:
	case direction::south:
		return true;
		break;
	case direction::east:
	case direction::west:	
		return false;
		break;
	case direction::unset:
	default:
		return false;
		break;
	}
}
bool isEastWest(direction d) {
	switch (d)
	{
	case direction::north:
	case direction::south:
		return false;
		break;
	case direction::east:
	case direction::west:
		return true;
		break;
	case direction::unset:
	default:
		return false;
		break;
	}
}

std::string toStdStr(const uString& ustr) {
	std::string stdstr = "";

	for (int i = 0; i < ustr.GetNumChars(); i++)
		stdstr.push_back(ustr[i]);
	return stdstr;
}


bool isLeftOfUnitLine(const glm::vec2& endpoint, const glm::vec2& p) {
	float s = (endpoint.x)*(p.y) - (endpoint.y)*(p.x);
	return (s <= 0.0f);
}

std::string padZeros_stdstr(int val, int ndigits) {
	std::string str = std::to_string(val);
	int s = str.size();

	if (s >= ndigits)
		return str;
	else {
		for (int i = s; i < ndigits; i++)
			str.insert(str.begin(), '0');
	}
	return str;
}

uString padZeros_ustr(int value, int ndigits) {

	uString str = agk::Str(value);
	int s = str.GetNumChars();

	if (s >= ndigits)
		return str;
	else {
		for (int i = s; i < ndigits; i++)
			str.InsertUnicodeAt(0, '0');
	}
	return str;
}