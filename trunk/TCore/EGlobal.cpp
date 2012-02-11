#include "CResource.h"
#include "CGrowableArray.h"
#include "CAABB.h"
#include "CQuad.h"
#include "CTimer.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IEntityMgr.h"
#include "IAssetMgr.h"
#include "ILoader.h"
#include "ISpaceMgr.h"
#include "IResourceMemMgr.h"

#include "EEntity.h"
#include "EEntityMgr.h"
#include "EAssetMgr.h"
#include "ELoader.h"
#include "EResourceMemMgr.h"
#include "EQuadSpaceTreeNode.h"
#include "EQuadSpaceTreeMgr.h"

#include "EGlobal.h"
#include "EEngine.h"



namespace GLOBAL
{
	EEngine				g_Engine;
	EAssetMgr			g_AssetMgr;
	EEntityMgr			g_EntityMgr;
	ELoader				g_Loader;
	EResourceMemMgr		g_ResourceMemMgr;
	EQuadSpaceTreeMgr	g_QuadSpaceMgr;

	IEngine*			Engine()			{ return &g_Engine;}
	IAssetMgr*			AssetMgr() 			{ return &g_AssetMgr; }
	IEntityMgr*			EntityMgr() 		{ return &g_EntityMgr; }
	ILoader*			Loader() 			{ return &g_Loader; }
	IResourceMemMgr*	ResourceMemMgr() 	{ return &g_ResourceMemMgr; }
	ISpaceMgr*			SpaceMgr() 			{ return &g_QuadSpaceMgr; }
}


extern "C"
{
	DLL_EXPORT IEngine* CreateEngine()
	{
		return &GLOBAL::g_Engine;
	}
};