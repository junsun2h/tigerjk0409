#pragma once

#include "SGlobal.h"


typedef std::vector<IEntity*>	TYPE_SELECTED_ENTITIES;


class SEntitySelection
{
public:
	void						Clear();
	void						Select(IEntity* pEntity, bool multiSelection = false);
	void						UnSelect();

	IEntity*					First();
	TYPE_SELECTED_ENTITIES*		List()		{ return& m_SelctionList; }

private:
	TYPE_SELECTED_ENTITIES		m_SelctionList;
};