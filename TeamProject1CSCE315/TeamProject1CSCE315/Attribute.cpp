#pragma once

#include "Attribute.h"

Attribute::Attribute(string& attributeName, string& attributeType) {
	name = attributeName;
	type = attributeType;
}
string Attribute::getName()
{
	return name;
}
string Attribute::getType()
{
	return type;
}