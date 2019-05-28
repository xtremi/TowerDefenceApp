#include "WaveJSONReader.h"

bool WaveJSONReader::readWaves(std::vector<WaveDescriptor>* _waveDescriptors, char* jsonfile, std::string& errmsg) {

	errmsg = "";
	waveDescriptors = _waveDescriptors;

	JSONArray* wdescrArr = checkJSONfile(jsonfile, errmsg);
	if (!wdescrArr)
		return false;

	for (int i = 0; i < wdescrArr->m_iNumElements; i++) {
		if (!checkJSONelementType("wave array element", wdescrArr->GetElement(i), AGK_JSON_OBJECT, errmsg))
			return false;

		JSONObject* waveObj = (JSONObject*)wdescrArr->GetElement(i);
		if (!createWaveDescriptor(waveObj, errmsg))
			return false;
	}
	return true;
}


bool WaveJSONReader::createWaveDescriptor(JSONObject* obj, std::string& errmsg) {

	WaveDescriptor wdescr;
	uString mobTypes = "";
	uString mobAmounts = "";

	int nPairs = obj->m_iNumPairs;
	for (int i = 0; i < nPairs; i++) {
		JSONKeyPair* pair = obj->m_pPairs[i];
		uString name = pair->m_sName;
		JSONElement* el = pair->m_pElement;

		if (!name.CompareTo("name")) {
			if (!checkJSONelementType("name", el, AGK_JSON_STRING, errmsg)) return false;
			wdescr.name = ((JSONString*)el)->m_sValue;
		}
		else if (!name.CompareTo("id")) {
			if (!checkJSONelementType("id", el, AGK_JSON_NUMBER, errmsg)) return false;
			wdescr.ID = ((JSONNumber*)el)->m_iValue;
		}
		else if (!name.CompareTo("duration")) {
			if (!checkJSONelementType("duration", el, AGK_JSON_NUMBER, errmsg)) return false;
			wdescr.duration = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("release_time")) {
			if (!checkJSONelementType("release_time", el, AGK_JSON_NUMBER, errmsg)) return false;
			wdescr.release_time = ((JSONNumber*)el)->m_fValue;
		}
		else if (!name.CompareTo("mob_types")) {
			if (!checkJSONelementType("mob_types", el, AGK_JSON_STRING, errmsg)) return false;
			mobTypes = ((JSONString*)el)->m_sValue;
		}
		else if (!name.CompareTo("mob_amounts")) {
			if (!checkJSONelementType("mob_amounts", el, AGK_JSON_STRING, errmsg)) return false;
			mobAmounts = ((JSONString*)el)->m_sValue;
		}
	}

	parseMobSetVector(&wdescr.mobSets, mobTypes, mobAmounts, errmsg);

	waveDescriptors->push_back(wdescr);
	return true;
}


bool WaveJSONReader::parseMobSetVector(std::vector<WaveDescriptor::MobSet>* mobsets, uString mobTypes, uString mobAmounts, std::string& errmsg) {
	
	int nTokensMtype = mobTypes.CountTokens2(',');
	int nTokensMamnt = mobAmounts.CountTokens2(',');

	if (nTokensMamnt == 0 || nTokensMtype == 0 || nTokensMamnt != nTokensMtype) {
		errmsg = "Invalid string in mob_types or mob_amounts";
		return false;
	}

	uString* strsAmount = new uString[nTokensMamnt];
	uString* strsType = new uString[nTokensMamnt];

	mobAmounts.SplitTokens2(',', strsAmount);
	mobTypes.SplitTokens2(',', strsType);

	for (int i = 0; i < nTokensMamnt; i++) {
		WaveDescriptor::MobSet mset;
		mset.amount = strsAmount[i].ToInt();
		mset.type	= strsType[i].ToInt();
		mobsets->push_back(mset);
	}

	delete[] strsAmount;
	delete[] strsType;
	return true;
}