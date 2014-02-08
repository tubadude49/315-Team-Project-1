#include "Attribute.h"

Attribute::Attribute(string& attributeName, string& attributeType) {
	name = attributeName;
	type = attributeType;
}

void Attribute::setName(string name) {
	Attribute::name = name;
}

string& const Attribute::getName()
{
	return name;
}

string& const Attribute::getType()
{
	return type;
}