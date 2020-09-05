#pragma once
#include "agk.h"
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "global.h"

void checkJSONfile(const char* filepath);
void checkJSONelement(JSONElement* el);
void checkJSONarray(JSONArray* arr);
void checkJSONobject(JSONObject* obj);

class JSONReader {
public:
	JSONReader() {}

protected:
	int sc = 0; int sc_sync = 8; //counter for Agk::Sync()

	JSONArray* checkJSONfile(char* filepath, std::string& errmsg);
	bool checkJSONelementType(const std::string& name, JSONElement* el, int wantedJSONtype, std::string& errmsg);
	JSONElement* getAndCheckJSONelement(const std::string& key, JSONObject* parentObj, int wantedJSONtype, std::string& errmsg);

	bool getValue(bool& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(float& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(int& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(uString& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(std::string& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(std::vector<std::string>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(std::vector<uString>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(std::vector<int>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(std::vector<float>& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);
	bool getValue(glm::vec2& val, const std::string& key, JSONObject* parentObj, std::string& errmsg);

	bool JSONarrayToStdVector(JSONArray* jsonarr, std::vector<std::string>* vs);
	bool JSONarrayToStdVector(JSONArray* jsonarr, std::vector<uString>* vs);
	bool JSONarrayToStdVector(JSONArray* jsonarr, std::vector<int>* vi);
	bool JSONarrayToStdVector(JSONArray* jsonarr, std::vector<float>* vi);
	bool JSONarrayToGLM2Dvec(JSONArray* jsonarr, glm::vec2* vec);
	std::vector<int> toIntVector(const uString& str);

};