#pragma once

#include "IEngine.h"

struct IActorMgr;


namespace GLOBAL
{
	IEngine*			Engine();
	IAssetMgr*			AssetMgr();
	IEntityMgr*			EntityMgr();
	ILoader*			Loader();
	IResourceMemMgr*	ResourceMemMgr();
	ISpaceMgr*			SpaceMgr();
	IActorMgr*			ActorMgr();
}