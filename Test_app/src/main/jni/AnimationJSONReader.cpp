#include "AnimationJSONReader.h"

bool AnimationJSONReader::readAnimations(
	std::map<std::string, Sprite*>* animSprites,
	std::map<std::string, std::vector<AgkImage*>>* animImages,
	char* jsonfile, std::string& errmsg) 
{
	errmsg = "";
	JSONArray* adescrArr = checkJSONfile(jsonfile, errmsg);
	if (!adescrArr)
		return false;

	for (int i = 0; i < adescrArr->m_iNumElements; i++) {
		if (!checkJSONelementType("animation element", adescrArr->GetElement(i), AGK_JSON_OBJECT, errmsg))
			return false;

		JSONObject* animObj = (JSONObject*)adescrArr->GetElement(i);
		if (!createAnimationSprite(animSprites, animImages, animObj, errmsg))
			return false;
	}
	return true;
}


bool AnimationJSONReader::createAnimationSprite(
	std::map<std::string, Sprite*>* animSprites,
	std::map<std::string, std::vector<AgkImage*>>* animImages, 
	JSONObject* obj, std::string& errmsg) 
{
	uString id, prefix;
	int start_n, end_n, npadded_0;

	int nPairs = obj->m_iNumPairs;
	for (int i = 0; i < nPairs; i++) {
		JSONKeyPair* pair = obj->m_pPairs[i];
		uString name = pair->m_sName;
		JSONElement* el = pair->m_pElement;

		if (!name.CompareTo("id")) {
			if (!checkJSONelementType("id", el, AGK_JSON_STRING, errmsg)) return false;
			id = ((JSONString*)el)->m_sValue;
		}
		else if (!name.CompareTo("prefix")) {
			if (!checkJSONelementType("prefix", el, AGK_JSON_STRING, errmsg)) return false;
			prefix = ((JSONString*)el)->m_sValue;
		}
		else if (!name.CompareTo("start_n")) {
			if (!checkJSONelementType("start_n", el, AGK_JSON_NUMBER, errmsg)) return false;
			start_n = ((JSONNumber*)el)->m_iValue;
		}
		else if (!name.CompareTo("end_n")) {
			if (!checkJSONelementType("end_n", el, AGK_JSON_NUMBER, errmsg)) return false;
			end_n = ((JSONNumber*)el)->m_iValue;
		}
		else if (!name.CompareTo("n_padded_0")) {
			if (!checkJSONelementType("n_padded_0", el, AGK_JSON_NUMBER, errmsg)) return false;
			npadded_0 = ((JSONNumber*)el)->m_iValue;
		}	
	}

	std::vector<AgkImage*> imgs;
	Sprite* spr = loadImagesAndCreateAnimationSprite(&imgs, toStdStr(prefix), start_n, end_n, npadded_0);
	(*animImages)[id.GetStr()] = imgs;
	(*animSprites)[id.GetStr()] = spr;
	return true;
}

Sprite* AnimationJSONReader::loadImagesAndCreateAnimationSprite(
	std::vector<AgkImage*>* imgs, const std::string &prefix,
	int start_n, int end_n, int npadded_0)
{
	for (int i = start_n; i < (end_n + 1); i++) {
		std::string path_i = prefix + padZeros_stdstr(i, npadded_0) + ".png";
		AgkImage* img = new AgkImage(&path_i[0]);
		imgs->push_back(img);
	}

	Sprite* spr = new Sprite();
	for (int i = 0; i < imgs->size(); i++) {
		spr->addSpriteAnimationFrame((*imgs)[i]);
	}
	spr->setAlignment(AgkHAlign::center, AgkVAlign::center);
	spr->hide();
	spr->setSize(50.0f, -1);
	spr->setDepth(3);
	return spr;
}