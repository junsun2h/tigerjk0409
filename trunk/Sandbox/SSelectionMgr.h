#pragma once

#include "SGlobal.h"


typedef std::vector<IEntity*>	TYPE_SELECTED_ENTITIES;

enum eTRANSFORM_MODE
{
	TRANSFORM_SELECT,
	TRANSFORM_MOVE,
	TRANSFORM_ROTATE,
	TRANSFORM_SCALE
};

class SSelectionMgr
{
public:
	SSelectionMgr();

	void						Clear();
	void						Select(IEntity* pEntity, bool multiSelection = false);
	void						UnSelect();

	void						SetTransformMode(eTRANSFORM_MODE m)		{ m_TransformMode = m; }
	eTRANSFORM_MODE				GetTransformMode()						{ return m_TransformMode; }

	bool						GrabHelper(long x, long y);
	void						GrabUpdate(long x, long y);

	IEntity*					First();
	TYPE_SELECTED_ENTITIES*		List()		{ return& m_SelctionList; }

private:
	eTRANSFORM_MODE				m_TransformMode;
	eDIRECTION					m_TransformAxis;
	TYPE_SELECTED_ENTITIES		m_SelctionList;
};