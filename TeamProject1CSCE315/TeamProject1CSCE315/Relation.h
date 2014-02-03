#pragma once

#include <string>
#include <vector>

using namespace std;

/* 	Private class, only used by DataManager.
No other class needs/should have access to Relations
*/
class Relation
{
	friend class DataManager;

	string name;
	vector<vector<string>> relation; //relation[attribute][tuple]
	vector<string> buildCmds;

	Relation(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes);
	~Relation();
	bool is(string &name);
	string& const getName();
	string const at(unsigned int i, unsigned int j);
	void addTuple(vector<string> &values);
	void addCmd(string &command);
	vector<string>& const getBuildCmds();
};