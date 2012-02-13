#include <atlcoll.h>

#include "CUnitPool.h"
#include "CQuad.h"

#include "CAABB.h"
#include "IEntity.h"
#include "IEntityProxy.h"

#include "EQuadSpaceTreeNode.h"


EQuadSpaceTreeNode::EQuadSpaceTreeNode()
	: m_bInitialized(false)
	, m_bCulled(true)
{

}

EQuadSpaceTreeNode::~EQuadSpaceTreeNode()
{
	Destroy();
}

void EQuadSpaceTreeNode::Init(CQuad& area, UINT spaceID )
{
	m_Area = area;
	m_bInitialized = true;
	m_SpaceID = spaceID;
	m_bCulled = true;
}

void EQuadSpaceTreeNode::Destroy()
{
	m_bInitialized = false;
}

void EQuadSpaceTreeNode::Regist(IEntity* pEntity)
{
	TYPE_ENTITY_MAP::CPair* itr = m_EntityMap.Lookup( pEntity->GetID() );
	if( itr == NULL )
	{
		m_EntityMap.SetAt( pEntity->GetID(), pEntity );
		pEntity->AddSpaceID(m_SpaceID);
	}
}

void EQuadSpaceTreeNode::UnRegist(IEntity* pEntity)
{
	TYPE_ENTITY_MAP::CPair* itr = m_EntityMap.Lookup( pEntity->GetID() );
	if( itr == NULL )
		assert(0);

	pEntity->RemoveSpaceID(m_SpaceID);
	m_EntityMap.RemoveKey( pEntity->GetID() );
}

bool EQuadSpaceTreeNode::IsInArea(IEntity* pEntity)
{
	const CAABB* pEntityAABB = pEntity->GetLocalEntityAABB();

	if( pEntityAABB->IsValid() == false )
	{
		CVector3 worldPos = pEntity->GetWorldPos();
		return m_Area.IsIn(worldPos.x, worldPos.y );
	}

	CVector3 min = CVector3::Transform( pEntityAABB->GetMin(), pEntity->GetWorldTM() );
	CVector3 max = CVector3::Transform( pEntityAABB->GetMax(), pEntity->GetWorldTM() );

	if( min.x < m_Area.left ||
		max.x > m_Area.right ||
		min.y < m_Area.bottom ||
		max.y > m_Area.top )
		return false;

	return true;
}

void EQuadSpaceTreeNode::Render()
{
	POSITION pos = m_EntityMap.GetStartPosition();
	TYPE_ENTITY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_EntityMap.GetNext(pos);
		IEntityProxyRender* pProxy = (IEntityProxyRender*)itr->m_value->GetProxy(ENTITY_PROXY_RENDER);
		if( pProxy != NULL )
			pProxy->Render();
	}
}

void EQuadSpaceTreeNode::OnCulled()
{
	m_bCulled = true;

	POSITION pos = m_EntityMap.GetStartPosition();
	TYPE_ENTITY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_EntityMap.GetNext(pos);
		itr->m_value->SetCull( true );
	}
}

void EQuadSpaceTreeNode::OnBecomeVisible()
{
	m_bCulled = false;

	POSITION pos = m_EntityMap.GetStartPosition();
	TYPE_ENTITY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_EntityMap.GetNext(pos);
		itr->m_value->SetCull( false );
	}
}