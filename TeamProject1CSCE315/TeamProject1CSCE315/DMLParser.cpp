// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DMLParser.h"

#include <iostream>

/* 	Main implementation of the DMLParser.
All functionality of the parser is here.
*/

/*
Parse Response Codes

0x0: Program returns without any non-0 responses.
0x1: Program encountered EXIT Command
0x2: parse(line) encountered an assignment which did not include "<-" as its second argument
0x3: invalid operator sent
0x4: missing keywords "VALUES" and "FROM" in "INSERT INTO"
0x5: missing ; at end of statement
0x6: missing opening "("

0xE: not yet implemented
*/
int DMLParser::parse(string &line) {
	//add try/catch for bounds checking (std::invalid_argument)
	vector<string> split = split(line);
	
	if(split[0] == "EXIT") { // IMPL
		return 0x1;
	}
	else if(split[0] == "UPDATE") {
		return 0xE;
	}
	else if(split[0] == "SHOW") {
		return 0xE;
	}
	else if(split[0] == "WRITE") {
		string outputRelation = split[1];
		return 0xE;
	}
	else if(split[0] == "OPEN") {
		string outputRelation = split[1];
		return 0xE;		
	}
	else if((split[0] == "CREATE") && (split[1] == "TABLE")) { // IMPL
		//CREATE TABLE
		string outputRelation = split[2];
		if(split[3] == "(") {
			vector<string> tuple;
			for(int i=4; split[i] != ")";i++) {
				tuple.push_back(split[i]);
			}
			dataManager.insert(outputRelation, tuple);
			dataManager.addBuildCmd(outputRelation, line);
		}
		else {
			return 0x6;
		}
	}
	else if((split[0] == "INSERT") && (split[1] == "INTO")) {
		string outputRelation = split[2];
		if((split[3] == "VALUES") && (split[4] == "FROM")) {
			if(split[5] == "(") {
				for(int i=6; split[i] != ")";i++) {
					
				}
			}
		}
		else {
			return 0x4;
		}
		return 0xE;
	}
	else if((split[0] == "DELETE") && (split[1] == "FROM"))
	else {
		string outputRelation = split[0];
		if(split[1] != "<-") return 0x2;
		if(split[2] == "project") {
			//PROJECT
			return 0xE;
		}
		else if(split[2] == "rename") {
			//RENAME
			return 0xE;
		}
		else if(split[2] == "select") {
			//SELECT
			return 0xE;
		}
		else {
			string firstRelation = split[2];
			string op = split[3];
			string secondRelation = split[4];
			if(op == "+") {
				dataManager.setUnion(firstRelation, secondRelation, outputRelation);
				dataManager.addBuildCmd(outputRelation, line);				
				if(split[5] != ";") return 0x5;
			}
			else if(op == "-") {
				dataManager.setDifference(firstRelation, secondRelation, outputRelation);
				dataManager.addBuildCmd(outputRelation, line);
				if(split[5] != ";") return 0x5;
			}
			else if(op == "*") {
				dataManager.crossProduct(firstRelation, secondRelation, outputRelation);
				dataManager.addBuildCmd(outputRelation, line);			
				if(split[5] != ";") return 0x5;
			}
			else if(op == "JOIN") {
				dataManager.naturalJoin(firstRelation, secondRelation, outputRelation);				
				dataManager.addBuildCmd(outputRelation, line);
				if(split[5] != ";") return 0x5;
			}
			else {
				return 0x3;
			}
		}
	}
	
	
	cout << "[DMLParser]: " << line << endl;
}

DMLParser::DMLParser(DataManager* dataManager) {
	DMLParser::dataManager = dataManager;
}

DMLParser::~DMLParser() {
}

int DMLParser::parseProgram(string &program) {
	vector<string> program = splitPrograms(program);
	for(string line : program) {
		int response;
		if(response = parse(line)) {
			return response;
		}
	}
	return 0x0;
}


static string terminalChars = " ,&<>=!()"; //TODO
static string removeChars = " ";
//static vector<char> terminalChars = { ',', '&', '<', '>', '=', '!', '(', ')' };
//static vector<char> removeChars = { ' ' };

vector<string> split(string &input){
	vector<string> splitString;
	size_t found = input.find_first_of(terminalChars);
	while (found != string::npos)
	{		
		input[found] = '*';
		found = input.find_first_of(removeChars, found + 1);
	}
	stringstream reader(input);
	string tmp;
	while (getline(reader, tmp, '*')){
		if (tmp != "") {
			splitString.push_back(tmp);
		}
	}
	return splitString;
}
