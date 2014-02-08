// Shell code by Thomas Bateson
// 2/3/2014
// CSCE 315

#include "DataManager.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

/*	Get a relation by its name field.
*/
Relation* DataManager::getRelationByName(string &relationName) {
	// Loop through this backwards to search through newly created relations first and older ones last
	for (int i = database.size() - 1; i >= 0; i--) {
		if (database[i].is(relationName)) {
			return &database[i];
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
void DataManager::create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey) {
	database.push_back(Relation(relationName, attributeNames, attributeTypes, primaryKey));
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
		
		vector<Attribute> attrs = relation->getAttributes();
		vector<int> widths(attrs.size());
		for (int j = 0; j < attrs.size(); j++ )
		{
			Attribute attr = attrs[j];
			
			string typestr = attr.getType();
			int varcharPos = typestr.find("(");
			if(varcharPos != -1)//Type is VARCHAR
			{
				string parenContent =  typestr.substr(varcharPos + 1, typestr.length() - 2 - varcharPos);
				widths[j] = stoi(parenContent);
			}
			
			widths[j] = max(widths[j],10); //Gives room for VARCHARS less than 10
			os << setw(widths[j]) << attr.getName() << " [" << typestr << "]";
		}
		os << "\n";
		
		for (int i = 0; i < relation->getTuples().size(); i++)
		{
			for (int j = 0; j < relation->tupleSize(i); j++)
			{
				os << setw(widths[j]) << relation->at(i, j);
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
	for (int i = database.size() - 1; i >= 0; i--) {
		if (database[i].is(relationName)) {
			database.erase(database.begin() + i);
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
*/
void DataManager::rename(string &relationName, vector<string> &attributeOldName, vector<string> &attributeNewName) {
	Relation* relation = getRelationByName(relationName);		
	for (int i = 0; i < attributeOldName.size();i++) {
		Attribute* attribute = relation->getAttributeByName(attributeOldName[i]);
		if (attribute != NULL) {
			attribute->setName(attributeNewName[i]);
			string s = attribute->getName();
			cout << "old name: " << attributeOldName[i] << " | new name: " << s << endl;
		}
	}
	database.push_back(*relation);
}

/*	compute the union of two relations; the relations must be union-compatible.
	TODO
*/
Relation* DataManager::setUnion(string &relationName1, string &relationName2) {
	return NULL;
}

/*	compute the set difference of two relations; the relations must be union-compatible.
*/
void DataManager::setDifference(string &relationName1, string &relationName2) {
	//Create pointers to the indicated relations
	Relation* relationPointer1 = getRelationByName(relationName1);
	Relation* relationPointer2 = getRelationByName(relationName2);

	//Create a new relation with the same attributes, etc, as relation1
	Relation r1 = *relationPointer1;

	//Compute the difference: relation1 - relation2
	for (int i = 0; i < relationPointer2->getTuples().size(); i++){
		for (int j = 0; j < r1.getTuples().size(); j++){
			if (relationPointer2->getTuples()[i] == r1.getTuples()[j]){	//If a tuple is found to be in both relations...
				r1.getTuples().erase(r1.getTuples().begin() + j);		//...remove it from r1
			}
		}
	}

	//Add r1 to the database
	database.push_back(r1);
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

bool DataManager::testRelation(string &relationName, vector<string> attrNames, vector<string> attrTypes, vector<vector<string>> tuples) {	
	Relation* relation = getRelationByName(relationName);

	vector<Attribute> relationAttributes = relation->getAttributes();
	if (attrNames.size() != relationAttributes.size() || attrTypes.size() != relationAttributes.size()) return false;
	for (int i = 0; i < relationAttributes.size(); i++) {
		if (attrNames[i] != relationAttributes[i].getName()) return false;
		if (attrTypes[i] != relationAttributes[i].getType()) return false;
	}
	vector<vector<string>> relationTuples = relation->getTuples();
	if (relationTuples.size() != tuples.size()) return false;
	for (int i = 0; i < relationTuples.size(); i++) {
		for (int j = 0; j < relationTuples[i].size(); j++) {
			if (relationTuples[i][j] != tuples[i][j]) return false;
		}
	}
	return true;
}
