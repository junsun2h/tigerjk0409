#pragma once

#include "IEngine.h"
#include "EEntityMgr.h"
#include "EAssetMgr.h"
#include "CTimer.h"
#include "ELoader.h"
#include "IRenderCallback.h"
#include "EFileUtility.h"
#include "EEngineMemoryMgr.h"
#include "EQuadSpaceTreeMgr.h"


struct IEntityProxyCamera;
struct IRenderHelper;

class EEngine : public IEngine
{
public:
	EEngine();
	~EEngine();

	bool				StartUp(const CENGINE_INIT_PARAM &param) override;
	bool				ShutDown() override;
	void				Resize(UINT width, UINT height) override;

	IRDevice*			RDevice() override			{ return m_pRenderer; }
	
	IAssetMgr*			AssetMgr() override			{ return &m_AssetMgr; }
	IEntityMgr*			EntityMgr() override		{ return &m_EntityMgr; }

	IRenderHelper*		RenderHelper() override		{ return m_pRenderer->GetRenderHelper(); }
	CTimer*				GlobalTimer() override		{ return &m_GlobalTimer; }
	IFileUtility*		FileUtility() override		{ return &m_FileUtility; }
	ILoader*			Loader() override			{ return &m_Loader; }
	IEngineMemoryMgr*	EngineMemoryMgr() override	{ return &m_EngineMemoryMgr; }

	void				UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback) override;
	ISpaceMgr*			SpaceMgr() override			{ return &m_QuadSpaceMgr; }

public:
	EEngineMemoryMgr*	EngMemoryPoolMgr()			{ return &m_EngineMemoryMgr; }
	long				GetCurrentFrame()			{ return m_CurrentFrame; }

private:
	IRDevice*			m_pRenderer;
	IRenderHelper*		m_pRenderHelper;

	EAssetMgr			m_AssetMgr;

	EEntityMgr			m_EntityMgr;

	ELoader				m_Loader;
	EEngineMemoryMgr	m_EngineMemoryMgr;

	EQuadSpaceTreeMgr	m_QuadSpaceMgr;

	long				m_CurrentFrame;
	CTimer				m_GlobalTimer;

	EFileUtility		m_FileUtility;
};

extern EEngine g_Engine;