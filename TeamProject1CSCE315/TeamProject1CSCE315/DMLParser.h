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
	void parse(string &line, string &response);
public:
	DMLParser(DataManager* dataManager);
	~DMLParser();
	void parseProgram(string &program, string &response);
	vector<string> split(string &input);
};
