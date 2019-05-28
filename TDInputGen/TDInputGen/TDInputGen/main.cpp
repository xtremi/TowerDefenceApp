#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "jsoncons/json.hpp"
#include "jsoncons_ext/csv/csv_reader.hpp"
#include "jsoncons_ext/csv/csv_encoder.hpp"

#define ASSET_PATH "D:\\AGK\\AGK_libraries\\apps\\Test_app_tower_defence\\Test_app\\src\\main\\assets"

/*
$(SolutionDir)\..\..\Test_app\src\main\data_input_files\towers.csv 
$(SolutionDir)\..\..\Test_app\src\main\assets\json\tower_defs.json

$(SolutionDir)\..\..\Test_app\src\main\data_input_files\mobs.csv
$(SolutionDir)\..\..\Test_app\src\main\assets\json\mob_defs.json

$(SolutionDir)\..\..\Test_app\src\main\data_input_files\waves.csv
$(SolutionDir)\..\..\Test_app\src\main\assets\json\wave_defs.json

$(SolutionDir)\..\..\Test_app\src\main\data_input_files\bullets.csv
$(SolutionDir)\..\..\Test_app\src\main\assets\json\bullet_defs.json

*/


jsoncons::ojson* csvToJson(const std::string& csvpath, const std::string& headers);
bool writeJsonFile(const std::string& jsonpath, jsoncons::ojson* jsonData);

std::string getFileWithSuffix(const std::vector<std::filesystem::path>& paths, const std::string& suffix);
std::vector<std::string> getFilesWithSuffix(const std::vector<std::filesystem::path>& paths, const std::string& suffix);
std::vector<std::filesystem::path> getFilesAtPath(const std::string& path, const std::string& relativeTo);
 
std::string to3digitStr(int i);
static bool endsWith(const std::string& str, const std::string& suffix);


int processTowers(const std::string& csvPath, const std::string& jsonPath, bool dump = false);
int processMobs(const std::string& csvPath, const std::string& jsonPath, bool dump = false);
int processWaves(const std::string& csvPath, const std::string& jsonPath, bool dump = false);
int processBullets(const std::string& csvPath, const std::string& jsonPath, bool dump = false);
int processAnimations(const std::string& csvPath, const std::string& jsonPath, bool dump = false);

int main(int argc, char** argv){

	std::vector<std::string> args(argv + 1, argv + argc);
	if (args.size() < 8)
		return 1;

	bool dumpOutput = false;
	if (args.size() > 8) {
		if (args[8] == "dump")
			dumpOutput = true;
	}

	if (int r = processTowers(args[0], args[1], dumpOutput))
		return r + 10;
	if (int r = processMobs(args[2], args[3], dumpOutput))
		return r + 20;
	if (int r = processWaves(args[4], args[5], dumpOutput))
		return r + 30;
	if (int r = processBullets(args[6], args[7], dumpOutput))
		return r + 40;
	if (int r = processAnimations(args[8], args[9], dumpOutput))
		return r + 50;

	return 0;
}


int processTowers(const std::string& csvPath, const std::string& jsonPath, bool dumpOutput) {
	jsoncons::ojson* csvData = csvToJson(csvPath, "string,integer,string,float,float,float,string,float,float,float,float,boolean,boolean,float");
	if (!csvData)
		return 1;

	jsoncons::ojson* base = new jsoncons::ojson;
	base->insert_or_assign("towers", (*csvData));

	std::string relPath = ASSET_PATH;

	for (jsoncons::ojson& item : (*base)["towers"].array_range()) {

		std::string towerName = item["name"].as_string();

		std::string imagePath = relPath + "\\images\\towers\\" + towerName;
		std::vector<std::filesystem::path> imagePaths = getFilesAtPath(imagePath, relPath);

		std::vector<std::string> fixImages = getFilesWithSuffix(imagePaths, "_f.png");
		std::vector<std::string> rotImages = getFilesWithSuffix(imagePaths, "_r.png");
		std::vector<std::string> btnImages;
		btnImages.push_back(getFileWithSuffix(imagePaths, "_btn.png"));
		btnImages.push_back(getFileWithSuffix(imagePaths, "_btn_pressed.png"));
		btnImages.push_back(getFileWithSuffix(imagePaths, "_btn_inactive.png"));

		jsoncons::ojson fImgArr = jsoncons::ojson::array();
		jsoncons::ojson rImgArr = jsoncons::ojson::array();
		jsoncons::ojson bImgArr = jsoncons::ojson::array();

		for (std::string str : fixImages) fImgArr.push_back(str);
		for (std::string str : rotImages) rImgArr.push_back(str);
		for (std::string str : btnImages) bImgArr.push_back(str);

		item["images"]["fix"] = fImgArr;
		item["images"]["rot"] = rImgArr;
		item["images"]["btn"] = bImgArr;
	}

	if (!writeJsonFile(jsonPath, base))
		return 2;

	if(dumpOutput)
		std::cout << jsoncons::pretty_print((*base)) << std::endl;

	return 0;
}
int processMobs(const std::string& csvPath, const std::string& jsonPath, bool dumpOutput) {
	jsoncons::ojson* csvData = csvToJson(csvPath, "string,integer,float,float,float,float,float,float,float,float,float");
	if (!csvData)
		return 1;
	
	jsoncons::ojson* base = new jsoncons::ojson;
	base->insert_or_assign("mobs", (*csvData));

	std::string mobsImgPath = std::string("images/mobs/");
	for (jsoncons::ojson& item : (*base)["mobs"].array_range()) {
		std::string mobName = item["name"].as_string();
		std::string imgFile = mobsImgPath + mobName +"/" + mobName + ".png";
		item["image_path"] = imgFile;
	}


	if (!writeJsonFile(jsonPath, base))
		return 2;

	if (dumpOutput)
		std::cout << jsoncons::pretty_print((*base)) << std::endl;

	return 0;
}
int processWaves(const std::string& csvPath, const std::string& jsonPath, bool dumpOutput) {
	
	jsoncons::ojson* csvData = csvToJson(csvPath, "string,integer,float,float,string,string");
	if (!csvData)
		return 1;

	jsoncons::ojson* base = new jsoncons::ojson;
	base->insert_or_assign("waves", (*csvData));

	if (!writeJsonFile(jsonPath, base))
		return 2;

	if (dumpOutput)
		std::cout << jsoncons::pretty_print((*base)) << std::endl;

	return 0;
}

int processBullets(const std::string& csvPath, const std::string& jsonPath, bool dumpOutput) {
	jsoncons::ojson* csvData = csvToJson(csvPath, "integer,string,string");
	if (!csvData)
		return 1;

	jsoncons::ojson* base = new jsoncons::ojson;
	base->insert_or_assign("bullets", (*csvData));

	if (!writeJsonFile(jsonPath, base))
		return 2;

	if (dumpOutput)
		std::cout << jsoncons::pretty_print((*base)) << std::endl;

	return 0;
}

int processAnimations(const std::string& csvPath, const std::string& jsonPath, bool dumpOutput) {
	jsoncons::ojson* csvData = csvToJson(csvPath, "string,string,string,integer,integer,integer");
	if (!csvData)
		return 1;

	jsoncons::ojson* base = new jsoncons::ojson;
	base->insert_or_assign("animations", (*csvData));

	if (!writeJsonFile(jsonPath, base))
		return 2;

	if (dumpOutput)
		std::cout << jsoncons::pretty_print((*base)) << std::endl;

	return 0;
}

jsoncons::ojson* csvToJson(const std::string& csvpath, const std::string& headers) {

	std::ifstream csvFile(csvpath);

	if (!csvFile.is_open())
		return NULL;

	jsoncons::csv::csv_options opts;
	opts.assume_header(true).trim(true).column_types(headers);

	jsoncons::ojson* csvData = new jsoncons::ojson;
	(*csvData) = jsoncons::csv::decode_csv<jsoncons::ojson>(csvFile, opts);

	csvFile.close();

	return csvData;
}

std::vector<std::filesystem::path> getFilesAtPath(const std::string& path, const std::string& relativeTo){
	std::vector<std::filesystem::path> files;
	if(std::filesystem::exists(path)){
		for (const auto &entry : std::filesystem::directory_iterator(path)) {
			std::filesystem::path relPath = std::filesystem::relative(entry.path(), relativeTo);			
			files.push_back(relPath);
		}
	}
	return files;
}

std::string getFileWithSuffix(const std::vector<std::filesystem::path>& paths, const std::string& suffix) {
	std::string file;
	for (std::filesystem::path path : paths) {
		std::string filename = path.filename().generic_string();
		std::string filepath = path.generic_string();
		if (endsWith(filename, suffix))
			return filepath;
	}
	return "";
}

std::vector<std::string> getFilesWithSuffix(const std::vector<std::filesystem::path>& paths, const std::string& suffix){
	std::vector<std::string> files;
	for (std::filesystem::path path : paths) {
		std::string filename = path.filename().generic_string();
		std::string filepath = path.generic_string();
		if (endsWith(filename, suffix))
			files.push_back(filepath);
	}		
	return files;
}

bool writeJsonFile(const std::string& jsonpath, jsoncons::ojson* jsonData) {
	std::ofstream jsonfile;
	jsonfile.open(jsonpath);
	if (!jsonfile.is_open())
		return false;

	jsonfile << jsoncons::pretty_print((*jsonData));
	jsonfile.close();
	return true;
}




std::string to3digitStr(int i) {
	std::string str = std::to_string(i);
	std::string str3d = std::string(3 - str.length(), '0') + str;
	return str3d;
}

static bool endsWith(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}