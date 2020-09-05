#include "MobJSONReader.h"

bool MobJSONReader::readMobs(std::map<int, MobDescriptor>* _mobDescriptors, char* jsonfile, std::string& errmsg) {
	errmsg = "";
	mobDescriptor = _mobDescriptors;

	JSONArray* mdescrArr = checkJSONfile(jsonfile, errmsg);
	if (!mdescrArr)
		return false;

	for (int i = 0; i < mdescrArr->m_iNumElements; i++) {
		if (!checkJSONelementType("mob array element", mdescrArr->GetElement(i), AGK_JSON_OBJECT, errmsg))
			return false;

		JSONObject* mobObj = (JSONObject*)mdescrArr->GetElement(i);
		if (!createMobDescriptor(mobObj, errmsg))
			return false;
	}
	return true;
}


bool MobJSONReader::createMobDescriptor(JSONObject* obj, std::string& errmsg) {

	MobDescriptor mdescr;
	uString imgPath = "";

	int nPairs = obj->m_iNumPairs;
	for (int i = 0; i < nPairs; i++) {
		JSONKeyPair* pair = obj->m_pPairs[i];
		uString name = pair->m_sName;
		JSONElement* el = pair->m_pElement;

		if (!name.CompareTo("name")) {
			if (!checkJSONelementType("name", el, AGK_JSON_STRING, errmsg)) return false;
			mdescr.name = ((JSONString*)el)->m_sValue;
		}
		else if (!name.CompareTo("id")) {
			if (!checkJSONelementType("id", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.ID = ((JSONNumber*)el)->m_iValue;
		}
		else if (!name.CompareTo("health")) {
			if (!checkJSONelementType("health", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.health = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("armor1")) {
			if (!checkJSONelementType("armor1", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.armor1 = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("armor2")) {
			if (!checkJSONelementType("armor2", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.armor2 = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("armor3")) {
			if (!checkJSONelementType("armor3", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.armor3 = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("speed_min")) {
			if (!checkJSONelementType("speed_min", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.speed_min = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("speed_max")) {
			if (!checkJSONelementType("speed_max", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.speed_max = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("size.x")) {
			if (!checkJSONelementType("size.x", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.size.x = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("size.y")) {
			if (!checkJSONelementType("size.y", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.size.y = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("reward")) {
			if (!checkJSONelementType("reward", el, AGK_JSON_NUMBER, errmsg)) return false;
			mdescr.reward = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("image_path")) {
			if (!checkJSONelementType("image_path", el, AGK_JSON_STRING, errmsg)) return false;		
			imgPath = ((JSONString*)el)->m_sValue;
		}
	}
	
	if (imgPath.GetNumChars() == 0) {
		errmsg = "Missing mob image.";
		return false;
	}

	uString imgkey = mdescr.name;
	imgkey.Append("_img");	
	mdescr.image = MEDIA_BANK.addImage(imgPath, imgkey);
	
	(*mobDescriptor)[mdescr.ID] = mdescr;
	return true;

}