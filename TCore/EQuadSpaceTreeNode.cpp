#include "EQuadSpaceTreeNode.h"
#include "EEntity.h"


EQuadSpaceTreeNode::EQuadSpaceTreeNode()
	: m_bInitialized(false)
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
}

void EQuadSpaceTreeNode::Destroy()
{
	m_bInitialized = false;
}

void EQuadSpaceTreeNode::Register(EEntity* pEntity)
{
	if( m_EntityMap.find( pEntity->GetID() ) == m_EntityMap.end() )
	{
		m_EntityMap.insert( pEntity->GetID() );
		pEntity->AddSpaceID(m_SpaceID);
	}
}

void EQuadSpaceTreeNode::UnRegister(EEntity* pEntity)
{
	TYPE_ENTITY_MAP::iterator itr = m_EntityMap.find( pEntity->GetID() );

	if( itr == m_EntityMap.end() )
		assert(0);

	pEntity->RemoveSpaceID(m_SpaceID);
	m_EntityMap.erase( pEntity->GetID() );
}

bool EQuadSpaceTreeNode::IsInArea(EEntity* pEntity)
{
	const IAABB* pWorldAABB = pEntity->GetWorldAABB();

	if( pWorldAABB->IsValid() == false )
	{
		CVector3 worldPos = pEntity->GetWorldPos();
		return m_Area.IsIn(worldPos.x, worldPos.y );
	}

	CVector3 min = pWorldAABB->GetMin();
	CVector3 max = pWorldAABB->GetMax();

	if( min.x < m_Area.left ||
		max.x > m_Area.right ||
		min.y < m_Area.bottom ||
		max.y > m_Area.top )
		return false;

	return true;
}
