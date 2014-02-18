// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "DataManager.h"

using namespace std;

class DataManager;

/* 	Main implementation of the DMLParser.
All functionality of the parser is here.
*/

class DMLParser {
private:
	DataManager* dataManager;
	/* Parse the program line by line */
	int parse(string &line);
	/* Parse a complex select, project, or rename statement */
	string parseComplex(vector<string> tokens, int startAt, string recursiveRelation);
public:
	/* Constructor for DMLParser */
	DMLParser(DataManager* dataManager);
	/* Destructor for DMLParser */
	~DMLParser();
	/* Parse an arbitrarily large program. 
	Output returned is the error code number.
	Refer to implementation for error code reference. */
	int parseProgram(string &program);
	/* Split the given program into the multiple statements that comprise it.
	*/
	vector<string> splitProgram(string &input);
	/* Split a given statement into a tokenized vector of strings.
	*/
	vector<string> split(string &input);
};
