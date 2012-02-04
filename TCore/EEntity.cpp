#include "EEntity.h"
#include "CDefine.h"
#include "EEngine.h"

EEntity::EEntity()
	: m_pParent(NULL)
{
}

void EEntity::Init(std::string& name, UINT id)
{
	m_LocalPos = CVector3(0,0,0);
	m_LocalScale = CVector3(1,1,1);
	m_LocalRotation = CQuat(0,0,0,1);
	m_WorldPos = CVector3(0,0,0);
	m_WorldRotation = CQuat(0,0,0,1);
	m_pParent = NULL;
	m_Name = name;
	m_ID = id;

	m_LocalTM = XMMatrixIdentity();
	m_WorldTM = XMMatrixIdentity();

	m_LocalEntityAABB.Reset();
	m_WorldAABB.Reset();
}

EEntity::~EEntity()
{
	Destroy();
}

void EEntity::Destroy()
{
	DeleteAllProxy();
	DetachAllChild();
	m_EventQueue.RemoveAll();

	if( m_pParent )
		m_pParent->DetachChild( this );

	if( m_EventQueue.GetSize() > 0 || 
		m_pParent != NULL || 
		m_ProxyMap.GetCount() > 0 || 
		m_Children.size() > 0)
		assert(0);

	m_SpaceIDList.clear();
	m_ID = -1;

	m_LocalEntityAABB.Reset();
	m_WorldAABB.Reset();
}

void EEntity::AddSpaceID(UINT id)
{
	TYPE_SPACE_IDS::iterator itr = m_SpaceIDList.begin();
	for( ; itr != m_SpaceIDList.end(); itr++ )
	{
		if( *itr == id)
			return;
	}

	m_SpaceIDList.push_back(id);
}

void EEntity::RemoveSpaceID(UINT id)
{
	TYPE_SPACE_IDS::iterator itr = m_SpaceIDList.begin();
	for( ; itr != m_SpaceIDList.end(); itr++ )
	{
		if( *itr == id)
		{
			m_SpaceIDList.erase(itr);
			return;
		}
	}
}

bool EEntity::IsVisible()
{
	if( GetProxy(ENTITY_PROXY_RENDER) == NULL )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
//    transform functions
//////////////////////////////////////////////////////////////////////////

void EEntity::SetLocalPos(const CVector3& _pos)
{
	if( m_LocalPos == _pos )
		return;

	m_LocalPos = _pos;
	UpdateLocalTM();
}

void EEntity::SetLocalScale(const CVector3& _scale)
{
	if( m_LocalScale == _scale )
		return;

	m_LocalScale = _scale;
	UpdateLocalTM();
}

void EEntity::SetLocalRot(const CQuat& _rot)
{
	if( m_LocalRotation == _rot )
		return;

	m_LocalRotation = _rot;
	UpdateLocalTM();
}

inline void EEntity::SetLocalTM(const XMMATRIX& tm)
{
	m_LocalTM = tm;
	XMMATRIX_UTIL::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);
	UpdateWorldTM();
}

inline void EEntity::UpdateLocalTM()
{
	m_LocalTM = XMMATRIX_UTIL::TransformationAffine(m_LocalScale, m_LocalRotation, m_LocalPos);
	UpdateWorldTM();

	int count = m_Children.size();
	for(int i= 0; i < count; i++)
	{
		m_Children[i]->UpdateWorldTM();
	}
}

void EEntity::SetWorldPos(const CVector3& _pos)
{
	m_WorldTM._41 = _pos.x;
	m_WorldTM._42 = _pos.y;
	m_WorldTM._43 = _pos.z;
	m_WorldPos = _pos;

	UpdateLocalFromWorld();
	OnTransformChanged();
}

inline void EEntity::SetWorldRot(const CQuat& _rot)
{
	m_WorldRotation = _rot;
	m_WorldTM = XMMATRIX_UTIL::TransformationAffine( CVector3(1,1,1), m_WorldRotation, m_WorldPos);
	
	UpdateLocalFromWorld();
	OnTransformChanged();
}

inline void EEntity::SetWorldTM(const XMMATRIX& tm)
{
	m_WorldTM = tm;
	XMMATRIX_UTIL::Decompose(NULL, &m_WorldRotation, &m_WorldPos, m_WorldTM);

	UpdateLocalFromWorld();
	OnTransformChanged();
}

inline void EEntity::UpdateWorldTM()
{
	if( m_pParent == NULL )
	{
		m_WorldTM = m_LocalTM;
		m_WorldPos = m_LocalPos;
		m_WorldRotation = m_LocalRotation;

		EntityEvent e;
		e.type = E_EVENT_TRANSFORM_CHANGED;
		SendEvent(e);
	}
	else
	{
		m_WorldTM = XMMatrixMultiply( m_LocalTM, m_pParent->GetWorldTM());
		XMMATRIX_UTIL::Decompose(NULL, &m_WorldRotation, &m_WorldPos, m_WorldTM);
	}

	OnTransformChanged();
}

void EEntity::UpdateLocalFromWorld()
{
	if( m_pParent )
	{
		XMMATRIX parentWorldInverse = XMMATRIX_UTIL::Inverse(NULL, m_pParent->GetWorldTM());
		m_LocalTM =  XMMatrixMultiply( m_WorldTM, parentWorldInverse );
		XMMATRIX_UTIL::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);
	}
	else
	{
		m_LocalTM =  m_WorldTM;		
		m_LocalPos = m_WorldPos;
		m_LocalRotation = m_WorldRotation;	
	}
}

void EEntity::OnTransformChanged()
{
	EAABB temp = m_WorldAABB;

	m_WorldAABB.Reset();
	if( m_LocalEntityAABB.IsValid() )
		m_WorldAABB.AddAABB( m_WorldTM, m_LocalEntityAABB.GetMin(), m_LocalEntityAABB.GetMax() );

	int count = m_Children.size();
	for(int i= 0; i < count; i++)
	{
		m_Children[i]->UpdateWorldTM();

		if( m_Children[i]->GetWorldAABB()->IsValid() )
			m_WorldAABB.AddAABB( m_Children[i]->GetWorldAABB() );
	}

	if( temp != m_WorldAABB )
	{
		g_Engine.SpaceMgr()->UpdateEntitySpace(this);

		if( m_pParent != NULL )
			m_pParent->UpdateWorldAABB();
	}

	EntityEvent e;
	e.type = E_EVENT_TRANSFORM_CHANGED;
	SendEvent(e);
}


void EEntity::MoveOnLocalAxis(float x, float y, float z)
{
	m_LocalPos += *(CVector3*)&m_LocalTM._11 * x;
	m_LocalPos += *(CVector3*)&m_LocalTM._21 * y;
	m_LocalPos += *(CVector3*)&m_LocalTM._31 * z;

	UpdateLocalTM();
}

void EEntity::RotateLocalAxis(CVector3 axis, float radian)
{
	m_WorldTM.r[3].x = 0;
	m_WorldTM.r[3].y = 0;
	m_WorldTM.r[3].z = 0;

	XMMATRIX rotTM = XMMatrixRotationAxis( axis.ToXMVEECTOR(), radian );

	rotTM = m_WorldTM * rotTM;

	rotTM.r[3].x = m_WorldPos.x;
	rotTM.r[3].y = m_WorldPos.y;
	rotTM.r[3].z = m_WorldPos.z;

	SetWorldTM(rotTM);
}


//////////////////////////////////////////////////////////////////////////
//    hierarchy functions
//////////////////////////////////////////////////////////////////////////

void EEntity::Reparent( IEntity* _pNewParent )
{
	if( _pNewParent == this || m_pParent == _pNewParent )
		return;

	if( m_pParent != NULL )
		m_pParent->DetachChild( this );

	if( _pNewParent == NULL )
	{
		m_pParent = NULL;
		SetLocalTM( GetWorldTM() );
		return;
	}
	else
	{
		m_pParent = (EEntity*)_pNewParent;
		XMMATRIX parentWorldInverse = XMMATRIX_UTIL::Inverse(NULL, m_pParent->GetWorldTM());
		m_LocalTM = XMMatrixMultiply( m_WorldTM , parentWorldInverse);	
		XMMATRIX_UTIL::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);
		UpdateWorldTM();
	}
}

void EEntity::AttachChild( IEntity* pChild )
{
	if( pChild == this )
		return;

	for( UINT i=0 ; i < m_Children.size(); ++i )
	{
		if( pChild == m_Children[i] )
			return;
	}

	m_Children.push_back( (EEntity*)pChild );
	pChild->Reparent( this );

	UpdateWorldAABB();
}

void EEntity::DetachChild( IEntity* _pChild )
{
	if( _pChild == this )
		return;

	TYPE_ENTITY_LIST::iterator itr = m_Children.begin();

	for( ; itr != m_Children.end(); itr++ )
	{
		IEntity* pChild = *itr;
		if( pChild == _pChild )
		{
			itr = m_Children.erase(itr);
			_pChild->Reparent( NULL );
			UpdateWorldAABB();
			return;
		}
	}
}

void EEntity::DetachAllChild()
{
	TYPE_ENTITY_LIST::iterator itr = m_Children.begin();

	for( ; itr != m_Children.end(); itr++ )
	{
		IEntity* pChild = *itr;
		itr = m_Children.erase(itr);
		pChild->Reparent( NULL );

		if( itr == m_Children.end() )
			break;
	}

	UpdateWorldAABB();
}

IEntity* EEntity::GetChild(UINT index )
{ 
	if( index >= m_Children.size() ) 
		return NULL; 

	return m_Children[index]; 
}

//////////////////////////////////////////////////////////////////////////
//   proxy
//////////////////////////////////////////////////////////////////////////

IEntityProxy* EEntity::GetProxy( eENTITY_PROXY_TYPE type, bool bCreate )
{
	ENEITY_PROXY_MAP::CPair* pEntityProxy = m_ProxyMap.Lookup( type );
	if( pEntityProxy != NULL )
		return pEntityProxy->m_value;

	if( bCreate )
		return CreateProxy(type);
	
	return NULL;
}

IEntityProxy* EEntity::CreateProxy( eENTITY_PROXY_TYPE type )
{
	EEngineMemoryMgr* pPool = g_Engine.EngMemoryPoolMgr();

	IEntityProxy* pProxy = GetProxy( type );
	if( pProxy != NULL)
		return pProxy;

	if( type == ENTITY_PROXY_ACTOR)			{	pProxy = pPool->GetNewProxy( ENTITY_PROXY_ACTOR );	}
	else if( type == ENTITY_PROXY_CAMERA)	{	pProxy = pPool->GetNewProxy( ENTITY_PROXY_CAMERA );	}
	else if( type == ENTITY_PROXY_RENDER )	{	pProxy = pPool->GetNewProxy( ENTITY_PROXY_RENDER );	}

	pProxy->Init(this);
	m_ProxyMap.SetAt( type, pProxy );
	return pProxy;
}

bool EEntity::DeleteProxy( eENTITY_PROXY_TYPE type )
{
	IEntityProxy* pProxy = GetProxy(type);

	if( pProxy == NULL )
		return false;

	g_Engine.EngMemoryPoolMgr()->RemoveProxy( pProxy );
	return true;
}

void EEntity::DeleteAllProxy()
{
	EEngineMemoryMgr* pPool = g_Engine.EngMemoryPoolMgr();

	POSITION pos = m_ProxyMap.GetStartPosition();
	ENEITY_PROXY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_ProxyMap.GetNext(pos);
		IEntityProxy* pProxy = itr->m_value;
		pPool->RemoveProxy(itr->m_value );
		m_ProxyMap.RemoveKey( itr->m_key );
	}
}

//////////////////////////////////////////////////////////////////////////
//   etc
//////////////////////////////////////////////////////////////////////////
void EEntity::SendEvent( EntityEvent &e )
{
	POSITION pos = m_ProxyMap.GetStartPosition();
	ENEITY_PROXY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_ProxyMap.GetNext(pos);
		IEntityProxy* pProxy = itr->m_value;
		pProxy->ProcessEvent(e);
	}
}

void EEntity::ADDLocalEntityAABB(CVector3 min, CVector3 max)
{
	m_LocalEntityAABB.AddAABB(min, max);
	UpdateWorldAABB();
}

void EEntity::UpdateWorldAABB()
{
	EAABB temp = m_WorldAABB;

	m_WorldAABB.Reset();
	if( m_LocalEntityAABB.IsValid() )
		m_WorldAABB.AddAABB( m_WorldTM, m_LocalEntityAABB.GetMin(), m_LocalEntityAABB.GetMax() );

	int count = m_Children.size();
	for(int i= 0; i < count; i++)
	{
		if(  m_Children[i]->GetWorldAABB()->IsValid() )
			m_WorldAABB.AddAABB( m_Children[i]->GetWorldAABB() );
	}

	if( temp != m_WorldAABB )
	{
		g_Engine.SpaceMgr()->UpdateEntitySpace(this);

		if( m_pParent != NULL )
			m_pParent->UpdateWorldAABB();
	}
}


bool EEntity::Pick(CVector3& from, CVector3& to, TYPE_ENTITY_LIST& list)
{
	if( m_LocalEntityAABB.IsValid() )
	{
		if( m_LocalEntityAABB.IsLineInBox( from, to, &m_WorldTM ) )
			list.push_back(this);
	}

	if( m_WorldAABB.IsValid() && m_WorldAABB.IsLineInBox( from, to ) )
	{
		int count = m_Children.size();
		for(int i= 0; i < count; i++)
		{
			m_Children[i]->Pick(from, to, list);
		}
	}

	if( list.size() > 0 )
		return true;
	else
		return false;
}