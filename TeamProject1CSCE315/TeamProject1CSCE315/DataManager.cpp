// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#pragma once

#include "DataManager.h"

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
void DataManager::create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes) {
	relations.push_back(new Relation(relationName, attributeNames, attributeTypes));
}

/*	Insert a tuple (row) into a relation.
*/
void DataManager::insert(string &relationName, vector<string> &values) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
		relation->addTuple(values);
	}
}

string DataManager::show(string &relationName) {
	//TODO: Beautiful ASCII output goes here.
	return "";
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
