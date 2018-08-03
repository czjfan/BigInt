#pragma once

#include <iostream>


#define CHAR_IDENTIFIERINIT(C)	(isalpha(C) || (C) == '_')
#define CHAR_VALID(C)			((C) >= 32 && (C) < 127)

inline bool identifierchar(char c)
{
	return (isdigit(c) || isalpha(c) || c == '_');
}
inline bool indentifierinitialchar(char c)
{
	return CHAR_IDENTIFIERINIT(c);
}
inline bool validchar(char c)
{
	return CHAR_VALID(c);
}
inline bool invalidchar(char c)
{
	return !CHAR_VALID(c);
}
inline bool numberhead(char c)
{
	return (isdigit(c) || c == '-' || c == '+');
}
inline bool spacechar(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}