#include "CGrowableArray.h"
#include "CUnitPool.h"

#include "IMotionInstance.h"
#include "IEntityProxy.h"
#include "IActorMgr.h"

#include "EEntityProxyActor.h"
#include "EActorMgr.h"



CObjectPool<EEntityProxyActor>	g_MemPoolProxyActor(100);


IEntityProxyActor* EActorMgr::GetNew()
{
	return g_MemPoolProxyActor.GetNew();
}

void EActorMgr::Remove(IEntityProxy* pActor)
{
	g_MemPoolProxyActor.Remove(pActor);
}

void EActorMgr::Update(float timeDelta)
{
	const OBJ_HANDLE_LIST* pHandleList = g_MemPoolProxyActor.UsingHandleList();

	OBJ_HANDLE_LIST::const_iterator itr = pHandleList->begin();
	for(; itr != pHandleList->end(); ++itr)
	{
		g_MemPoolProxyActor.Get( *itr)->Update(timeDelta);
	}
}