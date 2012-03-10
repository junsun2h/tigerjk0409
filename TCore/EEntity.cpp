#include <atlcoll.h>

#include "CGrowableArray.h"
#include "CUnitPool.h"
#include "CAABB.h"
#include "CCamera.h"
#include "CRenderElement.h"
#include "CLight.h"

#include "IRDevice.h"
#include "IMotionInstance.h"
#include "ISpaceMgr.h"
#include "IEntityProxy.h"
#include "IEntity.h"
#include "IActorMgr.h"

#include "EEntityProxyActor.h"
#include "EEntityProxyCamera.h"
#include "EEntityProxyRender.h"
#include "EGlobal.h"
#include "EEntity.h"


CObjectPool<EEntityProxyCamera>	g_MemPoolProxyCamera(100);
CObjectPool<EEntityProxyRender>	g_MemPoolProxyRender(100);

void RemoveProxyFromMemeory(IEntityProxy* pProxy)
{
	eENTITY_PROXY_TYPE type = pProxy->GetType();

	if( type == ENTITY_PROXY_ACTOR )			{	return GLOBAL::ActorMgr()->Remove(pProxy);	}
	else if( type == ENTITY_PROXY_RENDER )		{	return g_MemPoolProxyRender.Remove(pProxy);	}
	else if( type == ENTITY_PROXY_CAMERA )		{	return g_MemPoolProxyCamera.Remove(pProxy);	}
	else
		assert(0);
}

EEntity::EEntity()
	: m_pParent(NULL)
	, m_bCulled(true)
	, m_ID(0)
	, m_SpaceFlag(0)
{
	m_pParent = NULL;
	
	m_LocalTM = XMMatrixIdentity();
	m_WorldTM = XMMatrixIdentity();

	m_LocalEntityAABB.Reset();
	m_WorldAABB.Reset();
}

void EEntity::Init(std::string& name, long id)
{
	m_Name = name;
	m_ID = id;
}

EEntity::~EEntity()
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

	GLOBAL::SpaceMgr()->DeleteEntityFromSpace(this);
}

bool EEntity::IsVisible()
{
	if( GetProxy(ENTITY_PROXY_RENDER) == NULL )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
//    Culling information
//////////////////////////////////////////////////////////////////////////
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



//////////////////////////////////////////////////////////////////////////
//    transform functions
//////////////////////////////////////////////////////////////////////////

void EEntity::SetLocalPos(const CVector3& _pos)
{
	m_LocalTM._41 = _pos.x;
	m_LocalTM._42 = _pos.y;
	m_LocalTM._43 = _pos.z;
	UpdateWorldTM();
}

void EEntity::SetLocalScale(const CVector3& _scale)
{
	CVector3 pos;
	CQuat	 rot;

	XMMATRIX_UTIL::Decompose(NULL, &rot, &pos, m_LocalTM);
	m_LocalTM = XMMATRIX_UTIL::TransformationAffine(_scale, rot, pos);

	UpdateWorldTM();
}

void EEntity::SetLocalRot(const CQuat& _rot)
{
	CVector3 pos;
	CVector3 scale;

	XMMATRIX_UTIL::Decompose(&scale, NULL, &pos, m_LocalTM);
	m_LocalTM = XMMATRIX_UTIL::TransformationAffine(scale, _rot, pos);

	UpdateWorldTM();
}

inline void EEntity::SetLocalTM(const XMMATRIX& tm, bool bSkipUpdateWorld)
{
	m_LocalTM = tm;

	if( !bSkipUpdateWorld)
		UpdateWorldTM();
}

CQuat EEntity::GetLocalRot()
{
	CQuat	 rot;
	XMMATRIX_UTIL::Decompose(NULL, &rot, NULL, m_LocalTM);
	return rot;
}

CVector3 EEntity::GetLocalScale()
{
	CVector3 scale;
	XMMATRIX_UTIL::Decompose(&scale, NULL, NULL, m_LocalTM);
	return scale;
}

void EEntity::SetWorldPos(const CVector3& _pos)
{
	m_WorldTM._41 = _pos.x;
	m_WorldTM._42 = _pos.y;
	m_WorldTM._43 = _pos.z;

	UpdateLocalFromWorld();
	OnTransformChanged();
}

inline void EEntity::SetWorldRot(const CQuat& _rot)
{
	CVector3 pos;
	CVector3 scale;

	XMMATRIX_UTIL::Decompose(&scale, NULL, &pos, m_WorldTM);
	m_WorldTM = XMMATRIX_UTIL::TransformationAffine(scale, _rot, pos);

	UpdateLocalFromWorld();
	OnTransformChanged();
}

inline void EEntity::SetWorldTM(const XMMATRIX& tm)
{
	m_WorldTM = tm;

	UpdateLocalFromWorld();
	OnTransformChanged();
}

inline void EEntity::UpdateWorldTM()
{
	if( m_pParent == NULL )
	{
		m_WorldTM = m_LocalTM;
	}
	else
	{
		// make sure SRT order
		m_WorldTM = XMMatrixMultiply( m_LocalTM, m_pParent->GetWorldTM());
	}

	OnTransformChanged();
}

CQuat EEntity::GetWorldRot()
{
	CQuat	 rot;
	XMMATRIX_UTIL::Decompose(NULL, &rot, NULL, m_WorldTM);
	return rot;
}

CVector3 EEntity::GetWorldScale()
{
	CVector3 scale;
	XMMATRIX_UTIL::Decompose(&scale, NULL, NULL, m_WorldTM);
	return scale;
}

void EEntity::UpdateLocalFromWorld()
{
	if( m_pParent )
	{
		XMMATRIX parentWorldInverse = XMMATRIX_UTIL::Inverse(NULL, m_pParent->GetWorldTM());
		m_LocalTM =  XMMatrixMultiply( m_WorldTM, parentWorldInverse );
	}
	else
	{
		m_LocalTM =  m_WorldTM;
	}
}

void EEntity::OnTransformChanged()
{
	CAABB temp = m_WorldAABB;

	m_WorldAABB.Reset();
	if( m_LocalEntityAABB.IsValid() )
		m_WorldAABB.AddAABB( m_WorldTM, m_LocalEntityAABB.GetMin(), m_LocalEntityAABB.GetMax() );

	GLOBAL::SpaceMgr()->UpdateEntitySpaceList(this);

	int count = m_Children.size();
	for(int i= 0; i < count; i++)
	{
		m_Children[i]->UpdateWorldTM();

		if( m_Children[i]->GetWorldAABB()->IsValid() )
			m_WorldAABB.AddAABB( m_Children[i]->GetWorldAABB() );
	}

	if( temp != m_WorldAABB && m_pParent != NULL )
		m_pParent->UpdateWorldAABB();

	EntityEvent e;
	e.type = E_EVENT_TRANSFORM_CHANGED;
	SendEvent(e);
}


void EEntity::MoveOnLocalAxis(float x, float y, float z)
{
	CVector3 pos = m_LocalTM.r[3];
	pos += *(CVector3*)&m_LocalTM._11 * x;
	pos += *(CVector3*)&m_LocalTM._21 * y;
	pos += *(CVector3*)&m_LocalTM._31 * z;

	SetLocalPos(pos);
}

void EEntity::RotateLocalAxis(CVector3 axis, float radian)
{
	CVector4 pos = m_WorldTM.r[3];

	m_WorldTM.r[3] = CVector4( 0,0,0,1 ).m128;

	XMMATRIX rotTM = XMMatrixRotationAxis( axis.ToXMVEECTOR(), radian );

	rotTM = XMMatrixMultiply( m_WorldTM, rotTM );

	rotTM.r[3] = pos.m128;

	SetWorldTM(rotTM);
}


//////////////////////////////////////////////////////////////////////////
//    hierarchy functions
//////////////////////////////////////////////////////////////////////////

void EEntity::Reparent( IEntity* _pNewParent, bool keepLocalTM  )
{
	if( _pNewParent == this || m_pParent == _pNewParent )
		return;

	if( m_pParent != NULL )
		m_pParent->DetachChild( this );

	if( _pNewParent == NULL )
	{
		m_pParent = NULL;
		m_LocalTM = m_WorldTM;
		return;
	}
	else
	{
		m_pParent = (EEntity*)_pNewParent;

		if( keepLocalTM == false)
		{
			XMMATRIX parentWorldInverse = XMMATRIX_UTIL::Inverse(NULL, m_pParent->GetWorldTM());
			m_LocalTM = XMMatrixMultiply( m_WorldTM , parentWorldInverse);	
		}

		UpdateWorldTM();
	}
}

void EEntity::AttachChild( IEntity* pChild, bool keepLocalTM   )
{
	if( pChild == this )
		return;

	for( UINT i=0 ; i < m_Children.size(); ++i )
	{
		if( pChild == m_Children[i] )
			return;
	}

	m_Children.push_back( pChild );
	pChild->Reparent( this, keepLocalTM );

	UpdateWorldAABB();
}

void EEntity::DetachChild( IEntity* _pChild )
{
	if( _pChild == this )
		return;

	TYPE_ENTITY_LIST::iterator itr = m_Children.begin();

	for( ; itr != m_Children.end(); itr++ )
	{
		if( *itr == _pChild )
		{
			m_Children.erase( itr);
			EEntity* pChild = (EEntity*)_pChild;
			pChild->m_pParent = NULL;
			pChild->SetLocalTM( pChild->GetWorldTM());
			UpdateWorldAABB();
			return;
		}
	}
}

void EEntity::DetachAllChild()
{
	for(UINT i =0; i < m_Children.size(); ++i )
	{
		EEntity* pChild = (EEntity*)m_Children[i];
		pChild->m_pParent = NULL;
		pChild->SetLocalTM( pChild->GetWorldTM());
	}

	m_Children.clear();
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
	IEntityProxy* pProxy = GetProxy( type );
	if( pProxy != NULL)
		return pProxy;

	if( type == ENTITY_PROXY_ACTOR)			{	pProxy = GLOBAL::ActorMgr()->GetNew();	}
	else if( type == ENTITY_PROXY_CAMERA)	{	pProxy = g_MemPoolProxyCamera.GetNew();	}
	else if( type == ENTITY_PROXY_RENDER )	{	pProxy = g_MemPoolProxyRender.GetNew();	}

	pProxy->Init(this);
	m_ProxyMap.SetAt( type, pProxy );
	return pProxy;
}

bool EEntity::DeleteProxy( eENTITY_PROXY_TYPE type )
{
	IEntityProxy* pProxy = GetProxy(type);

	if( pProxy == NULL )
		return false;

	RemoveProxyFromMemeory( pProxy );
	return true;
}

void EEntity::DeleteAllProxy()
{
	POSITION pos = m_ProxyMap.GetStartPosition();
	ENEITY_PROXY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_ProxyMap.GetNext(pos);
		RemoveProxyFromMemeory( itr->m_value );
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
	GLOBAL::SpaceMgr()->UpdateEntitySpaceList(this);
}

void EEntity::UpdateWorldAABB()
{
	CAABB temp = m_WorldAABB;

	m_WorldAABB.Reset();
	if( m_LocalEntityAABB.IsValid() )
		m_WorldAABB.AddAABB( m_WorldTM, m_LocalEntityAABB.GetMin(), m_LocalEntityAABB.GetMax() );

	int count = m_Children.size();
	for(int i= 0; i < count; i++)
	{
		if(  m_Children[i]->GetWorldAABB()->IsValid() )
			m_WorldAABB.AddAABB( m_Children[i]->GetWorldAABB() );
	}

	if( temp != m_WorldAABB && m_pParent != NULL)
		m_pParent->UpdateWorldAABB();
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