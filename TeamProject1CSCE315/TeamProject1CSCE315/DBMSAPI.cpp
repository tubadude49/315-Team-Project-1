// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include <iostream>

#include "DBMSAPI.h"

DBMSAPI::DBMSAPI() {
	dataManager = new DataManager();
	dmlParser = new DMLParser(dataManager);
}

DBMSAPI::~DBMSAPI() {
}


int DBMSAPI::execute(string &program) {
	return dmlParser->parseProgram(program);
}
