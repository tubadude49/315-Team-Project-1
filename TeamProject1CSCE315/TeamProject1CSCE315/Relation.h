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
	vector<Attribute*> attributes;
	vector<vector<string>> tuples; //relation[attribute][tuple]
	vector<string> buildCmds;

	Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, unsigned int primaryKeyAt);
	~Relation();
	bool is(string &name);
	string& const getName();
	string const at(unsigned int i, unsigned int j);
	void addTuple(vector<string> &values);
	void addCmd(string &command);
	vector<string>& const getBuildCmds();
};
