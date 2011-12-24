#include "EEntity.h"
#include "CDefine.h"

EEntity::EEntity()
	: m_LocalPos(0,0,0)
	, m_LocalScale(1,1,1)
	, m_LocalRotation(0,0,0,1)
	, m_WorldPos(0,0,0)
	, m_WorldScale(1,1,1)
	, m_WorldRotation(0,0,0,1)
	, m_pParent(NULL)
{
	m_LocalTM = CMatrix::Identity();
	m_WorldTM = CMatrix::Identity();
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

inline void EEntity::SetLocalTM(const CMatrix& tm)
{
	m_LocalTM = tm;
	CMatrix::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);
}

inline void EEntity::SetWorldTM(const CMatrix& tm)
{
	m_LocalTM = tm;
	CMatrix::Decompose(&m_LocalScale, &m_LocalRotation, &m_LocalPos, m_LocalTM);
}

inline void EEntity::UpdateLocalTM()
{
	m_LocalTM = CMatrix::TransformationAffine(m_LocalScale, m_LocalPos, m_LocalRotation, m_LocalPos);

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
		return;
	}

	m_WorldTM = CMatrix::Multiply(m_LocalTM, m_pParent->GetLocalTM());
	CMatrix::Decompose(&m_WorldScale, &m_WorldRotation, &m_WorldPos, m_WorldTM);

	int count = m_Children.size();
	for(int i= 0; i < count; i++)
	{
		m_Children[i]->UpdateWorldTM();
	}
}


//////////////////////////////////////////////////////////////////////////
//    hierarchy functions
//////////////////////////////////////////////////////////////////////////

void EEntity::Reparent( IEntity* _pNewParent )
{
	if( _pNewParent == this || m_pParent == _pNewParent )
		return;

	if( _pNewParent == NULL )
	{
		m_pParent = NULL;
		SetLocalTM( GetWorldTM() );
		UpdateWorldTM();
		return;
	}
	else
	{
		if( m_pParent != NULL )
			GetParent()->DetachChild( this );

		_pNewParent->AttachChild( this );
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
			pChild->Reparent( NULL );

			return;
		}
	}
}

void EEntity::DetachAllChild()
{
	for( UINT i =0 ; i < m_Children.size(); ++i )
		DetachChild( m_Children[i] );
}

IEntity* EEntity::GetChild(UINT index )
{ 
	if( index >= m_Children.size() ) 
		return NULL; 

	return m_Children[index]; 
}

//////////////////////////////////////////////////////////////////////////
//   etc
//////////////////////////////////////////////////////////////////////////
void EEntity::SendEvent( ENTITY_EVENT &e )
{
}

void EEntity::Update()
{
	for( int i=0; i < NUM_ENTITY_PROXY_TYPE; ++i )
		m_Proxyes[i]->Update();
}

bool EEntity::IsUsingPerFrameUpdate()
{
	return false;
}

void EEntity::SetProxy( ENTITY_PROXY_TYPE type, IEntityProxy *pProxy)
{
	if( m_Proxyes[type] != NULL )
		SAFE_DELETE( m_Proxyes[type] );

	 m_Proxyes[type] = pProxy;
}