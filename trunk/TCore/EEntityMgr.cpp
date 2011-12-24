#include "EEntityMgr.h"


EEntityMgr::EEntityMgr()
{

}

EEntityMgr::~EEntityMgr()
{

}

IEntity* EEntityMgr::SPawn(CEntityProxyCreateParam param)
{

	return NULL;
}

bool EEntityMgr::IsUsingPerUpdate(IEntity* pEntity)
{
	if( m_PerFrameUpdateEntities.find( pEntity) == m_PerFrameUpdateEntities.end() )
		return false;

	return true;
}
