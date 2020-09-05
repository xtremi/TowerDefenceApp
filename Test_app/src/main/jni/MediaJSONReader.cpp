#include "MediaJSONReader.h"

bool MediaJSONReader::readMedia(std::map<std::string, int>* shaders, char* jsonfile, std::string& errmsg) {
	return readShaders(shaders, jsonfile, errmsg);
}
bool MediaJSONReader::readShaders(std::map<std::string, int>* shaders, char* jsonfile, std::string& errmsg) {

	errmsg = "";
	JSONArray* sdescrArr = checkJSONfile(jsonfile, errmsg);
	if (!sdescrArr)
		return false;

	JSONElement* e = nullptr;
	std::string id = "", path = "";
	for (int i = 0; i < sdescrArr->m_iNumElements; i++) {
		if (!checkJSONelementType("shader element", sdescrArr->GetElement(i), AGK_JSON_OBJECT, errmsg))
			return false;

		JSONObject* obj = (JSONObject*)sdescrArr->GetElement(i);
		if (e = getAndCheckJSONelement("id",   obj, AGK_JSON_STRING, errmsg)) id   = toStdStr(((JSONString*)e)->m_sValue);
		if (e = getAndCheckJSONelement("path", obj, AGK_JSON_STRING, errmsg)) path = toStdStr(((JSONString*)e)->m_sValue);	
		
		if(!id.empty() && !path.empty())
			(*shaders)[id] = agk::LoadSpriteShader(&path[0]);
	}
	return true;

}
