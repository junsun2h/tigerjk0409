#pragma once


class EQuadSpaceTreeNode
{
	friend CObjectPool<EQuadSpaceTreeNode>;
	EQuadSpaceTreeNode();
	~EQuadSpaceTreeNode();

public:
	void		Init(CQuad&	area, UINT spaceID );
	void		Regist(IEntity* pEntity);
	void		UnRegist(IEntity* pEntity);
	UINT		GetID()						{ return m_SpaceID; }
	bool		IsInArea(IEntity* pEntity);
	CQuad		GetArea()					{ return m_Area; }
	void		Render();
	bool		IsCulled()					{ return m_bCulled; }

	void		OnCulled();
	void		OnBecomeVisible();

private:
	typedef	ATL::CAtlMap<long, IEntity*>	TYPE_ENTITY_MAP;

	TYPE_ENTITY_MAP		m_EntityMap;
	CQuad				m_Area;
	bool				m_bInitialized;
	UINT				m_SpaceID;
	bool				m_bCulled;
};
