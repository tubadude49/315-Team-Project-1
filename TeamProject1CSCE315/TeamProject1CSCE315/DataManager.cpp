// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DataManager.h"
#include <iomanip>
#include <iostream>
using namespace std;

Relation* DataManager::getRelationByName(string &relationName) {
	// Loop through this backwards to search through newly created relations first and older ones last
	for (int i = relations.size() - 1; i >= 0; i--) {
		if (relations[i]->is(relationName)) {
			return relations[i];
		}
	}
	return NULL;
}

DataManager::DataManager() {
}

DataManager::~DataManager() {
}

/*	Create a new relation with the given attribute names and attribute types
*/
void DataManager::create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, unsigned int primaryKeyAt) {
	relations.push_back(new Relation(relationName, attributeNames, attributeTypes, primaryKeyAt));
}

/*	Insert a tuple (row) into a relation.
*/
void DataManager::insert(string &relationName, vector<string> &values) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
		relation->addTuple(values);
	}
}

void DataManager::show(string &relationName) {
	Relation* relation = getRelationByName(relationName);
	cout << relation->getName() << "\n";
	for(Attribute* attr : relation->getAttributes())
	{
		cout << setw(50) << attr->getName() << " [" << attr->getType() << "]";
	}
	cout << "\n";
	for(int i = 0; i < relation->numOfTuples(); i++)
	{
		for(int j = 0; j < relation->tupleSize(i); j++)
		{
			cout << setw(50) << relation->at(i,j);
		}
		cout << "\n";
	}
	cout << "\n";
}

/*	Remove the requested relation name from storage in this data manager.
Effectively deletes it from memory
*/
void DataManager::drop(string &relationName) {
	for (int i = relations.size() - 1; i >= 0; i--) {
		if (relations[i]->is(relationName)) {
			relations.erase(relations.begin() + i);
		}
	}
}

void DataManager::write(string &relationName) {
	Relation* relation = getRelationByName(relationName);
	vector<string> buildCmds = relation->getBuildCmds();
	//TODO: write the build commands to a file
}
