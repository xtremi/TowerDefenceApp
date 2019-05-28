#include "Cell.h"


Cell::Cell(const glm::vec2& pos, int locx, int locy) : Sprite()//, UIButton()
{
	setPosition(pos);
	maploc[0] = locx;
	maploc[1] = locy;
	setSize(glm::vec2(CELL_SIZE, CELL_SIZE));
	setType(cell_type::grass);
}

void Cell::assignCellDirection(direction in, direction out) {
	dir_in = in;
	dir_out = out;
}

void Cell::setType(cell_type ct) {
	_type = ct;
	setCellImage();
}

void Cell::setValid(bool state) 
{ 
	if(state != _valid){
		_valid = state;
		setCellImage();
	}
}

void Cell::setCellImage() {
	switch (_type)
	{
	case Cell::cell_type::grass:
		_valid ? setImage(GETIMG(GRASS_IMAGE_c)) : setImage(GETIMG(GRASS_INACTIVE_IMAGE_c));
		break;
	case Cell::cell_type::road:
		_valid ? setImage(GETIMG(ROAD_BLOCK_IMAGE)) : setImage(GETIMG(ROAD_BLOCK_INACTIVE_IMAGE));
		break;
	case Cell::cell_type::entrance:
		setImage(GETIMG(START_BLOCK_IMAGE)); break;
	case Cell::cell_type::exit:
		setImage(GETIMG(END_BLOCK_IMAGE)); 	break;
	default:
		break;
	}
}

bool Cell::hasTurn() {
	return !isAnyOf(direction::north, direction::south, dir_in, dir_out) || isAnyOf(direction::east, direction::west, dir_in, dir_out);
}

glm::vec2 Cell::posCenter() {
	return getPos() + CELL_SIZE / 2.0f;
}

glm::vec2 Cell::posIn() {
	glm::vec2 dpos;
	if (dir_in == direction::north)
		dpos = glm::vec2(CELL_SIZE / 2.0f, 0.0f);
	else if (dir_in == direction::south)
		dpos = glm::vec2(CELL_SIZE / 2.0f, CELL_SIZE);
	else if (dir_in == direction::east)
		dpos = glm::vec2(CELL_SIZE, CELL_SIZE / 2.0f);
	else
		dpos = glm::vec2(0.0f, CELL_SIZE / 2.0f);
	return dpos + getPos();
}

glm::vec2 Cell::getLocalPosInCell(const glm::vec2& pos) {
	return (pos - getPos() - glm::vec2(CELL_SIZE / 2.0f));
}
bool Cell::positionIsPassedCellCenter(const glm::vec2& pos) {
	glm::vec2 localPosition = getLocalPosInCell(pos);
	return (
		dir_in == direction::north && localPosition.y > 0.0f ||
		dir_in == direction::south && localPosition.y < 0.0f ||
		dir_in == direction::east && localPosition.x < 0.0f ||
		dir_in == direction::west && localPosition.x > 0.0f
		);
}
bool Cell::positionIsPassedCellDiagonal(const glm::vec2& pos) {
	glm::vec2 localPosition = getLocalPosInCell(pos);	
	if (
		(dir_in == direction::south && dir_out == direction::east) ||
		(dir_in == direction::west && dir_out == direction::north)
	){
		return isLeftOfUnitLine(glm::vec2(1.0f, 1.0f), localPosition);
	}
	else if (
		(dir_in == direction::south && dir_out == direction::west) ||
		(dir_in == direction::east && dir_out == direction::north)
		) {
		return isLeftOfUnitLine(glm::vec2(1.0f, -1.0f), localPosition);
	}
	else if (
		(dir_in == direction::east && dir_out == direction::south) ||
		(dir_in == direction::north && dir_out == direction::west)
		) {
		return isLeftOfUnitLine(glm::vec2(-1.0f, -1.0f), localPosition);
	}
	else if (
		(dir_in == direction::west && dir_out == direction::south) ||
		(dir_in == direction::north && dir_out == direction::east)
		) {
		return isLeftOfUnitLine(glm::vec2(-1.0f, 1.0f), localPosition);
	}
	return false;
}


//This should maybe be done in Map class?
void Cell::setBuilding(Building* bld) {
	building = bld;
	building->setPosition(this->posCenter(), maploc[0], maploc[1]);
}

Mob* Cell::firstMob() {
	mobsIterator = 0;
	return nextMob();
}
Mob* Cell::nextMob() {
	if (mobsIterator < mobs.size())
		return mobs[mobsIterator++];
	return NULL;
}
std::vector<Mob*>* Cell::getMobs() {
	return &mobs;
}
void Cell::clearMobs() {
	mobs.clear();
}
void Cell::addMob(Mob* m) {
	mobs.push_back(m);
}

bool Cell::isRoad() {
	return (_type == cell_type::road);
}

bool cellCompareRoadIDgreater(Cell* c1, Cell* c2) { return (c1->getRoadID() > c2->getRoadID()); }