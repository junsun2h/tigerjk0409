#pragma once

#include "IEngine.h"


namespace GLOBAL
{
	IEngine*			Engine();
	IRDevice*			RDevice();
	IAssetMgr*			AssetMgr();
	IEntityMgr*			EntityMgr();
	IRenderHelper*		RenderHelper();
	CTimer*				GlobalTimer();
	IFileUtility*		FileUtility();
	ILoader*			Loader();
	IEngineMemoryMgr*	EngineMemoryMgr();
	ISpaceMgr*			SpaceMgr();
	long				GetCurrentFrame();
}