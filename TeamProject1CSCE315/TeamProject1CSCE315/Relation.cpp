// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#pragma once

#include "Relation.h"

/*	Constructor with field definitions.
Instantiating the relation without these fields is not possible.
*/
Relation::Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, unsigned int primaryKeyAt) {
	name = relationName;
	primaryKeyIndex = primaryKeyAt;
	for (int i = 0; i < attributeNames.size(); i++) {
		attributes.push_back(new Attribute(attributeNames[i], attributeTypes[i]));
	}
	//relation.push_back(attributeNames);
	//relation.push_back(attributeTypes);
}

/* 	Destructor
*/
Relation::~Relation() {
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
string const Relation::at(unsigned int tuple, unsigned int attribute) {
	return tuples[tuple][attribute];
}

/*	Add a new tuple (row) onto the relation field.
*/
void Relation::addTuple(vector<string> &values) {
	unsigned int requiredSize = attributes.size();
	if (values.size() == requiredSize) { // This should never be false in a situation where the parsed program has no errors
		tuples.push_back(values);
	}
}

void Relation::addCmd(string &command) {
	buildCmds.push_back(command);
}

vector<string>& const Relation::getBuildCmds() {
	return buildCmds;
}

vector<Attribute*> Relation::getAttributes()
{
	return attributes;
}

int Relation::numOfTuples()
{
	return tuples.size();
}

int Relation::tupleSize(int pos)
{
	return tuples[pos].size();
}