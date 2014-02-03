#pragma once

#include <string>

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
};