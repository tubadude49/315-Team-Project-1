// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#pragma once

#include <string>
#include <sstream>
#include <vector>

using namespace std;

class DataManager;

class DMLParser {
private:
	DataManager* dataManager;
	int parse(string &line);
public:
	DMLParser(DataManager* dataManager);
	~DMLParser();
	int parseProgram(string &program);
};
