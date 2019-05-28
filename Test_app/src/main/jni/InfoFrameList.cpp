#include "InfoFrameList.h"

void InfoFrameList::resizeBackground() {
	glm::vec2 size;
	size.x = txtOffset.x + dx1 + dx2;
	size.y = txtOffset.y + dy1 * rows + dy2;
	sprites["bg"]->setSize(size);
}

void InfoFrameList::setListItemValue(const uString& name, const uString& val) {
	
	std::string keyN = name.GetStr() + std::string("-n");
	std::string keyV = name.GetStr() + std::string("-v");

	if (isNotInMap<AgkSprite>(keyN, &sprites)) {
		addListItem(name);
	}

	uString vstr = val;
	((TextSprite*)sprites[keyV])->setText((char*)vstr.GetStr());
}
void InfoFrameList::setListItemValue(const uString& name, float val, int prec) {
	uString str = agk::Str(val, prec);
	setListItemValue(name, str);
}
void InfoFrameList::setListItemValue(const uString& name, int val) {
	uString str = agk::Str(val);
	setListItemValue(name, str);
}
void InfoFrameList::setListItemValue(const uString& name, bool val) {
	uString str = agk::Str(val);
	setListItemValue(name, str);
}
void InfoFrameList::addListItem(const uString& name) {
	TextSprite* nameSprite = new TextSprite((char*)name.GetStr(), textColor, glm::vec2(0), textSize);
	TextSprite* valueSprite = new TextSprite("", textColor, glm::vec2(0), textSize);
	nameSprite->fixToScreen();
	valueSprite->fixToScreen();
	nameSprite->setDepth(3);
	valueSprite->setDepth(3);
	
	float ypos = dy1 * (float)rows + txtOffset.y;
	glm::vec2 pn(txtOffset.x, ypos);
	glm::vec2 pv(dx1 + txtOffset.x, ypos);
		
	std::string keyn = name.GetStr() + std::string("-n");
	std::string keyv = name.GetStr() + std::string("-v");

	addSprite(nameSprite,  keyn, pn);
	addSprite(valueSprite, keyv, pv);
		
	rows++;
	resizeBackground();
}

/*void InfoFrameList::setPosition(const glm::vec2& _pos) {
	pos = _pos;
	background->setPosition(pos);

}*/

