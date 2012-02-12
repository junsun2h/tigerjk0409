#pragma once

#include "SGlobal.h"
#include "CGeometryConstructor.h"

#define  TRANSFORM_HELPER_EXTENT	50

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
	void						SelectMotion(wxString strMotion)		{ m_strMotion = strMotion;}
	wxString					GetSelectedMotion()						{ return m_strMotion; }
	void						SelectEntity(IEntity* pEntity, bool multiSelection = false);
	void						UnSelect();

	void						SetTransformMode(eTRANSFORM_MODE m)		{ m_TransformMode = m; }
	eTRANSFORM_MODE				GetTransformMode()						{ return m_TransformMode; }

	bool						GrabHelper(long x, long y);
	void						GrabUpdate(long x, long y);

	IEntity*					First();
	TYPE_SELECTED_ENTITIES*		List()		{ return& m_SelctionList; }

private:
	wxString					m_strMotion;

	eTRANSFORM_MODE				m_TransformMode;
	eDIRECTION					m_TransformAxis;
	
	XMMATRIX					m_GrabedTransform;
	CVector3 					m_GrabedPoint;

	TYPE_SELECTED_ENTITIES		m_SelctionList;
};