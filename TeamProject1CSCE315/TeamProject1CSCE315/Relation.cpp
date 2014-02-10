// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "Relation.h"

/*	Constructor with field definitions.
Instantiating the relation without these fields is not possible.
*/
Relation::Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey) {
	name = relationName;
	for (int i = 0; i < attributeNames.size(); i++) {
		attributes.push_back(Attribute(attributeNames[i], attributeTypes[i]));
		if (primaryKey == attributeNames[i]) {
			primaryKeyIndex = i;
		}
	}
}

/* 	Destructor
*/
//Relation::~Relation() { //causes error
//}

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
string& const Relation::at(unsigned int tuple, unsigned int attribute) {
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

Attribute* Relation::getAttributeByName(string &attributeName) {
	for (Attribute attr : attributes) {
		if (attr.getName() == attributeName) {
			return &attr;
		}
	}
}

/*vector<string>& const Relation::getBuildCmds() {
	return buildCmds;
}

vector<Attribute>& const Relation::getAttributes()
{
	return attributes;
}

vector<vector<string>>& Relation::getTuples()
{
	return tuples;
}*/

unsigned int Relation::tupleSize(int pos)
{
	return tuples[pos].size();
}

unsigned int Relation::getPrimaryKeyIndex()
{
	return primaryKeyIndex;
}

string Relation::getPrimaryKey()
{
	return attributes[primaryKeyIndex].getName();
}
