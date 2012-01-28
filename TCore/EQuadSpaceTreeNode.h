#pragma once

#include "CUnitPool.h"
#include "CText.h"
#include <atlcoll.h>

class EEntity;
struct IAABB;


class EQuadSpaceTreeNode
{
	friend CObjectPool<EQuadSpaceTreeNode>;
	EQuadSpaceTreeNode();
	~EQuadSpaceTreeNode();

public:
	void		Init(CQuad&	area, UINT spaceID );
	void		Destroy();
	void		Register(EEntity* pEntity);
	void		UnRegister(EEntity* pEntity);
	UINT		GetID()						{ return m_SpaceID; }
	bool		IsInArea(EEntity* pEntity);
	CQuad		GetArea()					{ return m_Area; }
	void		Render();

private:
	typedef	ATL::CAtlMap<long, EEntity*>	TYPE_ENTITY_MAP;

	TYPE_ENTITY_MAP		m_EntityMap;
	CQuad				m_Area;
	bool				m_bInitialized;
	UINT				m_SpaceID;
};
