#include "CResource.h"
#include "CDecal.h"
#include "CUnitPool.h"

#include "IDecalMgr.h"

#include "EDecalMgr.h"

CObjectPool<CDecalDesc>	g_Decals(100);



void EDecalMgr::Destroy()
{
	g_Decals.Destroy();
}

CDecalDesc*	EDecalMgr::Create()
{
	return g_Decals.GetNew();
}

CDecalDesc*	EDecalMgr::Find(UINT handle)
{
	return g_Decals.Get(handle);
}

void EDecalMgr::Remove(UINT handle)
{
	g_Decals.Remove(handle);
}

void EDecalMgr::UpdateVisible()
{

}