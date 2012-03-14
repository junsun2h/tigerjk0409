#include "CUnitPool.h"
#include "CLight.h"
#include "CAABB.h"

#include "IEntity.h"
#include "ILightMgr.h"

#include "ELightMgr.h"


CObjectPool<CLightDesc>	g_Lights(100);


void ELightMgr::Destroy()
{
	g_Lights.Destroy();
}

CLightDesc* ELightMgr::Create()
{
	return g_Lights.GetNew();
}

CLightDesc*	ELightMgr::Find(UINT handle)
{
	return g_Lights.Get(handle);
}

void ELightMgr::Remove(UINT handle)
{
	g_Lights.Remove(handle);
}

void ELightMgr::GetAffectLight(IEntity* pEntity, LIGHT_LIST* pLightList)
{
	const CAABB* pAABB = pEntity->GetWorldAABB();
	if( pAABB->IsValid() == false )
		return;

	const OBJ_HANDLE_LIST* pHandleList = g_Lights.UsingHandleList();
	OBJ_HANDLE_LIST::const_iterator itr = pHandleList->begin();
	for(; itr != pHandleList->end(); ++itr)
	{
		CLightDesc* pLightDesc = g_Lights.Get( *itr);

		if( pAABB->IsSphereInBox( pLightDesc->pos, pLightDesc->range ) )
			pLightList->push_back(pLightDesc);
	}
}

void ELightMgr::UpdateVisible()
{
	m_VisibleList.clear();
	const OBJ_HANDLE_LIST* pHandleList = g_Lights.UsingHandleList();

	OBJ_HANDLE_LIST::const_iterator itr = pHandleList->begin();
	for(; itr != pHandleList->end(); ++itr)
	{
		m_VisibleList.push_back( g_Lights.Get( *itr) );
	}
}
