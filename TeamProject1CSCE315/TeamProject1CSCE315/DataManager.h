// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
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
	void addBuildCmd(string &relationName, string &buildCmd);
	void create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey);
	void insert(string &relationName, vector<string> &values);
	void update(string &relationName, vector<string> &oldValues, vector<string> &newValues);
	void del(string &relationName, vector<string> &values);
	void show(string &relationName, ostream& os);
	void drop(string &relationName);
	void write(string &relationName);

	void select(string &relationName, string &newRelationName, vector<string> booleanArgs);
	void project(string &relationName, string &newRelationName, vector<string> &newAttributes);
	void rename(string &relationName, string &relationNewName, vector<string> &attributeNewName);
	void setUnion(string &relationName1, string &relationName2, string &newRelationName);
	void setDifference(string &relationName1, string &relationName2, string &newRelationName);
	void crossProduct(string &relationName1, string &relationName2, string &newRelationName);
	void naturalJoin(string &relationName1, string &relationName2, string newName);

	//Helper function
	vector<string> tupleJoiner(vector<string> tuple1, vector<string> tuple2, int index1, int index2); 

	bool testRelation(string &relationName, vector<string> attrNames, vector<string> attrTypes, vector<vector<string>> tuples);

	void shellSort(vector<string> &toSort);

	void relationToFile(string relationName);
	vector<string> relationFromFile(string relationName, string fileName);
};
