#pragma once

#include "DataManager.h"
#include "DMLParser.h"

class DBMSAPI {
private:
	DataManager* dataManager;
	DMLParser* dmlParser;
public:
	DBMSAPI();
	~DBMSAPI();
	void parseProgram(string &program, string &response);
};