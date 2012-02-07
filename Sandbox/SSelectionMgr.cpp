#include "SSelectionMgr.h"
#include "SPropertyPanel.h"
#include "SMainMenuBar.h"
#include "CColiision.h"
#include "CLog.h"
#include "CPlane.h"

#define MAX_COLLISION_DISTANCE	10000000000.0f


void GeometryIntersect(CVector3& lineStart, CVector3& lineDir,
						CVertexPC* pVertex, int vertexCount, 
						uint16* pIndex, int primitiveCount, 
						eDIRECTION& grabDirection,
						float& fDistance, float& fClosestDistance,
						eDIRECTION iAxis)
{
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
				grabDirection = iAxis;
			}
		}
	}
};


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

		GeometryIntersect( lineStart, lineDir,
							pVertex, vertexCount, 
							pIndex, primitiveCount, 
							grabDirection,
							fDistance, fClosestDistance,
							eDIRECTION(iAxis) );
	}

	return fClosestDistance;
}


//------------------------------------------------------------------------------------------------------------------------------
bool GrabRotator(XMMATRIX& worldTM, CVector3& from, CVector3& to, eDIRECTION& grabDirection, float& fDistance )
{
	XMMATRIX invWorldTM = XMMATRIX_UTIL::Inverse( NULL, worldTM );

	CVector3 lineStart = CVector3::Transform( from, invWorldTM);
	CVector3 lineEnd = CVector3::Transform( to, invWorldTM);
	CVector3 lineDir = lineEnd - lineStart;

	CIRCLE_MAKE_PARAM param;
	param.radius = TRANSFORM_HELPER_EXTENT;
	param.direction = X_AXIS;
	param.segment = 10;

	CVertexPC* pVertex = NULL;
	uint16* pIndex = NULL;
	CGEOMETRY_CONSTRUCTOR::CreateCircle(param, &pVertex, &pIndex);

	fDistance = TransformHelperCollision(lineStart, lineDir, 
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
	param.offset = CVector3(TRANSFORM_HELPER_EXTENT, 0, 0);
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
	param.offset = CVector3(TRANSFORM_HELPER_EXTENT, 0, 0);

	CVertexPC* pVertex = NULL;
	uint16* pIndex = NULL;
	CGEOMETRY_CONSTRUCTOR::CreateBox(param, &pVertex, &pIndex);

	float fDistance = TransformHelperCollision(lineStart, lineDir, 
												pVertex, 8, 
												pIndex , 12, 
												grabDirection );

	float fCenterDistance = MAX_COLLISION_DISTANCE;
	for( int i =0 ; i < 8; ++i)
		pVertex[i].vPos.z -= TRANSFORM_HELPER_EXTENT;

	GeometryIntersect(lineStart, lineDir, 
						pVertex, 8, 
						pIndex , 12, 
						grabDirection,
						fCenterDistance, fDistance, CENTER );


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

	GLOBAL::PropertyPanel()->Empty();
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

	m_GrabedTransform = m_SelctionList[0]->GetWorldTM();
	
	XMMATRIX worldTransform = XMMatrixIdentity();
	worldTransform.r[3] = m_GrabedTransform.r[3];

	if( m_TransformMode == TRANSFORM_MOVE )
	{
		return GrabMover( worldTransform, origin, to, m_TransformAxis);
	}
	else if( m_TransformMode == TRANSFORM_ROTATE )
	{
		float fDistance;
		if( GrabRotator( worldTransform, origin, to, m_TransformAxis, fDistance) )
		{
			m_GrabedPoint = origin + direction * fDistance * GLOBAL::ObserverCamera()->GetDesc().farClip;
			return true;
		}
		return false;
	}
	else if( m_TransformMode == TRANSFORM_SCALE )
	{
		return GrabScaler( worldTransform, origin, to, m_TransformAxis);
	}

	return false;
}

void SSelectionMgr::GrabUpdate(long x, long y)
{
	if( m_SelctionList.size() == 0 )
		return;

	CVector3 pickFrom;
	CVector3 pickDir;
	GLOBAL::ObserverCamera()->GetPickRayFromScreen( x, y, pickFrom, pickDir);
	CVector3 pickTo = pickFrom + pickDir * GLOBAL::ObserverCamera()->GetDesc().farClip;

	CVector3 pa, pb;
	if( m_TransformMode == TRANSFORM_MOVE )
	{
		if( m_TransformAxis == X_AXIS )
		{
			COLLISION_UTIL::LineLineIntersect( m_GrabedTransform.r[3], CVector3(1000000,0,0), pickFrom, pickTo, &pa, &pb );

			CVector3 pos = m_GrabedTransform.r[3];
			pos.x = pa.x - TRANSFORM_HELPER_EXTENT;
			m_SelctionList[0]->SetWorldPos( pos);
		}
		else if( m_TransformAxis == Y_AXIS )
		{
			COLLISION_UTIL::LineLineIntersect( m_GrabedTransform.r[3], CVector3(0,1000000,0), pickFrom, pickTo, &pa, &pb );

			CVector3 pos = m_GrabedTransform.r[3];
			pos.y = pa.y - TRANSFORM_HELPER_EXTENT;
			m_SelctionList[0]->SetWorldPos( pos);
		}
		else
		{
			COLLISION_UTIL::LineLineIntersect( m_GrabedTransform.r[3], CVector3(0,0,1000000), pickFrom, pickTo, &pa, &pb );

			CVector3 pos = m_GrabedTransform.r[3];
			pos.z = pa.z - TRANSFORM_HELPER_EXTENT;
			m_SelctionList[0]->SetWorldPos( pos);
		}
	}
	else if( m_TransformMode == TRANSFORM_ROTATE )
	{
		CVector3 scale;
		CQuat rot;
		CVector3 pos;
		XMMATRIX rotTM;
		float d;

		if( XMMATRIX_UTIL::Decompose(&scale, &rot, &pos, m_GrabedTransform) == false )
			return;

		if( m_TransformAxis == X_AXIS )
		{
			if( m_GrabedTransform.r[3].x == 0 )
				d = 100000000.f;
			else
				d = 1/m_GrabedTransform.r[3].x;

			CPlane YZPlane(d, 0.f, 0.f, -1);

			CVector3 intersection;
			YZPlane.IntersectLine( pickFrom, pickTo, &intersection);

			CVector3 grabPointDirection = m_GrabedPoint - m_GrabedTransform.r[3];
			grabPointDirection.x = 0;
			CVector3 pickDirection = intersection - m_GrabedTransform.r[3];
			pickDirection.x = 0;

			grabPointDirection = CVector3::Normalize(grabPointDirection);
			pickDirection = CVector3::Normalize(pickDirection);

			float angle = acos( CVector3::Dot( grabPointDirection, pickDirection ) );
			CVector3 rotDirection = CVector3::Cross( grabPointDirection, pickDirection );
			if( rotDirection.x < 0 )
				angle *= -1;

			rotTM = XMMatrixMultiply( XMMatrixRotationQuaternion( rot.m128 ), XMMatrixRotationX(angle) );
		}
		else if( m_TransformAxis == Y_AXIS )
		{
			if( m_GrabedTransform.r[3].y == 0 )
				d = 100000000.f;
			else
				d = 1/m_GrabedTransform.r[3].y;

			CPlane XZPlane(0.f, d, 0.f, -1);

			CVector3 intersection;
			XZPlane.IntersectLine( pickFrom, pickTo, &intersection);

			CVector3 grabPointDirection = m_GrabedPoint - m_GrabedTransform.r[3];
			grabPointDirection.y = 0;
			CVector3 pickDirection = intersection - m_GrabedTransform.r[3];
			pickDirection.y = 0;

			grabPointDirection = CVector3::Normalize(grabPointDirection);
			pickDirection = CVector3::Normalize(pickDirection);

			float angle = acos( CVector3::Dot( grabPointDirection, pickDirection ) );
			CVector3 rotDirection = CVector3::Cross( grabPointDirection, pickDirection );
			if( rotDirection.y < 0 )
				angle *= -1;

			rotTM = XMMatrixMultiply( XMMatrixRotationQuaternion( rot.m128 ), XMMatrixRotationY(angle) );
		}
		else
		{ 
			if( m_GrabedTransform.r[3].z == 0 )
				d = 100000000.f;
			else
				d = 1/m_GrabedTransform.r[3].z;

			CPlane XYPlane(0.f, 0.f, d, -1);

			CVector3 intersection;
			XYPlane.IntersectLine( pickFrom, pickTo, &intersection);

			CVector3 grabPointDirection = m_GrabedPoint - m_GrabedTransform.r[3];
			grabPointDirection.z = 0;
			CVector3 pickDirection = intersection - m_GrabedTransform.r[3];
			pickDirection.z = 0;

			grabPointDirection = CVector3::Normalize(grabPointDirection);
			pickDirection = CVector3::Normalize(pickDirection);

			float angle = acos( CVector3::Dot( grabPointDirection, pickDirection ) );
			CVector3 rotDirection = CVector3::Cross( grabPointDirection, pickDirection );
			if( rotDirection.z < 0 )
				angle *= -1;

			rotTM = XMMatrixMultiply( XMMatrixRotationQuaternion( rot.m128 ), XMMatrixRotationZ(angle) );
		}

		XMMATRIX_UTIL::Decompose( NULL, &rot, NULL, rotTM);
		XMMATRIX tm  = XMMATRIX_UTIL::TransformationAffine(scale, rot, pos);
		m_SelctionList[0]->SetWorldTM( tm );
	}
	else if( m_TransformMode == TRANSFORM_SCALE )
	{
		CVector3 scale;
		CQuat rot;
		CVector3 pos;
		if( XMMATRIX_UTIL::Decompose(&scale, &rot, &pos, m_GrabedTransform) == false )
			return;

		if( m_TransformAxis == X_AXIS )
		{
			COLLISION_UTIL::LineLineIntersect( m_GrabedTransform.r[3], CVector3(1000000,0,0), pickFrom, pickTo, &pa, &pb );
			
			float offset = m_GrabedTransform.r[3].x + TRANSFORM_HELPER_EXTENT;
			float dx = pa.x - offset;
			float scaleX = scale.x + dx/TRANSFORM_HELPER_EXTENT;

			XMMATRIX tm  = XMMATRIX_UTIL::TransformationAffine(CVector3(scaleX, scale.y, scale.z), rot, pos);
			m_SelctionList[0]->SetWorldTM( tm );
		}
		else if( m_TransformAxis == Y_AXIS )
		{
			COLLISION_UTIL::LineLineIntersect( m_GrabedTransform.r[3], CVector3(0,1000000,0), pickFrom, pickTo, &pa, &pb );

			float offset = m_GrabedTransform.r[3].y + TRANSFORM_HELPER_EXTENT;
			float dy = pa.y - offset;
			float scaleY = scale.y + dy/TRANSFORM_HELPER_EXTENT;

			XMMATRIX tm  = XMMATRIX_UTIL::TransformationAffine(CVector3(scale.x, scaleY, scale.z), rot, pos);
			m_SelctionList[0]->SetWorldTM( tm );
		}
		else if( m_TransformAxis == Z_AXIS )
		{
			COLLISION_UTIL::LineLineIntersect( m_GrabedTransform.r[3], CVector3(0,0,1000000), pickFrom, pickTo, &pa, &pb );

			float offset = m_GrabedTransform.r[3].z + TRANSFORM_HELPER_EXTENT;
			float dz = pa.z - offset;
			float scaleZ = scale.z + dz/TRANSFORM_HELPER_EXTENT;

			XMMATRIX tm  = XMMATRIX_UTIL::TransformationAffine(CVector3(scale.x, scale.y, scaleZ), rot, pos);
			m_SelctionList[0]->SetWorldTM( tm );
		}
		else if( m_TransformAxis == CENTER)
		{
			COLLISION_UTIL::LineLineIntersect( m_GrabedTransform.r[3], CVector3(1000000,0,0), pickFrom, pickTo, &pa, &pb );

			float offset = m_GrabedTransform.r[3].x + TRANSFORM_HELPER_EXTENT;
			float d = pa.x - offset;

			CVector3 uniformScale;
			uniformScale.x = scale.x + d/TRANSFORM_HELPER_EXTENT;
			uniformScale.y = scale.y + d/TRANSFORM_HELPER_EXTENT;
			uniformScale.z = scale.z + d/TRANSFORM_HELPER_EXTENT;

			XMMATRIX tm  = XMMATRIX_UTIL::TransformationAffine(uniformScale, rot, pos);
			m_SelctionList[0]->SetWorldTM( tm );
		}
	}
}