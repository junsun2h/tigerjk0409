#include "EEntityMgr.h"
#include "IResource.h"
#include "EEntity.h"


EEntityMgr::EEntityMgr()
{

}

EEntityMgr::~EEntityMgr()
{

}

IEntity* EEntityMgr::SPawn(std::string& name)
{
	long newID = GET_HASH_KEY( name );
	TYPE_ENTITY_MAP::CPair* pEntity = m_EntityMap.Lookup( newID );
	if( pEntity != NULL )
	{
		assert(0);
		return pEntity->m_value;
	} 
	
	EEntity* newEnitty = new EEntity(name, newID);
	m_EntityMap.SetAt( newID, newEnitty );

	return newEnitty;
}


void EEntityMgr::Clear()
{
	POSITION pos = m_EntityMap.GetStartPosition();
	TYPE_ENTITY_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_EntityMap.GetNext(pos);
		SAFE_DELETE( itr->m_value );
	}
}