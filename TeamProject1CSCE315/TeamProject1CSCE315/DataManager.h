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
	vector<Relation*> relations;
	Relation* getRelationByName(string &relationName);
public:
	DataManager();
	~DataManager();
	void create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes);
	void insert(string &relationName, vector<string> &values);
	string show(string &relationName);
	void drop(string &relationName);
	void write(string &relationName);
};
