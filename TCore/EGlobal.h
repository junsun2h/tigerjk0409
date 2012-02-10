#pragma once

#include "IEngine.h"


namespace GLOBAL
{
	IRDevice*			RDevice();
	IAssetMgr*			AssetMgr();
	IEntityMgr*			EntityMgr();
	ILoader*			Loader();
	IResourceMemMgr*	ResourceMemMgr();
	ISpaceMgr*			SpaceMgr();
	long				GetCurrentFrame();
}