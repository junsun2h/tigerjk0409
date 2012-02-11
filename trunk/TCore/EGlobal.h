#pragma once

#include "IEngine.h"


namespace GLOBAL
{
	IEngine*			Engine();
	IAssetMgr*			AssetMgr();
	IEntityMgr*			EntityMgr();
	ILoader*			Loader();
	IResourceMemMgr*	ResourceMemMgr();
	ISpaceMgr*			SpaceMgr();
}