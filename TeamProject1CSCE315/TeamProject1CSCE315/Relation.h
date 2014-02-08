// Shell code by Thomas Bateson
// 2/3/2014
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
	vector<Attribute> attributes;
	vector<vector<string>> tuples; //relation[tuple][attribute]
	vector<string> buildCmds;

	Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey);
	//~Relation();
	bool is(string &name);
	string& const at(unsigned int i, unsigned int j);

	void addTuple(vector<string> &values);
	void addCmd(string &command);

	//string& const getName();
	Attribute* getAttributeByName(string &attributeName);
	//vector<string>& const getBuildCmds();
	//vector<Attribute>& const getAttributes();
	//vector<vector<string>>& getTuples();
	unsigned int tupleSize(int pos);
	unsigned int getPrimaryKeyIndex();
};
