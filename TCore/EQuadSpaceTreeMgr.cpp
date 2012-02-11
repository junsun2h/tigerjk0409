#include <atlcoll.h>

#include "CUnitPool.h"
#include "CFrustrum.h"
#include "CMathType.h"
#include "CQuad.h"
#include "CAABB.h"

#include "ISpaceMgr.h"
#include "IEntity.h"

#include "EQuadSpaceTreeNode.h"
#include "EQuadSpaceTreeMgr.h"


EQuadSpaceTreeMgr::EQuadSpaceTreeMgr()
{

}

EQuadSpaceTreeMgr::~EQuadSpaceTreeMgr()
{

}

void EQuadSpaceTreeMgr::Init(UINT worldSize, UINT divideCount)
{
	m_WorldSize = worldSize;
	m_DivideCount = divideCount;
	m_WorldStart = - int(m_WorldSize / 2);
	m_WorldUnitLength = m_WorldSize / m_DivideCount;

	m_SpaceNodePool.Init( divideCount * divideCount );
}

void EQuadSpaceTreeMgr::Destroy()
{
	POSITION pos = m_SpaceMap.GetStartPosition();
	TYPE_SPACE_NODE_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_SpaceMap.GetNext(pos);
		m_SpaceNodePool.Remove(itr->m_value);
	}

	m_SpaceMap.RemoveAll();
}

EQuadSpaceTreeNode* EQuadSpaceTreeMgr::GetSpace(float x, float y)
{
	float dx = x - m_WorldStart;
	float dy = y - m_WorldStart;
	
	int xIndex = int( dx/m_WorldUnitLength );
	int yIndex = int( dy/m_WorldUnitLength );

	if( xIndex < 0 || xIndex > m_DivideCount ||
		yIndex < 0 || yIndex > m_DivideCount )
	{
		assert(0);
		return NULL;
	}
	
	UINT index = xIndex * m_DivideCount + yIndex;

	return GetSpace(index);
}

EQuadSpaceTreeNode*	EQuadSpaceTreeMgr::GetSpace(UINT index)
{
	TYPE_SPACE_NODE_MAP::CPair* itr =  m_SpaceMap.Lookup(index);

	// make new space if there is no space node
	if( itr == NULL )
	{
		UINT xIndex = index % m_DivideCount;
		UINT yIndex = index / m_DivideCount;

		EQuadSpaceTreeNode* pNewNode = m_SpaceNodePool.GetNew();

		CQuad quad;
		quad.left = m_WorldStart + xIndex * m_WorldUnitLength;
		quad.bottom = m_WorldStart + yIndex * m_WorldUnitLength;
		quad.right = quad.left +  m_WorldUnitLength;
		quad.top = quad.bottom +  m_WorldUnitLength;

		pNewNode->Init(quad, index);

		m_SpaceMap.SetAt( index, pNewNode);

		return pNewNode;
	}

	return itr->m_value;
}

void EQuadSpaceTreeMgr::UpdateVisibleSpaceList(const CCAMERA_DESC* pCameraDesc)
{
	m_VisibleSpaceList.clear();

	CFrustumCull frustrum;
	frustrum.ConstructFrustum( *pCameraDesc );

	POSITION pos = m_SpaceMap.GetStartPosition();
	TYPE_SPACE_NODE_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_SpaceMap.GetNext(pos);
		EQuadSpaceTreeNode* pSpace = itr->m_value;

		CQuad area = pSpace->GetArea();
		CVector2 centerXY = area.Center();
		CVector3 center = CVector3( centerXY.x, centerXY.y, 0);

		CVector2 radiusXY = area.Radius();
		CVector3 radius = CVector3( radiusXY.x, radiusXY.y, 10000);
		
		if( frustrum.CheckRectangle( center, radius ) )
			m_VisibleSpaceList.push_back( pSpace );
	}
}

void EQuadSpaceTreeMgr::UpdateEntitySpace(IEntity* pEntity)
{	
	if( pEntity->IsVisible() == false )
		return;

	std::vector<EQuadSpaceTreeNode*> erasingList;

	// check former space list
	TYPE_SPACE_IDS* spaceIDList = pEntity->GetSpaceIDList();
	for(TYPE_SPACE_IDS::iterator itr = spaceIDList->begin() ; itr != spaceIDList->end(); itr++ )
	{
		UINT spaceID = *itr;	
		EQuadSpaceTreeNode* pNode = m_SpaceMap.Lookup( spaceID )->m_value;

		if( pNode->IsInArea(pEntity) == false)
			erasingList.push_back(pNode);
	}

	for( UINT i=0; i < erasingList.size(); ++i)
	{
		erasingList[i]->UnRegister(pEntity);
	}

	AssignSpace(pEntity);
}

void EQuadSpaceTreeMgr::AssignSpace(IEntity* pEntity)
{
	const CAABB* pAABB = pEntity->GetWorldAABB();

	if( pAABB->IsValid() == false)
	{
		CVector3 worldPos = pEntity->GetWorldPos();
		EQuadSpaceTreeNode*	pSpace = GetSpace( worldPos.x, worldPos.y );
		pSpace->Register(pEntity);
		return;
	}

	CVector3 min = pAABB->GetMin();
	CVector3 max = pAABB->GetMax();

	EQuadSpaceTreeNode*	pMinSpace = GetSpace( min.x, min.y);
	EQuadSpaceTreeNode*	pMaxSpace = GetSpace( max.x, max.y);

	UINT xMinIndex = pMinSpace->GetID() % m_DivideCount;
	UINT yMinIndex = pMinSpace->GetID() / m_DivideCount;

	UINT xMaxIndex = pMaxSpace->GetID() % m_DivideCount;
	UINT yMaxIndex = pMaxSpace->GetID() / m_DivideCount;

	for( UINT i = xMinIndex; i <=  xMaxIndex ; ++i)
	{
		for( UINT j = yMinIndex; j <= yMaxIndex; ++j )
		{
			GetSpace( i + j*m_DivideCount)->Register(pEntity);
		}
	}
}

void EQuadSpaceTreeMgr::Render()
{
	for( UINT i=0; i < m_VisibleSpaceList.size(); ++i)
		m_VisibleSpaceList[i]->Render();
}
