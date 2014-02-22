// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

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
	/* 	Send the parseable program to the DMLParser.
	*/
	void parseProgram(string &program, string &response);
};