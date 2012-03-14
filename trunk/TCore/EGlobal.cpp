#include "CResource.h"
#include "CGrowableArray.h"
#include "CAABB.h"
#include "CTimer.h"
#include "CQuad.h"
#include "CCommandBuffer.h"
#include "CDecal.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IEntityMgr.h"
#include "IAssetMgr.h"
#include "ILoader.h"
#include "ISpaceMgr.h"
#include "IActorMgr.h"
#include "IRDevice.h"
#include "IRenderer.h"
#include "ILightMgr.h"
#include "IDecalMgr.h"

#include "EEntity.h"
#include "EEntityMgr.h"
#include "EAssetMgr.h"
#include "ELoader.h"
#include "EQuadSpaceTreeNode.h"
#include "EQuadSpaceTreeMgr.h"
#include "EActorMgr.h"
#include "ERenderer.h"
#include "ELightMgr.h"
#include "EDecalMgr.h"

#include "EGlobal.h"
#include "EEngine.h"



namespace GLOBAL
{
	EEngine				g_Engine;
	EAssetMgr			g_AssetMgr;
	EEntityMgr			g_EntityMgr;
	ELoader				g_Loader;
	EQuadSpaceTreeMgr	g_QuadSpaceMgr;
	EActorMgr			g_ActorMgr;
	ERenderer			g_Renderer;
	ELightMgr			g_LightMgr;
	EDecalMgr			g_DecalMgr;
	IRDevice*			g_pRenderer = NULL;
	
	IRDevice*			RDevice()			{ return g_pRenderer; }
	IEngine*			Engine()			{ return &g_Engine;}
	IAssetMgr*			AssetMgr() 			{ return &g_AssetMgr; }
	IEntityMgr*			EntityMgr() 		{ return &g_EntityMgr; }
	ILoader*			Loader() 			{ return &g_Loader; }
	ISpaceMgr*			SpaceMgr() 			{ return &g_QuadSpaceMgr; }
	IActorMgr*			ActorMgr()			{ return &g_ActorMgr; }
	IEngineRenderer*	Renderer()			{ return &g_Renderer;}
	ILightMgr*			LightMgr()			{ return &g_LightMgr; }
	IDecalMgr*			DecalMgr()			{ return &g_DecalMgr; }
}


extern "C"
{
	DLL_EXPORT IEngine* CreateEngine()
	{
		return &GLOBAL::g_Engine;
	}
};