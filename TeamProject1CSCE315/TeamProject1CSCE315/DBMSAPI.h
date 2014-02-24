// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
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
	int execute(string &program);
};