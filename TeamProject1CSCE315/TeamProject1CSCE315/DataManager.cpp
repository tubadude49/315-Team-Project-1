// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DataManager.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

/*	Get a relation by its name field.
*/
Relation* DataManager::getRelationByName(string &relationName) {
	// Loop through this backwards to search through newly created relations first and older ones last
	for (int i = relations.size() - 1; i >= 0; i--) {
		if (relations[i]->is(relationName)) {
			return relations[i];
		}
	}
	return NULL;
}

/* Default Constructor.
*/
DataManager::DataManager() {
}

/*	Default Destructor.
*/
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

/*	Print the given relation to an ostream,
	If unsure, send cout as the ostream.
*/
void DataManager::show(string &relationName, ostream& os) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
		os << relation->getName() << "\n";
		for (Attribute* attr : relation->getAttributes())
		{
			os << setw(10) << attr->getName() << " [" << attr->getType() << "]";
		}
		os << "\n";
		for (int i = 0; i < relation->getTuples().size(); i++)
		{
			for (int j = 0; j < relation->tupleSize(i); j++)
			{
				os << setw(10) << relation->at(i, j);
			}
			os << "\n";
		}
		os << "\n";
	}	
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

/*	Write the given relation to a file.
*/
void DataManager::write(string &relationName) {
	Relation* relation = getRelationByName(relationName);
	vector<string> buildCmds = relation->getBuildCmds();
	ofstream ofs(relationName + ".txt");
	for (int i = 0; i < buildCmds.size(); i++){
		ofs << buildCmds[i] << endl;
	}
	ofs.close();
}

/*	Perform a shell sort on a vector<string>.
	Best performance is O(n). Worst is O(n*log^2(n)).
*/
void DataManager::shellSort(vector<string> &toSort) {
	for (int h = toSort.size(); h /= 2;) {
		for (int i = h; i < toSort.size(); i++) {
			string k = toSort[i];
			int j;
			for (j = i; j >= h && k < toSort[j - h]; j -= h) {
				toSort[j] = toSort[j - h]; 							
			}
			toSort[j] = k; 										
		}
	}
}

/*	select the tuples in a relation that satisfy a particular condition.
	TODO
*/
Relation* DataManager::select(string &relationName) {
	return NULL;
}

/*	select a subset of the attributes in a relation.
	TODO
*/
Relation* DataManager::project(string &relationName) {
	return NULL;
}

/*	rename the attributes in a relation.
	TODO
*/
void DataManager::rename(string &relationName, string &attributeOldName, string &attributeNewName) {
}

/*	compute the union of two relations; the relations must be union-compatible.
	TODO
*/
Relation* DataManager::setUnion(string &relationName1, string &relationName2) {
	return NULL;
}

/*	compute the set difference of two relations; the relations must be union-compatible.
	TODO
*/
Relation* DataManager::setDifference(string &relationName1, string &relationName2) {
	return NULL;
}

/*	compute the Cartesian product of two relations.
	TODO
*/
Relation* DataManager::crossProduct(string &relationName1, string &relationName2) {
	return NULL;
}

/*	In addition to these operations, we include the natural join operation. 
	The result of the natural join between relations R and S is the set of all combinations of tuples in R and S 
	that are equal on their common attribute names. The common attributes only appear once in the result.

	Natural join is expressible using the six fundamental operations,
	but a direct implementation for joins can reduce the need to use the (expensive) Cartesian product operation.

	TODO
*/
Relation* DataManager::naturalJoin(string &relatioName1, string &relationName2) {
	return NULL;
}