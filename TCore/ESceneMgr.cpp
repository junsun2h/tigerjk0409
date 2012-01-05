#include "ESceneMgr.h"
#include "EEntityMgr.h"
#include "EEngine.h"


ESceneMgr::ESceneMgr()
	: m_pRootEntity(NULL)
{

}

void ESceneMgr::InitOctreeSpace(UINT size, UINT limitUnitLegnth, UINT limitHeight)
{
	m_pRootEntity = g_Engine.EntityMgr()->SpawnEntity("SceneRoot");
	m_RootSpaceNode.Create( CVector3(0,0,0), size, limitUnitLegnth, limitHeight);
}


/*------------------------------------------------------------------
exp: make Octree's node list from camera near space to far space
*/
void ESceneMgr::UpdateVisibleOctreeNodeList(IEntityProxyCamera* pCamera)
{
	
}

void ESceneMgr::UpdateRenderObjectList()
{

}