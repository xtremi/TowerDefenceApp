#include "TowerJSONReader.h"

bool TowerJSONReader::readTowers(std::map<int, TowerDescriptor>* _towerDescriptors, char* jsonfile, std::string& errmsg) {
	errmsg = "";
	towerDescriptors = _towerDescriptors;

	JSONArray* tdescrArr = checkJSONfile(jsonfile, errmsg);
	if (!tdescrArr)
		return false;

	for (int i = 0; i < tdescrArr->m_iNumElements; i++) {
		if (!checkJSONelementType("tower array element", tdescrArr->GetElement(i), AGK_JSON_OBJECT, errmsg))
			return false;
		
		JSONObject* towerObj = (JSONObject*)tdescrArr->GetElement(i);
		if (!createTowerDescriptor(towerObj, errmsg)) 
			return false;
	}
	return true;
}


bool TowerJSONReader::readBullets(char* jsonfile, std::string& errmsg) {
	errmsg = "";
	
	JSONArray* tdescrArr = checkJSONfile(jsonfile, errmsg);
	if (!tdescrArr)
		return false;

	for (int i = 0; i < tdescrArr->m_iNumElements; i++) {
		if (!checkJSONelementType("bullet array element", tdescrArr->GetElement(i), AGK_JSON_OBJECT, errmsg))
			return false;

		JSONObject* bulletObj = (JSONObject*)tdescrArr->GetElement(i);
		if (!loadBulletImage(bulletObj, errmsg))
			return false;
	}
	return true;
}

bool TowerJSONReader::loadBulletImage(JSONObject* obj, std::string& errmsg) {

	uString bname;
	uString bpath;
	int bid = -1;

	int nPairs = obj->m_iNumPairs;
	for (int i = 0; i < nPairs; i++) {
		JSONKeyPair* pair = obj->m_pPairs[i];
		uString name = pair->m_sName;
		JSONElement* el = pair->m_pElement;

		if (!name.CompareTo("name")) {
			if (!checkJSONelementType("name", el, AGK_JSON_STRING, errmsg)) return false;
			bname = ((JSONString*)el)->m_sValue;
		}
		else if (!name.CompareTo("id")) {
			if (!checkJSONelementType("id", el, AGK_JSON_NUMBER, errmsg)) return false;
			bid = ((JSONNumber*)el)->m_iValue;
		}
		else if (!name.CompareTo("path")) {
			if (!checkJSONelementType("path", el, AGK_JSON_STRING, errmsg)) return false;
			bpath = ((JSONString*)el)->m_sValue;
		}
	}

	if (bname.GetNumChars() == 0 || bpath.GetNumChars() == 0) {
		errmsg = "Invalid name or path in bullet definition.";
		return false;
	}

	MEDIA_BANK.addImage(bpath, bname);
	return true;
}

bool TowerJSONReader::createTowerDescriptor(JSONObject* obj, std::string& errmsg) {
	TowerDescriptor tdescr;
	std::vector<uString> fixedImagePaths;
	std::vector<uString> rotImagePaths;
	std::vector<uString> buttonImagePaths;

	int nPairs = obj->m_iNumPairs;
	for (int i = 0; i < nPairs; i++) {
		JSONKeyPair* pair = obj->m_pPairs[i];
		uString name = pair->m_sName;
		JSONElement* el = pair->m_pElement;

		if (!name.CompareTo("images")) {
			if (!checkJSONelementType("images", el, AGK_JSON_OBJECT, errmsg)) return false;
			if (!getTowerDescriptorImagePaths((JSONObject*)el, &fixedImagePaths, &rotImagePaths, &buttonImagePaths, errmsg)) return false;
		}
		else if (!name.CompareTo("name")) {
			if (!checkJSONelementType("name", el, AGK_JSON_STRING, errmsg)) return false;
			tdescr.name = ((JSONString*)el)->m_sValue;
		}
		else if (!name.CompareTo("id")) {
			if (!checkJSONelementType("id", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.ID = ((JSONNumber*)el)->m_iValue;
		}
		else if (!name.CompareTo("rotate_speed")) {
			if (el->GetType() != 0) {
				if (!checkJSONelementType("rotate_speed", el, AGK_JSON_NUMBER, errmsg)) return false;
				tdescr.rotate_speed = ((JSONNumber*)el)->m_fValue;
				tdescr.canRotate = true;
			}
			else
				tdescr.canRotate = false;
		}
		else if (!name.CompareTo("range")) {
			if (!checkJSONelementType("range", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.range = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("reload_time")) {
			if (!checkJSONelementType("reload_time", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.reload_time = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("bullet_stats.width")) {
			if (!checkJSONelementType("bullet_stats.width", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.bulletStats.width = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("bullet_stats.height")) {
			if (!checkJSONelementType("bullet_stats.height", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.bulletStats.heigth = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("bullet_stats.damage")) {
			if (!checkJSONelementType("bullet_stats.damage", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.bulletStats.damage = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("bullet_stats.speed")) {
			if (!checkJSONelementType("bullet_stats.speed", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.bulletStats.speed = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("bullet_stats.aoe")) {}
		else if (!name.CompareTo("bullet_stats.follow_target")) {}
		else if (!name.CompareTo("upgrades")) {
			if (!checkJSONelementType("upgrades", el, AGK_JSON_STRING, errmsg)) return false;
			uString strvec = ((JSONString*)el)->m_sValue;
			tdescr.upgradable = toIntVector(strvec);
		}
		else if (!name.CompareTo("price")) {
			if (!checkJSONelementType("price", el, AGK_JSON_NUMBER, errmsg)) return false;
			tdescr.price = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("bullet_name")) {
			if (!checkJSONelementType("bullet_name", el, AGK_JSON_STRING, errmsg)) return false;
			uString bulletName = ((JSONString*)el)->m_sValue;
			tdescr.bulletStats.bulletImage = MEDIA_BANK.getImage(bulletName.GetStr());
		}
	}
	loadTowerDescriptorImages(&tdescr.images, tdescr.name, &fixedImagePaths, &rotImagePaths, &buttonImagePaths);
	(*towerDescriptors)[tdescr.ID] = tdescr;
	return true;
}

void TowerJSONReader::loadTowerDescriptorImages(TowerImages* twrImages, const uString& towerName,
	std::vector<uString>* fixedImagePaths,
	std::vector<uString>* rotImagePaths,
	std::vector<uString>* buttonsImagePaths)
{
	std::vector<std::vector<uString>*> allImgpaths = { fixedImagePaths, rotImagePaths, buttonsImagePaths };
	std::vector<std::vector<AgkImage*>*> allImgs = { &twrImages->fixed, &twrImages->rotatable, &twrImages->buttons };
	int c = 0;
	for (int i = 0; i < 3; i++) {
		for (uString path : (*allImgpaths[i])) {
			uString imgkey = "";
			imgkey.Append(towerName).Append(uString("-")).AppendInt(c++);

			AgkImage* img = MEDIA_BANK.addImage(&path[0], imgkey);
			allImgs[i]->push_back(img);
		}
	}
}

bool TowerJSONReader::getTowerDescriptorImagePaths(JSONObject* obj,
	std::vector<uString>* fixedImagePaths,
	std::vector<uString>* rotImagePaths,
	std::vector<uString>* buttonsImagePaths,
	std::string& errmsg)
{
	if (!checkTowerImagesJSONobject(obj, errmsg)) return false;

	std::vector<std::vector<uString>*> imgpaths = { fixedImagePaths, rotImagePaths, buttonsImagePaths };

	for (int i = 0; i < imgpaths.size(); i++) {
		JSONArray* jarray = ((JSONArray*)obj->m_pPairs[i]->m_pElement);
		if (!JSONarrayToStdVector(jarray, imgpaths[i])) {
			errmsg = "Error parsing images paths AGK_JSON_ARRAY";
			return false;
		}
	}
	return true;
}



bool TowerJSONReader::checkTowerImagesJSONobject(JSONObject* obj, std::string& errmsg) {
	if (obj->m_iNumPairs != 3) {
		errmsg = "Expected 3 JSON pairs in images object, got " + std::to_string(obj->m_iNumPairs);
		return false;
	}

	bool gotFixed = false, gotRot = false, gotButton = false;
	for (int i = 0; i < 3; i++) {
		if (!obj->m_pPairs[i]->m_sName.CompareTo("fix"))
			gotFixed = true;
		if (!obj->m_pPairs[i]->m_sName.CompareTo("rot"))
			gotRot = true;
		if (!obj->m_pPairs[i]->m_sName.CompareTo("btn"))
			gotButton = true;
		if (obj->m_pPairs[i]->m_pElement->GetType() != AGK_JSON_ARRAY) {
			errmsg = "Attribute of JSON object images is not an AGK_JSON_ARRAY";
			return false;
		}
	}

	if (!gotFixed || !gotRot || !gotButton) {
		errmsg = "Missing one or more attributes of fixed, rotatable or buttons in images object.";
		return false;
	}

	return true;
}



