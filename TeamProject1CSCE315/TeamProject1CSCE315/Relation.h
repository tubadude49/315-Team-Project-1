// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#pragma once

#include <string>
#include <vector>

#include "Attribute.h"

using namespace std;

/* 	Private class, only used by DataManager.
No other class needs/should have access to Relations
*/
class Relation
{
	friend class DataManager;

	string name;
	unsigned int primaryKeyIndex;
	string primaryKey;
	vector<Attribute> attributes;
	vector<vector<string>> tuples; //relation[tuple][attribute]
	vector<string> buildCmds;
	
	/*	Default constructor
	*/
	Relation();
	/*	Constructor with field definitions.
	Instantiating the relation without these fields is not possible.
	*/
	Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey);
	//~Relation();
	/* 	Check the relations' name, true if there is a match, false otherwise.
	*/
	bool is(string &name);
	/* 	Getter function for the relations field.
	Call in the order of (attribute,tuple)
	*/
	string& const at(unsigned int i, unsigned int j);
	/*	Add a new tuple (row) onto the relation field.
	*/
	void addTuple(vector<string> &values);
	void addCmd(string &command);
	/* 	Getter function for the name field.
	*/
	string& const getName();
	Attribute* getAttributeByName(string &attributeName);
	//vector<string>& const getBuildCmds();
	//vector<Attribute>& const getAttributes();
	//vector<vector<string>>& getTuples();
	unsigned int tupleSize(int pos);
	unsigned int getPrimaryKeyIndex();
	string getPrimaryKey();
};
