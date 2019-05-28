#pragma once
#include "agk.h"
#include <iostream>
#include <string>

void checkJSONfile(const char* filepath);
void checkJSONelement(JSONElement* el);
void checkJSONarray(JSONArray* arr);
void checkJSONobject(JSONObject* obj);

class JSONReader {
public:
	JSONReader() {}

protected:
	JSONArray* checkJSONfile(char* filepath, std::string& errmsg);
	bool checkJSONelementType(const std::string& name, JSONElement* el, int wantedJSONtype, std::string& errmsg);

	bool JSONarrayToStdVector(JSONArray* jsonarr, std::vector<uString>* vs);
	bool JSONarrayToStdVector(JSONArray* jsonarr, std::vector<int>* vi);
	std::vector<int> toIntVector(const uString& str);

};