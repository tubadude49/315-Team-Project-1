// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DMLParser.h"

#include <iostream>
#include <algorithm>

/* 	Main implementation of the DMLParser.
All functionality of the parser is here.
*/

/*
Parse Response Codes

0x0: Program returns without any non-0 responses.
0x1: Program encountered EXIT Command
0x2: parse(line) encountered an assignment which did not include "<-" as its second argument
0x3: invalid operator sent
0x4: missing keywords "VALUES FROM" in "INSERT INTO"
0x5: Unknown command, argument
0x6: missing opening "("
0x7: missing keyword "SET" in "UPDATE"
0x8: missing keyword "WHERE" in "UPDATE"

0xA: std::invalid_argument was thrown by DMLParser::parse

0xE: not yet implemented
*/
int DMLParser::parse(string &line) {
	//add try/catch for bounds checking (std::invalid_argument)
	vector<string> tokens = split(line);
	
	if (tokens[0] == "EXIT") {
		return 0x1;
	}
	else if (tokens[0] == "UPDATE") {
		string outputRelation = tokens[1];
		if (tokens[2] == "SET") {
			vector<string> setArgs;
			for (int i = 3; i < 6; i++) {
				setArgs.push_back(tokens[i]);
			}			
			if (tokens[6] == "WHERE") {
				vector<string> booleanArgs;
				for (int i = 7; i < 10; i++) {
					booleanArgs.push_back(tokens[i]);
				}
				dataManager->update(outputRelation, setArgs, booleanArgs);
				dataManager->addBuildCmd(outputRelation, line);
			}
			else {
				return 0x8;
			}
		}
		else {
			return 0x7;
		}
		return 0xE;
	}
	else if (tokens[0] == "SHOW") {
		string outputRelation = tokens[1];
		dataManager->show(outputRelation, cout);
	}
	else if (tokens[0] == "WRITE") {
		string outputRelation = tokens[1];
		dataManager->relationToFile(outputRelation);		
	}
	else if (tokens[0] == "OPEN") {
		string outputRelation = tokens[1];
		dataManager->relationFromFile(outputRelation, outputRelation + ".txt");
	}
	else if (tokens[0] == "CREATE TABLE") {
		string outputRelation = tokens[1];
		if (tokens[2] == "(") {
			vector<string> tuple;
			for (int i = 3; tokens[i] != ")"; i++) {
				tuple.push_back(tokens[i]);
			}
			dataManager->insert(outputRelation, tuple);
			dataManager->addBuildCmd(outputRelation, line);
		}
		else {
			return 0x6;
		}
	}
	else if (tokens[0] == "INSERT INTO") {
		string outputRelation = tokens[1];
		if (tokens[2] == "VALUES FROM") {
			if (tokens[3] == "(") {
				vector<string> values;
				for (int i = 4; tokens[i] != ")"; i++) {
					values.push_back(tokens[i]);
				}
				dataManager->insert(outputRelation, values);
				dataManager->addBuildCmd(outputRelation, line);
			}
			else {
				return 0x6;
			}
		}
		else {
			return 0x4;
		}
	}
	else if (tokens[0] == "DELETE FROM") {

	}
	else if (tokens[1] == "<-") {
		string outputRelation = tokens[0];
		if (tokens[2] == "project" || tokens[2] == "rename" || tokens[2] == "select") {
			if (tokens[3] == "(") {
				vector<string> newArgs;
				int oldRelationIndex = 0;
				for (int i = 0; tokens[i] != ")"; i++) {
					newArgs.push_back(tokens[i]);
				}
				string oldRelation = tokens[oldRelationIndex];
				if (tokens[2] == "project") {
					dataManager->project(oldRelation, outputRelation, newArgs);
					dataManager->addBuildCmd(outputRelation, line);
				}
				else if (tokens[2] == "rename") {
					dataManager->rename(oldRelation, outputRelation, newArgs);
					dataManager->addBuildCmd(outputRelation, line);
				}
				else if (tokens[2] == "select") {
					dataManager->select(oldRelation, outputRelation, newArgs);
					dataManager->addBuildCmd(outputRelation, line);
				}
			}
			else {
				return 0x6;
			}
		}	
		else {
			string firstRelation = tokens[2];
			string op = tokens[3];
			string secondRelation = tokens[4];
			if(op == "+") {
				dataManager->setUnion(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
			}
			else if(op == "-") {
				dataManager->setDifference(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
			}
			else if(op == "*") {
				dataManager->crossProduct(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
			}
			else if(op == "JOIN") {
				dataManager->naturalJoin(firstRelation, secondRelation, outputRelation);
				dataManager->addBuildCmd(outputRelation, line);
			}
			else {
				return 0x3;
			}
		}
	}
	else {
		return 0x5;
	}
		
	cout << "[DMLParser]: " << line << endl;
	return 0x0;
}

DMLParser::DMLParser(DataManager* dataManager) {
	DMLParser::dataManager = dataManager;
}

DMLParser::~DMLParser() {
}

int DMLParser::parseProgram(string &programs) {
	vector<string> program = splitProgram(programs);
	try {
		for (string line : program) {
			int error;
			if (error = parse(line)) {
				return error;
			}
		}
	}
	catch (std::invalid_argument) {
		return 0xA;
	}
	return 0x0;
}

/*assumptions: takes in a command string such as "SHOW animals; \n WRITE animals; \nOPEN animals;", and 
produces a vector<string> that contains { "SHOW animals", "WRITE animals", "OPEN animals" }*/
vector<string> DMLParser::splitProgram(string &input) {
	stringstream reader(input);
	vector<string> splitString;
	while (reader)
	{
		string next = "";
		if (reader.peek() != ';')
		{
			next += reader.get();
		}
		next += reader.get();
		splitString.push_back(next);
	}
	return splitString;
}

static string terminalChars = " ,&<>=!()"; //TODO
static string removeChars = " ";
//static vector<char> terminalChars = { ',', '&', '<', '>', '=', '!', '(', ')' };
//static vector<char> removeChars = { ' ' };

vector<string> DMLParser::split(string &input){
	vector<string> splitString;
	if ((input.size() == 0) || (input.at(input.size() - 1) != ';'))
	{
		cerr << "Invalid command. Missing ;\n";
		return splitString;
	}
	input = input.substr(0, input.size() - 1);//Remove semicolon
	std::replace(input.begin(),input.end(),',', ' ');
	stringstream reader(input);

	while (reader)//No eof or errors
	{
		string next = "";
		reader >> next;

		if (next.size() <= 0)
		{
			break;
		}

		if ((next.size() > 1) && next.find("(") == 0)//Seperate open paren from following data
		{
			splitString.push_back("(");
			next = next.substr(1);
		}
		if (next.find("\"") == 0)//Objects enclosed in quotes are one string 
		{
			int lastQ = next.find_last_of("\"");
			if (lastQ != 0)//no spaces
			{
				splitString.push_back(next.substr(1, lastQ - 1));
				string remainder = next.substr(lastQ + 1);
				if (remainder != "")
				{
					splitString.push_back(remainder);
				}
				continue;
			}

			next = next.substr(1);
			while (reader.peek() != 34)
			{
				next += reader.get();
			}
			reader.get();
		}

		if (next.size() > 1 && next.find("V") != 0 && next.find(")") == next.size() - 1)//Seperate ) if not VARCHAR
		{
			next = next.substr(0, next.size() - 1);
			splitString.push_back(next);
			splitString.push_back(")");
			continue;
		}
		
		splitString.push_back(next);
		if (next == "INSERT" || next == "CREATE" || next == "VALUES" || next == "PRIMARY" || next == "DELETE")//INSERT INTO,CREATE TABLE, VALUES FROM, & PRIMARY KEY are one token
		{
			reader >> next;
			splitString[splitString.size() - 1] += " " + next;
		}
	}
	return splitString;
}
