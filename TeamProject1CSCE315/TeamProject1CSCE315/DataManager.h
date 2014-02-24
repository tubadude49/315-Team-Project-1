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
	/*	Get a relation by its name field.*/
	Relation* getRelationByName(string &relationName);
public:
	/*	Default Constructor.*/
	DataManager();

	/*	Default Destructor.*/
	~DataManager();

	/*	Create a new relation with the given attribute names and attribute types*/
	void create(string &relationName, vector<string> &attributeNames, vector<string> &attributeTypes, string &primaryKey);

	/*	Insert a tuple (row) into a relation.*/
	void insert(string &relationName, vector<string> &values);

	/*	Update a tuple to represent new information.
	Search by tuple (not index).
	*/
	void update(string &relationName, string &attribute, string &value, vector<string> &booleanArgs);

	/*	Delete a tuple.
	Search by tuple (not index).
	*/
	void del(string &relationName, vector<string> &booleanArgs);

	/*	Print the given relation to an ostream,
	If unsure, send cout as the ostream.
	*/
	void show(string &relationName, ostream& os);

	/*	Remove the requested relation name from storage in this data manager.
	Effectively deletes it from memory
	*/
	void drop(string &relationName);

	/*	Write the given relation to a file.*/
	void write(string &relationName);

	/*	select the tuples in a relation that satisfy a particular condition.
	code by Thomas Bateson
	*/
	void select(string &relationName, string &newRelationName, vector<string> booleanArgs);

	/*	select a subset of the attributes in a relation.
	code by Jason Sitzman
	*/
	void project(string &relationName, string &newRelationName, vector<string> &newAttributes);

	/*	rename the attributes in a relation.
	code by Thomas Bateson.
	*/
	void rename(string &relationName, string &relationNewName, vector<string> &attributeNewName);

	/*	compute the union of two relations; the relations must be union-compatible.
	code by Thomas Bateson.
	*/
	void setUnion(string &relationName1, string &relationName2, string &newRelationName);

	/*	compute the set difference of two relations; the relations must be union-compatible.
	Author: Josh Tutt
	*/
	void setDifference(string &relationName1, string &relationName2, string &newRelationName);

	/*	compute the Cartesian product of two relations.
	code by Colin Lenzen
	*/
	void crossProduct(string &relationName1, string &relationName2, string &newRelationName);

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
	void naturalJoin(string &relationName1, string &relationName2, string newName);

	//Helper function
	vector<string> tupleJoiner(vector<string> tuple1, vector<string> tuple2, int index1, int index2); 

	bool testRelation(string &relationName, vector<string> attrNames, vector<string> attrTypes, vector<vector<string>> tuples);

	/*	Perform a shell sort on a vector<string>.
	Best performance is O(n). Worst is O(n*log^2(n)).
	*/
	void shellSort(vector<string> &toSort);

	void relationToFile(string relationName);
	string relationFromFile(string relationName);
};
