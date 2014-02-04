// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#pragma once

#include "Relation.h"

/*	Constructor with field definitions.
Instantiating the relation without these fields is not possible.
*/
Relation::Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes) {
	name = relationName;
	relation.push_back(attributeNames);
	relation.push_back(attributeTypes);
}

/* 	Destructor
*/
Relation::~Relation() {
	/*if(relation.size()>0) {
	for(int i=0;i<relation.size();i++) {
	for(int j=0;j<relation.at(0).size();i++) {
	delete relation[i][j];
	}
	}
	}*/
}

/* 	Check the relations' name, true if there is a match, false otherwise.
*/
bool Relation::is(string &name) {
	if (Relation::name == name) {
		return true;
	}
	else return false;
}

/* 	Getter function for the name field.
*/
string& const Relation::getName() {
	return name;
}

/* 	Getter function for the relations field.
Call in the order of (attribute,tuple)
*/
string const Relation::at(unsigned int attribute, unsigned int tuple) {
	return relation[attribute][tuple];
}

/*	Add a new tuple (row) onto the relation field.
*/
void Relation::addTuple(vector<string> &values) {
	unsigned int requiredSize = relation[0].size();
	if (values.size() == requiredSize) { // This should never be false in a situation where the parsed program has no errors
		relation.push_back(values);
	}
}

void Relation::addCmd(string &command) {
	buildCmds.push_back(command);
}

vector<string>& const Relation::getBuildCmds() {
	return buildCmds;
}