#include "ThirdParty/tinyxml/tinyxml.h"


inline void GetAttribute(TiXmlNode* pNode, char* id, char* pValue )
{
	const char* str = pNode->ToElement()->Attribute(id);
	if( str == NULL)
		return;

	strcpy_s( pValue, 64, str);
}

inline void GetAttribute(TiXmlNode* pNode, char* id, bool& value )
{
	const char* str = pNode->ToElement()->Attribute(id);
	if( str == NULL)
		return;

	value = atoi(str) ? true : false;
}

inline void GetAttribute(TiXmlNode* pNode, char* id, int& value )
{
	const char* str = pNode->ToElement()->Attribute(id);
	if( str == NULL)
		return;

	value = atoi(str);
}

inline void GetAttribute(TiXmlNode* pNode, char* id, float& value )
{
	const char* str = pNode->ToElement()->Attribute(id);
	if( str == NULL)
		return;

	value = (float)atof(str);
}

