// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#pragma once

#include <vector>
#include <string>
#include "Relation.h"

/*	Only one DataManager should be instantiated at a time.
*/
class DataManager {
private:
	vector<Relation> database;
	Relation* getRelationByName(string &relationName);
public:
	DataManager();
	~DataManager();
	void create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey);
	void insert(string &relationName, vector<string> &values);
	void show(string &relationName, ostream& os);
	void drop(string &relationName);
	void write(string &relationName);

	void select(string &relationName);
	void project(string &relationName);
	void rename(string &relationName, string &relationNewName, vector<string> &attributeNewName);
	void setUnion(string &relationName1, string &relationName2);
	void setDifference(string &relationName1, string &relationName2);
	void crossProduct(string &relationName1, string &relationName2);
	void naturalJoin(string &relatioName1, string &relationName2);

	bool testRelation(string &relationName, vector<string> attrNames, vector<string> attrTypes, vector<vector<string>> tuples);

	void shellSort(vector<string> &toSort);
};
