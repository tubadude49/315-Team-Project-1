// Thomas Bateson
// Jason Sitzman
// Colin Lenzen
// Josh Tutt
// 2/10/2014
// CSCE 315

#include "Attribute.h"

Attribute::Attribute(){
}

Attribute::Attribute(string& attributeName, string& attributeType) {
	name = attributeName;
	type = attributeType;
}

string& const Attribute::getName()
{
	return name;
}

string& const Attribute::getType()
{
	return type;
}