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


Relation* DataManager::getRelationByName(string &relationName) {
	// Loop through this backwards to search through newly created relations first and older ones last
	for (int i = database.size() - 1; i >= 0; i--) {
		if (database[i].is(relationName)) {
			return &database[i];
		}
	}
	return NULL;
}


DataManager::DataManager() {
}


DataManager::~DataManager() {
}


void DataManager::create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey) {
	database.push_back(Relation(relationName, attributeNames, attributeTypes, primaryKey));
}


void DataManager::insert(string &relationName, vector<string> &values) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
		relation->addTuple(values);
	}
}


void DataManager::update(string &relationName, vector<string> &oldValues, vector<string> &newValues) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
		for (int i = 0; i < relation->tuples.size(); i++) {
			if (relation->tuples[i] == oldValues) {
				relation->tuples[i] = newValues;
			}
		}
	}
}

void DataManager::del(string &relationName, vector<string> &values) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
		for (int i = 0; i < relation->tuples.size(); i++) {
			if (relation->tuples[i] == values) {
				relation->tuples.erase(relation->tuples.begin() + i);
			}
		}
	}
}

void DataManager::show(string &relationName, ostream& os) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {	
		os << relation->getName() << "\n";
		
		vector<Attribute> attrs = relation->attributes;
		vector<int> widths(attrs.size());
		for (int i = 0; i < attrs.size(); i++)
		{
			Attribute attr = attrs[i];
			
			string fullname = " " + attr.getName() + "[" + attr.getType() + "]";

			widths[i] = max(widths[i], (int)fullname.length());			//Either length of title or largest data
			os << setw(widths[i]) << fullname;

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
	else {
		os << "NULL relation: " << relationName << endl;
	}
}



void DataManager::drop(string &relationName) {
	for (int i = database.size() - 1; i >= 0; i--) {
		if (database[i].is(relationName)) {
			database.erase(database.begin() + i);
		}
	}
}


void DataManager::write(string &relationName) {
	Relation* relation = getRelationByName(relationName);
	vector<string> buildCmds = relation->buildCmds;
	ofstream ofs(relationName + ".txt");
	for (int i = 0; i < buildCmds.size(); i++){
		ofs << buildCmds[i] << endl;
	}
	ofs.close();
}


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


void DataManager::select(string &relationName, string &newRelationName, vector<string> booleanArgs) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
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
				toDelete[j]--;
			}
		}

		database.push_back(newRelation);
	}
}


void DataManager::project(string &relationName, string &newRelationName, vector<string> &newAttributes) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
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
}
	

void DataManager::rename(string &relationName, string &relationNewName, vector<string> &attributeNewName) {
	Relation* relation = getRelationByName(relationName);

	if (relation != NULL) {
		Relation newRelation = Relation(*relation);
		newRelation.name = relationNewName;
		for (int i = 0; i<newRelation.attributes.size(); i++) {
			newRelation.attributes[i] = Attribute(attributeNewName[i], relation->attributes[i].getType());
		}
		database.push_back(newRelation);
	}
}


void DataManager::setUnion(string &relationName1, string &relationName2, string &newRelationName) {
	Relation* relation1 = getRelationByName(relationName1);
	Relation* relation2 = getRelationByName(relationName2);

	Relation newRelation = Relation(*relation1);
	newRelation.name = newRelationName;
	newRelation.tuples.clear();
	for (int i = 0; i < relation1->tuples.size(); i++)//Add all tuples of first
	{
		newRelation.tuples.push_back(relation1->tuples[i]);
	}
	for (int j = 0; j < relation2->tuples.size(); j++)
	{
		bool notAdded = true;
		for (int i = 0; i < relation1->tuples.size(); i++)//Check if first already added it
		{
			bool isEqual = true;
			for (int x = 0; x < relation1->tuples[i].size(); x++)
			{
				if (relation1->tuples[i][x] != relation2->tuples[j][x])
				{
					isEqual = false;
					break;
				}
			}
			if (isEqual)
			{
				notAdded = false;
				break;
			}
		}
		if (notAdded)//Add unique tuples
		{
			newRelation.tuples.push_back(relation2->tuples[j]);
		}
	}
	
	database.push_back(newRelation);
}


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


void DataManager::crossProduct(string &relationName1, string &relationName2, string &newRelationName) {
	Relation* rel1 = getRelationByName(relationName1);
	Relation* rel2 = getRelationByName(relationName2);
	vector<string> crossAttrNames;
	vector<string> crossAttrTypes;
	string crossPrimaryKey = rel1->getPrimaryKey();
	for (int i = 0; i < rel1->attributes.size(); i++)
	{
		crossAttrNames.push_back(rel1->attributes[i].getName());
		crossAttrTypes.push_back(rel1->attributes[i].getType());		
	}
	for (int j = 0; j < rel2->attributes.size(); j++)
	{
		crossAttrNames.push_back(rel2->attributes[j].getName());
		crossAttrTypes.push_back(rel2->attributes[j].getType());
	}

	vector<vector<string>> tuples;
	for (int x = 0; x < rel1->tuples.size(); x++) {
		vector<string> baseTuple;
		for (int i = 0; i < rel1->tuples[x].size(); i++) {
			baseTuple.push_back(rel1->tuples[x][i]);
		}
		for (int y = 0; y < rel2->tuples.size(); y++) {
			vector<string> addTo;
			for (int i = 0; i < baseTuple.size(); i++) {
				addTo.push_back(baseTuple[i]);
			}
			for (int i = 0; i < rel2->tuples[y].size(); i++){
				addTo.push_back(rel2->tuples[y][i]);
			}
			tuples.push_back(addTo);
		}		
	}	

	create(newRelationName, crossAttrNames, crossAttrTypes, crossPrimaryKey);
	Relation* crossRel = getRelationByName(newRelationName);
	for (int i = 0; i < tuples.size(); i++)
	{
		crossRel->tuples.push_back(tuples[i]);
	}	
}


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

/*	Write the buildCmds of a Relation to a file
*/
void DataManager::relationToFile(string relationName){
	Relation* relation = getRelationByName(relationName);
	string name = relation->name + ".txt";
	ofstream out(name.c_str());
	out << "CREATE TABLE " << relation->name << "(";
	for (int i = 0; i < relation->attributes.size(); i++) {
		out << relation->attributes[i].getName() << " " << relation->attributes[i].getType();
		if (i + 1 < relation->attributes.size()) out << ", ";
	}
	out << ") PRIMARY KEY(" << relation->getPrimaryKey() << ");";
	for (int i = 0; i < relation->tuples.size(); i++){
		out << "INSERT INTO " << relation->name << " VALUES FROM (";
		for (int j = 0; j < relation->tuples[i].size(); j++){
			if (relation->attributes[j].getType()[0] == 'V') {
				out << '\"' << relation->tuples[i][j] << '\"';
			}
			else {
				out << relation->tuples[i][j];
			}
			if (j + 1 < relation->tuples[i].size()) out << ", ";
		}
		out << ");";
	}
	out.close();
}

/*	Read build commands from fileName and set them to the specified relation.
	If no relation specified (i.e., relationName = ""), just return the vector<string> of build commands
*/
string DataManager::relationFromFile(string relationName){
	string fileName = relationName + ".txt";
	ifstream in(fileName.c_str());
	string line;

	getline(in, line); 

	in.close();
	return line;
}
