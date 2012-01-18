#include "EEntity.h"
#include "CDefine.h"
#include "EEngine.h"



EEntity::EEntity(std::string& name, UINT id)
	: m_LocalPos(0,0,0)
	, m_LocalScale(1,1,1)
	, m_LocalRotation(0,0,0,1)
	, m_WorldPos(0,0,0)
	, m_WorldScale(1,1,1)
	, m_WorldRotation(0,0,0,1)
	, m_pParent(NULL)
	, m_Name(name)
	, m_ID(id)
{
	m_LocalTM = XMMatrixIdentity();
	m_WorldTM = XMMatrixIdentity();
}

EEntity::~EEntity()
{
	DetachAllChild();

	if( m_pParent )
		m_pParent->DetachChild( this );
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

void EEntity::SetWorldPos(const CVector3& _pos)
{
	m_WorldTM._41 = _pos.x;
	m_WorldTM._42 = _pos.y;
	m_WorldTM._43 = _pos.z;

	m_WorldPos = _pos;

	if( m_pParent )
	{
		XMMATRIX parentWorldInverse = XMMATRIX_UTIL::Inverse(NULL, m_pParent->GetWorldTM());
		m_LocalTM =  parentWorldInverse * m_WorldTM;		
		XMMATRIX_UTIL::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);
	}
	else
	{
		m_LocalTM =  m_WorldTM;		
		m_LocalPos = m_WorldPos;
		m_LocalRotation = m_WorldRotation;
	}
}

inline void EEntity::SetWorldRot(const CQuat& _rot)
{
	m_WorldRotation = _rot;
	m_WorldTM = XMMATRIX_UTIL::TransformationAffine( CVector3(1,1,1), m_WorldRotation, m_WorldPos);
	
	if( m_pParent )
	{
		XMMATRIX parentWorldInverse = XMMATRIX_UTIL::Inverse(NULL, m_pParent->GetWorldTM());
		m_LocalTM =  parentWorldInverse * m_WorldTM;		
		XMMATRIX_UTIL::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);
	}
	else
	{
		m_LocalTM =  m_WorldTM;		
		m_LocalPos = m_WorldPos;
		m_LocalRotation = m_WorldRotation;
	}
}

inline void EEntity::SetWorldTM(const XMMATRIX& tm)
{
	m_WorldTM = tm;
	XMMATRIX_UTIL::Decompose(NULL, &m_WorldRotation, &m_WorldPos, m_WorldTM);

	if( m_pParent )
	{
		XMMATRIX parentWorldInverse = XMMATRIX_UTIL::Inverse(NULL, m_pParent->GetWorldTM());
		m_LocalTM =  parentWorldInverse * m_WorldTM;		
		XMMATRIX_UTIL::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);

		EntityEvent e;
		e.type = E_EVENT_TRANSFORM_CHANGED;
		SendEvent(e);

		int count = m_Children.size();
		for(int i= 0; i < count; i++)
		{
			m_Children[i]->UpdateWorldTM();
		}
	}
	else
	{
		m_LocalTM =  m_WorldTM;		
		m_LocalPos = m_WorldPos;
		m_LocalRotation = m_WorldRotation;
		
		EntityEvent e;
		e.type = E_EVENT_TRANSFORM_CHANGED;
		SendEvent(e);
	}
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

inline void EEntity::UpdateWorldTM()
{
	if( m_pParent == NULL )
	{
		m_WorldTM = m_LocalTM;
		m_WorldPos = m_LocalPos;
		m_WorldRotation = m_LocalRotation;
		m_WorldScale = m_LocalScale;

		EntityEvent e;
		e.type = E_EVENT_TRANSFORM_CHANGED;
		SendEvent(e);
	}
	else
	{
		m_WorldTM = XMMatrixMultiply( m_LocalTM, m_pParent->GetLocalTM());
		XMMATRIX_UTIL::Decompose(&m_WorldScale, &m_WorldRotation, &m_WorldPos, m_WorldTM);

		EntityEvent e;
		e.type = E_EVENT_TRANSFORM_CHANGED;
		SendEvent(e);

		int count = m_Children.size();
		for(int i= 0; i < count; i++)
		{
			m_Children[i]->UpdateWorldTM();
		}
	}
}

void EEntity::MoveLocalAxis(float x, float y, float z)
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
		XMMATRIX worldTM = GetWorldTM();

		m_pParent = _pNewParent;
		SetWorldTM(worldTM);
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

	m_Children.push_back( pChild );
	pChild->Reparent( this );
}

void EEntity::DetachChild( IEntity* _pChild )
{
	if( _pChild == this )
		return;

	std::vector<IEntity*>::iterator itr = m_Children.begin();

	for( ; itr != m_Children.end(); itr++ )
	{
		IEntity* pChild = *itr;
		if( pChild == _pChild )
		{
			itr = m_Children.erase(itr);
			_pChild->Reparent( NULL );

			return;
		}
	}
}

void EEntity::DetachAllChild()
{
	std::vector<IEntity*>::iterator itr = m_Children.begin();

	for( ; itr != m_Children.end(); itr++ )
	{
		IEntity* pChild = *itr;
		itr = m_Children.erase(itr);
		pChild->Reparent( NULL );
	}
}

IEntity* EEntity::GetChild(UINT index )
{ 
	if( index >= m_Children.size() ) 
		return NULL; 

	return m_Children[index]; 
}

IEntityProxy* EEntity::GetProxy( ENTITY_PROXY_TYPE type )
{
	ENEITY_PROXY_MAP::CPair* pEntityProxy = m_ProxyMap.Lookup( type );
	if( pEntityProxy != NULL )
		return pEntityProxy->m_value;

	return NULL;
}

void EEntity::SetProxy( IEntityProxy *pProxy)
{
	ENTITY_PROXY_TYPE type = pProxy->GetType();

	ENEITY_PROXY_MAP::CPair* pEntityProxy = m_ProxyMap.Lookup( type );
	if( pEntityProxy != NULL )
	{
		assert(0);
		return;
	}

	m_ProxyMap.SetAt( type, pProxy );
	pProxy->SetEntity(this);
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