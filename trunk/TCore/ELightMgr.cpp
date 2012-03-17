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
