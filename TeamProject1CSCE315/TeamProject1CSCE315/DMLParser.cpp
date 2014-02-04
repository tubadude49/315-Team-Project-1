#include "DMLParser.h"

#include <iostream>

/* 	Main implementation of the DMLParser.
All functionality of the parser is here.
*/
void DMLParser::parse(string &line, string &response) {
	//split keywords and variables based on terminal characters (i.e.) ' ', '&', '<', '*', ',', etc.
	//should we use vector<string> or string[]? <---------------------------------------------------
	//we need a split function either way^	
	cout << "[DMLParser]: " << line << endl;
}

DMLParser::DMLParser(DataManager* dataManager) {
	DMLParser::dataManager = dataManager;
}

DMLParser::~DMLParser() {
}

void DMLParser::parseProgram(string &program, string &response) {
	//split the lines of the program based on semicolons (;)
	//loop through for loop the lines and parse one at a time
	//for(;;) {
	string nuller = "";
	parse(nuller, response);
	//}
	response = "Completed!";
	// please don't run the above
}