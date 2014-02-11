// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
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

/*	Default Constructor.
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

/*	parse the given boolean statement either by string or integer comparisons
	internal function used by Select
	code by Thomas Bateson
*/
bool parseBooleanStmt(string type, string arg1, string op, string arg2) {
	if (type == "INTEGER") {
		int intArg1 = stoi(arg1);
		int intArg2 = stoi(arg2);
		if (op == "<") {
			return intArg1 < intArg2;
		}
		else if (op == ">") {
			return intArg1 > intArg2;
		}
		else if (op == "<=") {
			return intArg1 <= intArg2;
		}
		else if (op == ">=") {
			return intArg1 >= intArg2;
		}
		else if (op == "==") {
			return intArg1 == intArg2;
		}
		else if (op == "!=") {
			return intArg1 != intArg2;
		}
	}
	else {
		if (op == "<") {
			return arg1 < arg2;
		}
		else if (op == ">") {
			return arg1 > arg2;
		}
		else if (op == "<=") {
			return arg1 <= arg2;
		}
		else if (op == ">=") {
			return arg1 >= arg2;
		}
		else if (op == "==") {
			return arg1 == arg2;
		}
		else if (op == "!=") {
			return arg1 != arg2;
		}
	}
	return false;
}

/*	Compare a given tuple. Attributes are used for type information. 
	booleanArgs hold the necessarily comparison information.
	For internal use by Select
	code by Thomas Bateson
*/
bool compare(vector<Attribute> attributes, vector<string> tuple, vector<string> booleanArgs) {
	bool shouldKeep = false;
	for (int i = 0; i < attributes.size(); i++) {
		for (int j = 0; j < booleanArgs.size(); j += 4) {		
			string field = booleanArgs[j];
			string op = booleanArgs[j + 1];
			string constArg = booleanArgs[j + 2];
			if (j == 0) {
				if (attributes[i].getName() == field) {
					shouldKeep = parseBooleanStmt(attributes[i].getType(), tuple[i], op, constArg);
				}
			}
			else {
				if (attributes[i].getName() == field && booleanArgs[j - 1] == "&&") {
					shouldKeep = shouldKeep && parseBooleanStmt(attributes[i].getType(), tuple[i], op, constArg);
				}
				else if (attributes[i].getName() == field && booleanArgs[j - 1] == "||") {
					shouldKeep = shouldKeep || parseBooleanStmt(attributes[i].getType(), tuple[i], op, constArg);
				}
			}
		}
	}
	return shouldKeep;
}

/*	select the tuples in a relation that satisfy a particular condition.
	code by Thomas Bateson
*/
void DataManager::select(string &relationName, string &newRelationName, vector<string> booleanArgs) {
	Relation* relation = getRelationByName(relationName);
	Relation newRelation = Relation(*relation);
	newRelation.name = newRelationName;

	// Find the relations to remove
	vector<int> toDelete;
	for (int i = 0; i < newRelation.tuples.size(); i++) {
		if (!compare(newRelation.attributes, newRelation.tuples[i], booleanArgs)) {
			toDelete.push_back(i);
		}
	}

	// Remove the marked relations
	for (int i = 0; i < toDelete.size(); i++) {
		newRelation.tuples.erase(newRelation.tuples.begin() + toDelete[i]);
		for (int j = i + 1; j < toDelete.size(); j++) {
			toDelete[i]--;
		}
	}

	database.push_back(newRelation);
}

/*	select a subset of the attributes in a relation.
	code by Jason Sitzman
*/
void DataManager::project(string &relationName, string &newRelationName, vector<string> &newAttributes) {
	Relation* relation = getRelationByName(relationName);
	Relation newRelation = Relation(*relation);
	newRelation.name = newRelationName;
	
	vector<int> toRemove;
	for (int i = 0; i < newRelation.attributes.size(); i++) {
		bool keep = false;
		for (int j = 0; j < newAttributes.size(); j++) {
			if (newAttributes[j] == newRelation.attributes[i].getName()) {
				keep = true;
			}
		}
		if (!keep) {
			toRemove.push_back(i);
		}
	}

	for (int i = 0; i < toRemove.size(); i++) {
		newRelation.attributes.erase(newRelation.attributes.begin() + toRemove[i]);
		for (int j = 0; j < newRelation.tuples.size(); j++) {
			newRelation.tuples[j].erase(newRelation.tuples[j].begin() + toRemove[i]);
		}
		for (int j = i + 1; j < toRemove.size(); j++) {
			toRemove[j]--;
		}
	}
	database.push_back(newRelation);
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

	// Find similar tuples then add them to the new relation
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
void DataManager::setDifference(string &relationName1, string &relationName2, string &newRelationName) {
	//Create pointers to the indicated relations
	Relation* relation1 = getRelationByName(relationName1);
	Relation* relation2 = getRelationByName(relationName2);

	//Create a new relation with the same attributes, etc, as the first relation
	Relation newRelation = Relation(*relation1);
	newRelation.name = newRelationName;

	//Compute the difference: relation1 - relation2
	for (int i = 0; i < relation2->tuples.size(); i++){
		for (int j = 0; j < newRelation.tuples.size(); j++){
			if (relation2->tuples[i] == newRelation.tuples[j]){	//If a tuple is found to be in both relations...
				newRelation.tuples.erase(newRelation.tuples.begin() + j);	//...remove it from relation1
			}
		}
	}

	//Add relation1 to database
	database.push_back(newRelation);
}


/*	compute the Cartesian product of two relations.
	code by Colin Lenzen
*/
void DataManager::crossProduct(string &relationName1, string &relationName2, string &newRelationName) {
	Relation* rel1 = getRelationByName(relationName1);
	Relation* rel2 = getRelationByName(relationName2);
	vector<string> crossAttrNames;
	vector<string> crossAttrTypes;
	vector<vector<string>> crossTuples;
	//string crossName = rel1->getName() + " X " + rel2->getName();
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
	
	for (int i = 0; i < rel1->attributes.size(); i++)
	{
		for (int x = 0; x < rel1->tuples.size(); x++)
		{
			vector<string> curTuple;
			for (int y = 0; y < rel2->tuples.size(); y++)
			{
				for (int j = 0; j < rel2->attributes.size(); j++)
				{
					curTuple.push_back(rel1->tuples[x][i] + "~" + rel2->tuples[y][j]);
				}
			}
			crossTuples.push_back(curTuple);
			
		}
	}
	create(newRelationName, crossAttrNames, crossAttrTypes, crossPrimaryKey);
	Relation* crossRel = getRelationByName(newRelationName);
	for (int i = 0; i < crossTuples.size(); i++)
	{
		crossRel->addTuple(crossTuples[i]);
	}
	
}

/*	In addition to these operations, we include the natural join operation.
	The result of the natural join between relations R and S is the set of all combinations of tuples in R and S
	that are equal on their common attribute names. The common attributes only appear once in the result.

	Natural join is expressible using the six fundamental operations,
	but a direct implementation for joins can reduce the need to use the (expensive) Cartesian product operation.

	Author: Josh Tutt

	NOTE: Currently, this function creates a duplicate of the first relation if there are no shared attributes
	or attempts to create a new relation from a Natural Join of the two.
	However, it does not yet populate any rows of the new relation
	but only decides the correct attribute columns to include.
*/
void DataManager::naturalJoin(string &relationName1, string &relationName2, string newName) {
	//Create pointers to the indicated relations
	Relation* relationPointer1 = getRelationByName(relationName1);
	Relation* relationPointer2 = getRelationByName(relationName2);
	Relation joinedRelation;
	joinedRelation.name = newName;

	//Search for a common attribute
	int commonAttr1, commonAttr2 = -1; //Index numbers for a common attribute
	for (int i = 0; i < relationPointer1->attributes.size(); i++){
		for (int j = 0; j < relationPointer2->attributes.size(); j++){
			if (relationPointer1->attributes[i].getName() == relationPointer2->attributes[j].getName()){
				//If a common attribute is found, then write its index as commonAttr
				commonAttr1 = i;
				commonAttr2 = j;
			}
		}
	}

	//If no common attribute exists, create a duplicate of relation1 and exit the function
	if (commonAttr1 == -1 || commonAttr2 == -1){
		joinedRelation = Relation(*relationPointer1);
		database.push_back(joinedRelation);
		return;
	}

	//Create a list from the union of the two attribute sets
	vector<Attribute> newAttributes(0);
	for (int i = 0; i < relationPointer1->attributes.size(); i++){ //Add all attributes from relation1
		if (i == commonAttr1)
			continue;
		newAttributes.push_back(relationPointer1->attributes[i]);
	}
	newAttributes.push_back(relationPointer1->attributes[commonAttr1]);
	for (int j = 0; j < relationPointer2->attributes.size(); j++){
		if (j == commonAttr2)
			continue;
		newAttributes.push_back(relationPointer2->attributes[j]);
	}

	//Select the sets of tuples, and make deep copies
	vector<vector<string>> relation1Tuples = vector<vector<string>>(relationPointer1->tuples);
	vector<vector<string>> relation2Tuples = vector<vector<string>>(relationPointer2->tuples);

	//Create a new set of tuples
	vector<vector<string>> newTuples(0);
	vector<string> tuple(0);

	for (int i = 0; i < relation1Tuples.size(); i++){
		for (int j = 0; j < relation2Tuples.size(); j++){
			if (relation1Tuples[i][commonAttr1] == relation2Tuples[j][commonAttr2]){
				tuple = tupleJoiner(relation1Tuples[i], relation2Tuples[j], commonAttr1, commonAttr2);
				newTuples.push_back(tuple);
			}
		}
	}

	//Set newAttributes and newTuples to joinedRelation
	joinedRelation.attributes = newAttributes;
	joinedRelation.tuples = newTuples;

	//Add joinedRelation to the database
	database.push_back(joinedRelation);
}

/*	Helper function for naturalJoin()
*/
vector<string> DataManager::tupleJoiner(vector<string> tuple1, vector<string> tuple2, int index1, int index2){
	vector<string> newTuple(0);
	for (int i = 0; i < tuple1.size(); i++){
		if (i == index1)
			continue;
		newTuple.push_back(tuple1[i]);
	}
	newTuple.push_back(tuple1[index1]);
	for (int j = 0; j < tuple2.size(); j++){
		if (j == index2)
			continue;
		newTuple.push_back(tuple2[j]);
	}
	return newTuple;
}


/*	Tester function to determine if a given relation is "equal" to the given field information
	For test purposes only***
	code by Thomas Bateson
*/
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
