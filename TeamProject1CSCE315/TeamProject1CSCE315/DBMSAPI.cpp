#pragma once

#include <iostream>

#include "DBMSAPI.h"

DBMSAPI::DBMSAPI() {
	dataManager = new DataManager();
	dmlParser = new DMLParser(dataManager);
}

DBMSAPI::~DBMSAPI() {
}

/* 	Send the parseable program to the DMLParser.
*/
void DBMSAPI::parseProgram(string &program, string &response) {
	dmlParser->parseProgram(program, response);
	cout << response << endl;
}
