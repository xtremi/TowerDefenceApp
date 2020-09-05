#include "global.h"

const float ASPECT = 2.0;
int VIRTUAL_WIDTH = 500;
int VIRTUAL_HEIGHT = 1000;


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

glm::vec2 getOrthogonalDirectionAsVector(td::direction d) {
	if (d == td::direction::north)		return glm::vec2(-1.0f, 0.0f);
	else if (d == td::direction::south) return glm::vec2(1.0f, 0.0f);
	else if (d == td::direction::east)	return glm::vec2(0.0f, -1.0f);
	else							return glm::vec2(0.0f, 1.0f);
}

const static glm::vec2 DIR_NORTH(0.0f, -1.0f);
const static glm::vec2 DIR_SOUTH(0.0f, 1.0f);
const static glm::vec2 DIR_EAST(1.0f, 0.0f);
const static glm::vec2 DIR_WEST(-1.0f, 0.0f);

glm::vec2 getDirectionAsVector(td::direction d) {
	if (d == td::direction::north)		return DIR_NORTH;
	else if (d == td::direction::south) return DIR_SOUTH;
	else if (d == td::direction::east)	return DIR_EAST;
	else							return DIR_WEST;
}
td::direction oppositeDirectionOf(td::direction d) {
	switch (d)
	{
	case td::direction::north:
		return td::direction::south;
	case td::direction::south:
		return td::direction::north;
	case td::direction::east:
		return td::direction::west;
	case td::direction::west:
		return td::direction::east;
	case td::direction::unset:
	default:
		return td::direction::unset;
	}
}

bool isAnyOf(td::direction refd1, td::direction refd2, td::direction d1, td::direction d2) {
	return ((d1 == refd1 && d2 == refd2) || (d1 == refd2 && d2 == refd1));
}

bool isNorthSouth(td::direction d) {
	switch (d)
	{
	case td::direction::north:
	case td::direction::south:
		return true;
		break;
	case td::direction::east:
	case td::direction::west:	
		return false;
		break;
	case td::direction::unset:
	default:
		return false;
		break;
	}
}
bool isEastWest(td::direction d) {
	switch (d)
	{
	case td::direction::north:
	case td::direction::south:
		return false;
		break;
	case td::direction::east:
	case td::direction::west:
		return true;
		break;
	case td::direction::unset:
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

void splitStringNumeric(const std::string& orignial, std::string& prefix, int& suffix) {
	std::string suffixStr;

	for (int i = 0; i < orignial.size(); i++) {
		char c = orignial[i];
		if (c >= '0' && c <= '9')
			suffixStr.push_back(c);
		else
			prefix.push_back(c);
	}
	
	try{
		suffix = std::stoi(suffixStr);
	}
	catch (...) {
		suffix = -1;
	}
}