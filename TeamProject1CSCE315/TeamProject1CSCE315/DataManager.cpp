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
		
		vector<Attribute> attrs = relation->attributes;
		vector<int> widths(attrs.size());
		for (int j = 0; j < attrs.size(); j++ )
		{
			Attribute attr = attrs[j];
			
			string typestr = attr.getType();
			int varcharPos = typestr.find("(");
			if((varcharPos != -1) && (typestr.find(" X ") == -1))//Type is VARCHAR and field is not crossed
			{
				string parenContent =  typestr.substr(varcharPos + 1, typestr.length() - 2 - varcharPos);
				widths[j] = stoi(parenContent);
			}
			
			string fullname = " " + attr.getName() + "[" + typestr + "]";
			int fieldLength = fullname.length();
			widths[j] = max(widths[j], fieldLength);//Either length of title or largest data
			os << setw(widths[j]) << fullname;
		}
		os << "\n";
		
		for (int i = 0; i < relation->tuples.size(); i++)
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
	vector<string> buildCmds = relation->buildCmds;
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
void DataManager::select(string &relationName) {
}

/*	select a subset of the attributes in a relation.
	TODO
*/
void DataManager::project(string &relationName) {
}

/*	rename the attributes in a relation.
	code by Thomas Bateson.
*/
void DataManager::rename(string &relationName, string &relationNewName, vector<string> &attributeNewName) {
	Relation* relation = getRelationByName(relationName);
	Relation newRelation = Relation(*relation);
	newRelation.name = relationNewName;
	for(int i=0;i<newRelation.attributes.size();i++) {
		newRelation.attributes[i] = Attribute(attributeNewName[i], relation->attributes[i].getType());
	}
	database.push_back(newRelation);
}

/*	compute the union of two relations; the relations must be union-compatible.
	code by Thomas Bateson.
*/
void DataManager::setUnion(string &relationName1, string &relationName2, string &newRelationName) {
	Relation* relation1 = getRelationByName(relationName1);
	Relation* relation2 = getRelationByName(relationName2);

	if (relation1->attributes.size() != relation2->attributes.size()) return;
	for (int i = 0; i < relation1->attributes.size();i++) {
		if (relation1->attributes[i].getName() != relation2->attributes[i].getName() ||
			relation1->attributes[i].getType() != relation2->attributes[i].getType()) {
			return;		//these two sets are not relatable or union-compatible			
		}
	}	

	Relation newRelation = Relation(*relation1);
	newRelation.name = newRelationName;
	newRelation.tuples.clear();

	for (int i = 0; i < relation1->tuples.size(); i++) {
		for (int h = 0; h < relation2->tuples.size(); h++) {		
			bool isSame = true;
			for (int j = 0; j < relation1->tuples[i].size(); j++) {
				if (relation1->tuples[i][j] != relation2->tuples[h][j]) {
					isSame = false;
					break;
				}				
				if (!isSame) break;
			}
			if (isSame) {
				newRelation.tuples.push_back(relation1->tuples[i]);
			}
		}
	}
	
	database.push_back(newRelation);
}

/*	compute the set difference of two relations; the relations must be union-compatible.
	Author: Josh Tutt
*/
void DataManager::setDifference(string &relationName1, string &relationName2) {
	//Create pointers to the indicated relations
	Relation* relationPointer1 = getRelationByName(relationName1);
	Relation* relationPointer2 = getRelationByName(relationName2);

	//Create a new relation with the same attributes, etc, as the first relation
	Relation relation1 = Relation(*relationPointer1);

	//Compute the difference: relation1 - relation2
	for (int i = 0; i < relationPointer2->tuples.size(); i++){
		for (int j = 0; j < relation1.tuples.size(); j++){
			if (relationPointer2->tuples[i] == relation1.tuples[j]){	//If a tuple is found to be in both relations...
				relation1.tuples.erase(relation1.tuples.begin() + j);	//...remove it from relation1
			}
		}
	}

	//Add relation1 to database

	//NOTE: setDifference() needs to be able to change the name of the newly-created deep-copied relation
	//relation1.setName("Difference: \"" + relationName1 + "\" - \"" + relationName2 + "\"");
	relation1.name = "Difference: \"" + relationName1 + "\" - \"" + relationName2 + "\"";
	database.push_back(relation1);
}


/*	compute the Cartesian product of two relations.
	TODO
*/
string DataManager::crossProduct(string &relationName1, string &relationName2) {
	Relation* rel1 = getRelationByName(relationName1);
	Relation* rel2 = getRelationByName(relationName2);
	vector<string> crossAttrNames;
	vector<string> crossAttrTypes;
	vector<vector<string>> crossTuples;
	string crossName = rel1->getName() + " X " + rel2->getName();
	string crossPrimaryKey = "{" + rel1->getPrimaryKey() + " X " + rel2->getPrimaryKey() + "}";
	for (int i = 0; i < rel1->attributes.size(); i++)
	{
		for (int j = 0; j < rel2->attributes.size(); j++)
		{
			string crossAtName = "{" + rel1->attributes[i].getName() + " X " + rel2->attributes[j].getName() + "}";
			crossAttrNames.push_back(crossAtName);
			string crossTyName = "(" + rel1->attributes[i].getType() + " X " + rel2->attributes[j].getType() + ")";
			crossAttrTypes.push_back(crossTyName);
		}
	}
	create(crossName, crossAttrNames, crossAttrTypes, crossPrimaryKey);

	return crossName;//returns new relation name
}

/*	In addition to these operations, we include the natural join operation. 
	The result of the natural join between relations R and S is the set of all combinations of tuples in R and S 
	that are equal on their common attribute names. The common attributes only appear once in the result.

	Natural join is expressible using the six fundamental operations,
	but a direct implementation for joins can reduce the need to use the (expensive) Cartesian product operation.

	TODO
*/
void DataManager::naturalJoin(string &relatioName1, string &relationName2) {
}

bool DataManager::testRelation(string &relationName, vector<string> attrNames, vector<string> attrTypes, vector<vector<string>> tuples) {	
	Relation* relation = getRelationByName(relationName);

	if (relation == NULL) return false;

	if (attrNames.size() != relation->attributes.size() || attrTypes.size() != relation->attributes.size()) return false;
	for (int i = 0; i < relation->attributes.size(); i++) {
		if (attrNames[i] != relation->attributes[i].getName()) return false;
		if (attrTypes[i] != relation->attributes[i].getType()) return false;
	}
	if (relation->tuples.size() != tuples.size()) return false;
	for (int i = 0; i < relation->tuples.size(); i++) {
		for (int j = 0; j < relation->tuples[i].size(); j++) {
			if (relation->tuples[i][j] != tuples[i][j]) return false;
		}
	}
	return true;
}
