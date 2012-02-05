#include "IEngine.h"

#include "CGrowableArray.h"

#include "EEntityMgr.h"
#include "EAssetMgr.h"
#include "ELoader.h"
#include "EFileUtility.h"
#include "CAABB.h"
#include "EEntity.h"
#include "EEntityProxyActor.h"
#include "EEntityProxyCamera.h"
#include "EEntityProxyRender.h"
#include "EEngineMemoryMgr.h"
#include "EQuadSpaceTreeNode.h"
#include "EQuadSpaceTreeMgr.h"
#include "EEngine.h"

#include "EGlobal.h"


namespace GLOBAL
{
	EEngine				g_Engine;
	EAssetMgr			g_AssetMgr;
	EEntityMgr			g_EntityMgr;
	ELoader				g_Loader;
	EEngineMemoryMgr	g_EngineMemoryMgr;
	EQuadSpaceTreeMgr	g_QuadSpaceMgr;
	EFileUtility		g_FileUtility;


	IEngine*			Engine()			{ return &g_Engine; }
	IRDevice*			RDevice() 			{ return g_Engine.RDevice(); }
	IAssetMgr*			AssetMgr() 			{ return &g_AssetMgr; }
	IEntityMgr*			EntityMgr() 		{ return &g_EntityMgr; }
	IRenderHelper*		RenderHelper() 		{ return g_Engine.RDevice()->GetRenderHelper(); }
	CTimer*				GlobalTimer() 		{ return g_Engine.GlobalTimer(); }
	IFileUtility*		FileUtility() 		{ return &g_FileUtility; }
	ILoader*			Loader() 			{ return &g_Loader; }
	IEngineMemoryMgr*	EngineMemoryMgr() 	{ return &g_EngineMemoryMgr; }
	ISpaceMgr*			SpaceMgr() 			{ return &g_QuadSpaceMgr; }
	long				GetCurrentFrame()	{ return g_Engine.GetCurrentFrame(); }
}