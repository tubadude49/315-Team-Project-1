// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#include "Relation.h"


Relation::Relation(){
}


Relation::Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKeyIn) {
	name = relationName;
	for (int i = 0; i < attributeNames.size(); i++) {
		attributes.push_back(Attribute(attributeNames[i], attributeTypes[i]));
		if (primaryKeyIn == attributeNames[i]) {
			primaryKeyIndex = i;
		}
	}
	primaryKey = primaryKeyIn;
}

/* 	Destructor
*/
//Relation::~Relation() { //causes error
//}


bool Relation::is(string &name) {
	if (Relation::name == name) {
		return true;
	}
	else return false;
}


string& const Relation::getName() {
	return name;
}


string& const Relation::at(unsigned int tuple, unsigned int attribute) {
	return tuples[tuple][attribute];
}


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
	//return attributes[primaryKeyIndex].getName();
	return primaryKey;
}
