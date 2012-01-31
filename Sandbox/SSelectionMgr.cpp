#include "SSelectionMgr.h"
#include "SPropertyPanel.h"
#include "SMainMenuBar.h"
#include "CColiision.h"

#define MAX_COLLISION_DISTANCE	10000000000.0f



float TransformHelperCollision(CVector3& lineStart, CVector3& lineDir,
								CVertexPC* pVertex, int vertexCount, 
								uint16* pIndex, int primitiveCount, 
								eDIRECTION& grabDirection)
{

	float fClosestDistance = MAX_COLLISION_DISTANCE;
	float fDistance = -1;
	for( int iAxis = 0; iAxis < 3; ++iAxis)
	{
		if( iAxis == 1)
		{
			for( int i =0 ; i < vertexCount; ++i)
				TSWAP( pVertex[i].vPos.x, pVertex[i].vPos.y);
		}
		else
		{
			for( int i =0 ; i < vertexCount; ++i)
				TSWAP( pVertex[i].vPos.y, pVertex[i].vPos.z);
		}


		for( int i = 0; i < primitiveCount; ++i)
		{
			int a = pIndex[i*3];
			int b = pIndex[i*3 + 1];
			int c = pIndex[i*3 + 2];

			if( COLLISION_UTIL::LineTriangleIntersect( lineStart, lineDir,	
				pVertex[a].vPos, pVertex[b].vPos, pVertex[c].vPos, 
				&fDistance) )
			{
				if( fDistance < fClosestDistance)
				{
					fClosestDistance = fDistance;

					if( iAxis == 0 )
						grabDirection = X_AXIS;
					else if( iAxis == 1)
						grabDirection = Y_AXIS;
					else
						grabDirection = Z_AXIS;
				}
			}
		}
	}

	return fClosestDistance;
}


//------------------------------------------------------------------------------------------------------------------------------
bool GrabRotator(XMMATRIX& worldTM, CVector3& from, CVector3& to, eDIRECTION& grabDirection )
{
	XMMATRIX invWorldTM = XMMATRIX_UTIL::Inverse( NULL, worldTM );

	CVector3 lineStart = CVector3::Transform( from, invWorldTM);
	CVector3 lineEnd = CVector3::Transform( to, invWorldTM);
	CVector3 lineDir = lineEnd - lineStart;

	CIRCLE_MAKE_PARAM param;
	param.radius = 50;
	param.direction = X_AXIS;
	param.segment = 10;

	CVertexPC* pVertex = NULL;
	uint16* pIndex = NULL;
	CGEOMETRY_CONSTRUCTOR::CreateCircle(param, &pVertex, &pIndex);

	float fDistance = TransformHelperCollision(lineStart, lineDir, 
												pVertex, param.segment, 
												pIndex , param.segment - 2, 
												grabDirection );
	
	SAFE_DELETE_ARRAY(pVertex);
	SAFE_DELETE_ARRAY(pIndex);

	if( fDistance != MAX_COLLISION_DISTANCE)
		return true;

	return false;
}

//------------------------------------------------------------------------------------------------------------------------------
bool GrabMover(XMMATRIX& worldTM, CVector3& from, CVector3& to, eDIRECTION& grabDirection )
{
	XMMATRIX invWorldTM = XMMATRIX_UTIL::Inverse( NULL, worldTM );

	CVector3 lineStart = CVector3::Transform( from, invWorldTM);
	CVector3 lineEnd = CVector3::Transform( to, invWorldTM);
	CVector3 lineDir = lineEnd - lineStart;

	CONE_MAKE_PARAM param;
	param.radius = 5;
	param.height = 15;
	param.direction = X_AXIS;
	param.offset = CVector3(50, 0, 0);
	param.segment = 10;

	CVertexPC* pVertex = NULL;
	uint16* pIndex = NULL;
	CGEOMETRY_CONSTRUCTOR::CreateCone(param, &pVertex, &pIndex);

	float fDistance = TransformHelperCollision(lineStart, lineDir, 
												pVertex, param.segment +1, 
												pIndex , param.segment * 2 - 1, 
												grabDirection );

	SAFE_DELETE_ARRAY(pVertex);
	SAFE_DELETE_ARRAY(pIndex);

	if( fDistance != MAX_COLLISION_DISTANCE)
		return true;

	return false;
}


//------------------------------------------------------------------------------------------------------------------------------
bool GrabScaler(XMMATRIX& worldTM, CVector3& from, CVector3& to, eDIRECTION& grabDirection )
{
	XMMATRIX invWorldTM = XMMATRIX_UTIL::Inverse( NULL, worldTM );

	CVector3 lineStart = CVector3::Transform( from, invWorldTM);
	CVector3 lineEnd = CVector3::Transform( to, invWorldTM);
	CVector3 lineDir = lineEnd - lineStart;

	BOX_MAKE_PARAM param;
	param.color = COLOR_RED;
	param.min = CVector3(-5.f,-5.f,-5.f);
	param.max = CVector3(5.f,5.f,5.f);
	param.offset = CVector3(50, 0, 0);

	CVertexPC* pVertex = NULL;
	uint16* pIndex = NULL;
	CGEOMETRY_CONSTRUCTOR::CreateBox(param, &pVertex, &pIndex);

	float fDistance = TransformHelperCollision(lineStart, lineDir, 
												pVertex, 8, 
												pIndex , 12, 
												grabDirection );

	SAFE_DELETE_ARRAY(pVertex);
	SAFE_DELETE_ARRAY(pIndex);

	if( fDistance != MAX_COLLISION_DISTANCE)
		return true;

	return false;
}



//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
SSelectionMgr::SSelectionMgr()
	: m_TransformMode(TRANSFORM_SELECT)
{

}

void SSelectionMgr::Clear()
{
	m_SelctionList.clear();

	GLOBAL::PropertyPanel()->SetEmpty();
}

void SSelectionMgr::Select(IEntity* pEntity, bool multiSelection)
{
	assert( pEntity);

	//	#define IsKeyDown(key) ((GetAsyncKeyState(key) & 0x8000)!=0)
	// IsKeyDown(VK_SHIFT)

	if( multiSelection == false )
		m_SelctionList.clear();

	m_SelctionList.push_back(pEntity);
	GLOBAL::PropertyPanel()->SetObject(pEntity);
	SMainMenuBar* pMainMenuBar = (SMainMenuBar*)GLOBAL::MainFrame()->GetMenuBar();
	pMainMenuBar->EnableComponent(true);
}

void SSelectionMgr::UnSelect()
{
	m_SelctionList.clear();

	SMainMenuBar* pMainMenuBar = (SMainMenuBar*)GLOBAL::MainFrame()->GetMenuBar();
	pMainMenuBar->EnableComponent(false);
}

IEntity* SSelectionMgr::First()
{
	if( m_SelctionList.size() > 0 )
		return m_SelctionList[0];

	return NULL;
}

bool SSelectionMgr::GrabHelper(long x, long y)
{
	if( m_SelctionList.size() == 0 )
		return false;

	CVector3 origin;
	CVector3 direction;
	GLOBAL::ObserverCamera()->GetPickRayFromScreen( x, y, origin, direction);
	CVector3 to = origin + direction * GLOBAL::ObserverCamera()->GetDesc().farClip;

	if( m_TransformMode == TRANSFORM_MOVE )
	{
		return GrabMover( m_SelctionList[0]->GetWorldTM(), origin, to, m_TransformAxis);
	}
	else if( m_TransformMode == TRANSFORM_ROTATE )
	{
		return GrabRotator(m_SelctionList[0]->GetWorldTM(), origin, to, m_TransformAxis);
	}
	else if( m_TransformMode == TRANSFORM_SCALE )
	{
		return GrabScaler(m_SelctionList[0]->GetWorldTM(), origin, to, m_TransformAxis);
	}

	return false;
}

void SSelectionMgr::GrabUpdate(long x, long y)
{
	if( m_SelctionList.size() == 0 )
		return;

	if( m_TransformMode == TRANSFORM_MOVE )
	{
		if( m_TransformAxis == X_AXIS )
		{

		}
		else if( m_TransformAxis == Y_AXIS )
		{

		}
		else
		{

		}
	}
	else if( m_TransformMode == TRANSFORM_ROTATE )
	{
		if( m_TransformAxis == X_AXIS )
		{

		}
		else if( m_TransformAxis == Y_AXIS )
		{

		}
		else
		{

		}
	}
	else if( m_TransformMode == TRANSFORM_SCALE )
	{
		if( m_TransformAxis == X_AXIS )
		{

		}
		else if( m_TransformAxis == Y_AXIS )
		{

		}
		else
		{

		}
	}
}