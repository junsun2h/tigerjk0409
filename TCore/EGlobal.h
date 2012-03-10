#pragma once

#include "IEngine.h"

struct IActorMgr;
struct IRenderer;
struct IEngineRenderer;
struct IRDevice;
struct ILightMgr;


namespace GLOBAL
{
	IRDevice*			RDevice();
	IEngine*			Engine();
	IAssetMgr*			AssetMgr();
	IEntityMgr*			EntityMgr();
	ILoader*			Loader();
	ISpaceMgr*			SpaceMgr();
	IActorMgr*			ActorMgr();
	IEngineRenderer*	Renderer();
	ILightMgr*			LightMgr();
}