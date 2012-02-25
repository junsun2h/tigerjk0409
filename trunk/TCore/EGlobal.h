#pragma once

#include "IEngine.h"

struct IActorMgr;
struct IRenderStrategy;
struct IRenderCommandMgr;


namespace GLOBAL
{
	IRDevice*			RDevice();
	IEngine*			Engine();
	IAssetMgr*			AssetMgr();
	IEntityMgr*			EntityMgr();
	ILoader*			Loader();
	ISpaceMgr*			SpaceMgr();
	IActorMgr*			ActorMgr();
	IRenderStrategy*	Renderer();
	IRenderCommandMgr*	AsyncRenderer();
}