#include "JSONReader.h"

JSONArray* JSONReader::checkJSONfile(char* filepath, std::string& errmsg) {

	JSONElement* jsonbase = JSONElement::LoadJSONFromFile(filepath);
	if (!jsonbase)
		errmsg = "Error when parsing JSON file.";
	else if (jsonbase->GetType() != AGK_JSON_OBJECT)
		errmsg = "JSON data is not an object";
	else if (((JSONObject*)jsonbase)->m_iNumPairs != 1)
		errmsg = "Expected 1 JSON pair in object, got " + std::to_string(((JSONObject*)jsonbase)->m_iNumPairs);
	else {
		if (((JSONObject*)jsonbase)->m_pPairs[0]->m_pElement->GetType() != AGK_JSON_ARRAY)
			errmsg = "Expected first json object to be a JSON array";
		else {
			return (JSONArray*)(((JSONObject*)jsonbase)->m_pPairs[0]->m_pElement);
		}
	}
	return NULL;
}

bool JSONReader::getValue(bool& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_BOOL, errmsg)) {
		val = ((JSONBool*)e)->m_bValue;
		return true;
	}
	return false;
}

bool JSONReader::getValue(float& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_NUMBER, errmsg)){
		val = ((JSONNumber*)e)->m_fValue;
		return true;
	}
	return false;
}
bool JSONReader::getValue(int& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_NUMBER, errmsg)) {
		val = ((JSONNumber*)e)->m_iValue;
		return true;
	}
	return false;
}
bool JSONReader::getValue(uString& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_STRING, errmsg)) {
		val = ((JSONString*)e)->m_sValue;
		return true;
	}
	return false;
}

bool JSONReader::getValue(std::string& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	uString ustr;
	if (getValue(ustr, key, parentObj, errmsg)) {
		val = toStdStr(ustr);
		return true;
	}
	return false;
}

bool JSONReader::getValue(std::vector<uString>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_ARRAY, errmsg)) {
		return JSONarrayToStdVector((JSONArray*)e, &val);
	}
	return false;
}

bool JSONReader::getValue(std::vector<std::string>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_ARRAY, errmsg)) {
		return JSONarrayToStdVector((JSONArray*)e, &val);
	}
	return false;
}
bool JSONReader::getValue(std::vector<int>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_ARRAY, errmsg)) {
		return JSONarrayToStdVector((JSONArray*)e, &val);
	}
	return false;
}
bool JSONReader::getValue(std::vector<float>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_ARRAY, errmsg)) {
		return JSONarrayToStdVector((JSONArray*)e, &val);
	}
	return false;
}
bool JSONReader::getValue(glm::vec2& val, const std::string& key, JSONObject* parentObj, std::string& errmsg) {
	if (JSONElement* e = getAndCheckJSONelement(key, parentObj, AGK_JSON_ARRAY, errmsg)) {
		return JSONarrayToGLM2Dvec((JSONArray*)e, &val);
	}
	return false;
}

JSONElement* JSONReader::getAndCheckJSONelement(const std::string& key, JSONObject* parentObj, int wantedJSONtype, std::string& errmsg) {
	JSONElement* e = nullptr;
	if (e = parentObj->GetElement(&key[0])) {
		if (e->GetType() == 0 || !checkJSONelementType(key, e, wantedJSONtype, errmsg)) 
			return nullptr;
		return e;
	}
	errmsg = "Attribute " + key + " not found.";
	return nullptr;
}

bool JSONReader::checkJSONelementType(const std::string& name, JSONElement* el, int wantedJSONtype, std::string& errmsg) {
	if (sc++ == sc_sync) { agk::Sync(); sc = 0; }
	int t = el->GetType();
	if (el->GetType() != wantedJSONtype) {
		errmsg = "Attribute " + name + " is not of type " + std::string(el->TypeToString(wantedJSONtype));
		errmsg += (" (" + std::string(el->TypeToString(el->GetType())) + ")");
		return false;
	}
	return true;
}
bool JSONReader::JSONarrayToStdVector(JSONArray* jsonarr, std::vector<std::string>* vs){
	for (int i = 0; i < jsonarr->m_iNumElements; i++) {
		JSONElement* el = jsonarr->m_pElements[i];
		if (!el || el->GetType() != AGK_JSON_STRING) return false;

		std::string str = toStdStr(((JSONString*)el)->m_sValue);
		vs->push_back(str);
	}
	return true;

}

bool JSONReader::JSONarrayToStdVector(JSONArray* jsonarr, std::vector<uString>* vs) {
	for (int i = 0; i < jsonarr->m_iNumElements; i++) {
		JSONElement* el = jsonarr->m_pElements[i];
		if (!el || el->GetType() != AGK_JSON_STRING) return false;

		uString str = ((JSONString*)el)->m_sValue;
		vs->push_back(str);
	}
	return true;
}
bool JSONReader::JSONarrayToStdVector(JSONArray* jsonarr, std::vector<int>* vi) {
	for (int i = 0; i < jsonarr->m_iNumElements; i++) {
		JSONElement* el = jsonarr->m_pElements[i];
		if (!el || el->GetType() != AGK_JSON_NUMBER) return false;

		int ival = ((JSONNumber*)el)->m_iValue;
		vi->push_back(ival);
	}
	return true;
}

bool JSONReader::JSONarrayToStdVector(JSONArray* jsonarr, std::vector<float>* vi) {
	for (int i = 0; i < jsonarr->m_iNumElements; i++) {
		JSONElement* el = jsonarr->m_pElements[i];
		if (!el || el->GetType() != AGK_JSON_NUMBER) return false;

		float ival = ((JSONNumber*)el)->m_fValue;
		vi->push_back(ival);
	}
	return true;
}

bool JSONReader::JSONarrayToGLM2Dvec(JSONArray* jsonarr, glm::vec2* vec) {
	if (jsonarr->m_iNumElements < 2) 
		return false;
	
	for (int i = 0; i < 2; i++) {
		JSONElement* el = jsonarr->m_pElements[i];
		if (!el || el->GetType() != AGK_JSON_NUMBER) return false;

		(*vec)[i] = ((JSONNumber*)el)->m_fValue;
	}
	return true;
}

std::vector<int> JSONReader::toIntVector(const uString& str) {
	std::vector<int> intvec;
	uString strcpy = str;

	int ntks = strcpy.CountTokens2(',');
	uString* strarr = new uString[ntks];
	int res = strcpy.SplitTokens2((int)',', strarr);

	for (int i = 0; i < ntks; i++)
		intvec.push_back(strarr[i].ToInt());

	return intvec;
}


/**********************/

void checkJSONfile(const char* filepath) {
	JSONElement* jsonbase = JSONElement::LoadJSONFromFile(filepath);
	checkJSONelement(jsonbase);
}

void checkJSONobject(JSONObject* obj) {
	int nPairs = obj->m_iNumPairs;

	for (int i = 0; i < nPairs; i++) {
		JSONKeyPair* pair = obj->m_pPairs[i];
		uString name = pair->m_sName;
		JSONElement* el = pair->m_pElement;
		checkJSONelement(el);
	}
}

void checkJSONarray(JSONArray* arr) {
	int nElements = arr->m_iNumElements;

	for (int i = 0; i < nElements; i++) {
		JSONElement* eli = arr->GetElement(i);
		checkJSONelement(eli);
	}
}

void checkJSONelement(JSONElement* el) {
	if (el->GetType() == AGK_JSON_STRING) {
		JSONString* elStr = (JSONString*)el;
		uString val = elStr->m_sValue;
	}
	else if (el->GetType() == AGK_JSON_NUMBER) {
		JSONNumber* elNumb = (JSONNumber*)el;
		float val = elNumb->m_fValue;
	}
	else if (el->GetType() == AGK_JSON_BOOL) {
		JSONBool* elBool = (JSONBool*)el;
		bool val = elBool->m_bValue;
	}
	else if (el->GetType() == AGK_JSON_ARRAY) {
		JSONArray* elArr = (JSONArray*)el;
		checkJSONarray(elArr);
	}
	else if (el->GetType() == AGK_JSON_OBJECT) {
		checkJSONobject((JSONObject*)el);
	}
}