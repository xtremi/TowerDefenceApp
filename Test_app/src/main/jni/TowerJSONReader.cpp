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

	JSONElement* e = nullptr;

	if (!getValue(bname, "name", obj, errmsg)) return false;
	if (!getValue(bid, "id", obj, errmsg)) return false;
	if (!getValue(bpath, "path", obj, errmsg)) return false;

	if (bname.GetNumChars() == 0 || bpath.GetNumChars() == 0) {
		errmsg = "Invalid name or path in bullet definition.";
		return false;
	}

	MEDIA_BANK.addImage(bpath, bname);
	return true;
}

bool TowerJSONReader::createTowerDescriptor(JSONObject* obj, std::string& errmsg) {
	float t0, t1;
	float tStart;
	t0 = agk::Timer();
	tStart = t0;
	std::vector<float> dts;

	TowerDescriptor tdescr;
	if (!readTowerMainData(&tdescr, obj, errmsg)) return false;
	t1 = agk::Timer();
	dts.push_back(t1 - t0);
	t0 = t1;

	if (!readTowerImageData(&tdescr, obj, errmsg)) return false;
	t1 = agk::Timer();
	dts.push_back(t1 - t0);
	t0 = t1;

	JSONObject* bulletObj = nullptr;
	if (JSONElement* e = getAndCheckJSONelement("bullet", obj, AGK_JSON_OBJECT, errmsg))
		bulletObj = ((JSONObject*)e);
	t1 = agk::Timer();
	dts.push_back(t1 - t0);
	t0 = t1;

	if (!readTowerBulletData(&tdescr, bulletObj, errmsg)) return false;
	t1 = agk::Timer();
	dts.push_back(t1 - t0);
	t0 = t1;

	float dT = agk::Timer() - tStart;
	for (float& dt : dts) dt *= (100.0 / dT);

	(*towerDescriptors)[tdescr.ID] = tdescr;
	return true;
}


bool TowerJSONReader::readTowerMainData(TowerDescriptor* tdescr, JSONObject* obj, std::string& errmsg){
	
	if (!getValue(tdescr->name, "name", obj, errmsg)) return false;
	std::string idStr;
	if (!getValue(idStr, "id", obj, errmsg)) return false;
	tdescr->ID = towerIDstringToInt(idStr);

	if (!getValue(tdescr->rotate_speed, "rotate_speed", obj, errmsg)) return false;
	if (!getValue(tdescr->reload_time, "reload_time", obj, errmsg)) return false;
	if (!getValue(tdescr->range, "range", obj, errmsg)) return false;
	if (!getValue(tdescr->aoe_dir, "ndir", obj, errmsg)) return false;
	if (!getValue(tdescr->price, "price", obj, errmsg)) return false;

	std::vector<std::string> upgrades;
	if (!getValue(upgrades, "upgrades", obj, errmsg)) return false;
	for (std::string idStr : upgrades){
		if(!idStr.empty())
			tdescr->upgradable.push_back(towerIDstringToInt(idStr));
	}

	uString towerTypeStr;
	if (!getValue(towerTypeStr, "type", obj, errmsg)) return false;
	tdescr->towerType = getTowerTypeFromString(towerTypeStr);
	return true;
}
bool TowerJSONReader::readTowerBulletData(TowerDescriptor* tdescr, JSONObject* obj, std::string& errmsg) {
	
	uString bulletImgStr, bulletTypeStr;
	if (!getValue(bulletImgStr, "img", obj, errmsg)) return false;
	tdescr->bulletStats.bulletImage = MEDIA_BANK.getImage(bulletImgStr);

	if (!getValue(bulletTypeStr, "type", obj, errmsg)) return false;
	tdescr->bulletStats.type = getBulletTypeFromString(bulletTypeStr);

	if (!getValue(tdescr->bullet_buffer_size, "buffer", obj, errmsg)) return false;	
	if (!getValue(tdescr->bulletStats.size, "size", obj, errmsg)) return false;


	JSONObject* hitEffObj = nullptr;
	if (JSONElement* e = getAndCheckJSONelement("hiteffect", obj, AGK_JSON_OBJECT, errmsg))
		hitEffObj = ((JSONObject*)e);
	
	if (hitEffObj) {
		uString effVisStr, effTypeStr;
		if (!getValue(effVisStr, "vis", hitEffObj, errmsg)) return false;
		if (!getValue(effTypeStr, "type", hitEffObj, errmsg)) return false;
		tdescr->bulletStats.effectVisualization = getBulletEffectVisualizationFromString(effVisStr);
		tdescr->bulletStats.effectType = getBulletHitEffectFromString(effTypeStr);

		JSONObject* hitEffAnimObj = nullptr;
		if (JSONElement* e = getAndCheckJSONelement("animation", hitEffObj, AGK_JSON_OBJECT, errmsg))
			hitEffAnimObj = (JSONObject*)e;

		if (hitEffAnimObj) {
			if (!getValue(tdescr->bulletStats.hitAnimationStats.fps, "fps", hitEffAnimObj, errmsg)) return false;
			if (!getValue(tdescr->bulletStats.hitAnimationStats.animationID, "img", hitEffAnimObj, errmsg)) return false;
			if (!getValue(tdescr->bulletStats.hitAnimationStats.size, "size", hitEffAnimObj, errmsg)) return false;
		}
	}

	JSONObject* aoeObj = nullptr;
	if (JSONElement* e = getAndCheckJSONelement("aoe", obj, AGK_JSON_OBJECT, errmsg))
		aoeObj = (JSONObject*)e;

	if (aoeObj) {
		if (!getValue(tdescr->bulletStats.aoeStats.hasAoe, "on", aoeObj, errmsg)) return false;
		if(tdescr->bulletStats.aoeStats.hasAoe){
			if (!getValue(tdescr->bulletStats.aoeStats.aoeRange, "radius", aoeObj, errmsg)) return false;
			if (!getValue(tdescr->bulletStats.aoeStats.aoeReduction, "reduction", aoeObj, errmsg)) return false;
		}

	}

	JSONObject* valuesObj = nullptr;
	if (JSONElement* e = getAndCheckJSONelement("values", obj, AGK_JSON_OBJECT, errmsg))
		valuesObj = ((JSONObject*)e);
	if (valuesObj) {
		
		//double data:
		std::vector<std::string> dnames({ "v", "w", "z" });
		std::vector<std::vector<float>> data(3);
		for (int i = 0; i < dnames.size(); i++) {
			if (!getValue(data[i], dnames[i], valuesObj, errmsg)) return false;
		}
		tdescr->bulletStats.data.setV(data[0]);
		tdescr->bulletStats.data.setW(data[1]);
		tdescr->bulletStats.data.setZ(data[2]);

		//string data:
		std::vector<std::string> snames({"s", "a"});
		std::vector<std::vector<std::string>> sdata(2);
		for (int i = 0; i < snames.size(); i++) {
			if (!getValue(sdata[i], snames[i], valuesObj, errmsg)) return false;
		}
		tdescr->bulletStats.data.setS(sdata[0]);


		//Enumerations (defined as string in JSON to int TowerDescriptor):
		if (tdescr->bulletStats.effectType == bullet_hit_effect_type::debuff){
			tdescr->bulletStats.data.setA1((int)getMobEffectTypeFromString(sdata[1][0]));

			if (tdescr->bulletStats.type == bullet_type::chain)
				tdescr->bulletStats.data.setA2((int)getBoolFromString(sdata[1][1]));

		}
		if (tdescr->bulletStats.type == bullet_type::area_spawn_instant || tdescr->bulletStats.type == bullet_type::area_spawn_long)
			tdescr->bulletStats.data.setA2((int)getAreaSpawnBulletType(sdata[1][1]));

		if (tdescr->bulletStats.type == bullet_type::chain)
			tdescr->bulletStats.data.setA3((int)getBoolFromString(sdata[1][2]));

	}
	return true;
}

bool TowerJSONReader::readTowerImageData(TowerDescriptor* tdescr, JSONObject* obj, std::string& errmsg) {

	std::vector<uString> fixedImagePaths, rotImagePaths, iconImagePaths;
	JSONElement* e = nullptr;
	if (e = getAndCheckJSONelement("images", obj, AGK_JSON_OBJECT, errmsg)) {
		if (!getTowerDescriptorImagePaths((JSONObject*)e, &fixedImagePaths, &rotImagePaths, &iconImagePaths, errmsg))
			return false;
	}
	else 
		return false;

	loadTowerDescriptorImages(&tdescr->images, tdescr->name, &fixedImagePaths, &rotImagePaths, &iconImagePaths);
	return true;
}

void TowerJSONReader::loadTowerDescriptorImages(
	TowerImages*		  twrImages, 
	const uString&        towerName,
	std::vector<uString>* fixedImagePaths,
	std::vector<uString>* rotImagePaths,
	std::vector<uString>* iconImagePaths
)
{
	std::vector<std::vector<uString>*> allImgpaths = { fixedImagePaths, rotImagePaths, iconImagePaths };
	std::vector<std::vector<AgkImage*>*> allImgs = { &twrImages->fixed, &twrImages->rotatable, &twrImages->icons };
	int c = 0;
	
	for (int i = 0; i < allImgpaths.size(); i++) {
		for (uString path : (*allImgpaths[i])) {
			uString imgkey = "";
			imgkey.Append(towerName).Append(uString("-")).AppendInt(c++);

			AgkImage* img = MEDIA_BANK.addImage(path, imgkey);
			if (!img)
				int tmp = 1;
			allImgs[i]->push_back(img);
		}
	}
}

bool TowerJSONReader::getTowerDescriptorImagePaths(JSONObject* obj,
	std::vector<uString>* fixedImagePaths,
	std::vector<uString>* rotImagePaths,
	std::vector<uString>* iconImagePaths,
	std::string& errmsg)
{
	if (!getValue(*fixedImagePaths, "fix", obj, errmsg)) return false;
	if (!getValue(*rotImagePaths, "rot", obj, errmsg)) return false;
	if (!getValue(*iconImagePaths, "icon", obj, errmsg)) return false;
	return true;
}



bool TowerJSONReader::checkTowerImagesJSONobject(JSONObject* obj, std::string& errmsg) {
	if (obj->m_iNumPairs < 3) {
		errmsg = "Expected 3 or more JSON pairs in images object, got " + std::to_string(obj->m_iNumPairs);
		return false;
	}

	bool gotFixed = false, gotRot = false, gotIcon = false;
	for (int i = 0; i < 3; i++) {
		if (!obj->m_pPairs[i]->m_sName.CompareTo("fix"))
			gotFixed = true;
		if (!obj->m_pPairs[i]->m_sName.CompareTo("rot"))
			gotRot = true;
		if (!obj->m_pPairs[i]->m_sName.CompareTo("icon"))
			gotIcon = true;
		if (obj->m_pPairs[i]->m_pElement->GetType() != AGK_JSON_ARRAY) {
			errmsg = "Attribute of JSON object images is not an AGK_JSON_ARRAY";
			return false;
		}
	}

	if (!gotFixed || /*!gotRot ||*/ !gotIcon) {
		errmsg = "Missing one or more attributes of fixed, rotatable, buttons or icons in images object.";
		return false;
	}

	return true;
}
/*bool getChainDebuffReleaseStateFromString(const std::string& typestr) {
	return getChainDebuffReleaseStateFromString(uString(&typestr[0]));
}
bool getChainDebuffReleaseStateFromString(const uString& typestr) {
	uString str = typestr;
	str.Lower();
	if (!str.CompareTo("false"))
		return false;
	return true;
}*/

bullet_effect_vis_type getBulletEffectVisualizationFromString(const uString& typestr) {
	if (!typestr.CompareTo("none"))
		return bullet_effect_vis_type::none;
	else if (!typestr.CompareTo("animation"))
		return bullet_effect_vis_type::animation;
	return bullet_effect_vis_type::none;
}

bullet_hit_effect_type getBulletHitEffectFromString(const uString& typestr) {
	if (!typestr.CompareTo("damage"))
		return bullet_hit_effect_type::damage;
	else if (!typestr.CompareTo("damage-over-time"))
		return bullet_hit_effect_type::damage_over_time;
	else if (!typestr.CompareTo("debuff"))
		return bullet_hit_effect_type::debuff;
	else if (!typestr.CompareTo("teleport"))
		return bullet_hit_effect_type::teleport;
	return bullet_hit_effect_type::damage;
}
mob_effect_type	getMobEffectTypeFromString(const std::string& typestr) {
	return getMobEffectTypeFromString(uString(&typestr[0]));
}
mob_effect_type getMobEffectTypeFromString(const uString& typestr) {
	if (!typestr.CompareTo("speed"))
		return mob_effect_type::speed;
	else if (!typestr.CompareTo("DOT"))
		return mob_effect_type::dmg_over_time;
	else if (!typestr.CompareTo("tmp_health_red"))
		return mob_effect_type::temp_health_reduction;
	return mob_effect_type::speed;
}

bullet_type getBulletTypeFromString(const uString& typestr) {

	if (!typestr.CompareTo("normal"))
		return bullet_type::normal;
	else if (!typestr.CompareTo("area-spawn-long"))
		return bullet_type::area_spawn_long;
	else if (!typestr.CompareTo("chain"))
		return bullet_type::chain;
	else if (!typestr.CompareTo("follow"))
		return bullet_type::follow;
	else if (!typestr.CompareTo("area-spawn-instant"))
		return bullet_type::area_spawn_instant;
	else if (!typestr.CompareTo("path"))
		return bullet_type::path;
	else if (!typestr.CompareTo("lazer"))
		return bullet_type::lazer;
	return bullet_type::normal;
}

tower_type getTowerTypeFromString(const uString& typestr) {

	if (!typestr.CompareTo("aim"))
		return tower_type::aim;
	else if (!typestr.CompareTo("aimless"))
		return tower_type::aimless;
	else if (!typestr.CompareTo("block"))
		return tower_type::block;
	else if (!typestr.CompareTo("multi"))
		return tower_type::multi_dir;
	else if (!typestr.CompareTo("aoe"))
		return tower_type::aoe;
	return tower_type::aimless;
}

area_spawn_bullet_type	getAreaSpawnBulletType(const std::string& typestr) {
	return getAreaSpawnBulletType(uString(&typestr[0]));
}
area_spawn_bullet_type	getAreaSpawnBulletType(const uString& typestr) {
	if (!typestr.CompareTo("animation"))
		return area_spawn_bullet_type::animation;
	else if (!typestr.CompareTo("expansion"))
		return area_spawn_bullet_type::expansion;
	else if (!typestr.CompareTo("rotation"))
		return area_spawn_bullet_type::rotation;
	else
		return area_spawn_bullet_type::standard;

}

bool getBoolFromString(const std::string& str) {
	if (str[0] == '1' || str[0] == 'T' || str[0] == 't')
		return true;
	return false;
}


int towerIDstringToInt(const std::string& idstr) {
	int suffix = 0;
	std::string prefix = "";
	splitStringNumeric(idstr, prefix, suffix);
	int id_offset = 0;
	if (prefix == "TAL") id_offset = 1000;
	else if (prefix == "TA") id_offset = 2000;
	else if (prefix == "TB") id_offset = 3000;
	else if (prefix == "TM") id_offset = 4000;
	else if (prefix == "TAOE") id_offset = 5000;

	return id_offset + suffix;
}