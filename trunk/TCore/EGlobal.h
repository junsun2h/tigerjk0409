#pragma once

#include "IEngine.h"


namespace GLOBAL
{
	IRDevice*			RDevice();
	IAssetMgr*			AssetMgr();
	IEntityMgr*			EntityMgr();
	ILoader*			Loader();
	IEngineMemoryMgr*	EngineMemoryMgr();
	ISpaceMgr*			SpaceMgr();
	long				GetCurrentFrame();
}