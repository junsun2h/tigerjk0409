#pragma once

#include "IEngine.h"
#include "EEntityMgr.h"
#include "EAssetMgr.h"
#include "ESceneMgr.h"
#include "CTimer.h"
#include "ELoader.h"
#include "IRenderCallback.h"
#include "EFileUtility.h"



struct IEntityProxyCamera;
struct IRenderHelper;

class EEngine : public IEngine
{
public:
	EEngine();
	~EEngine();

	bool			StartUp(const CENGINE_INIT_PARAM &param) override;
	bool			ShutDown() override;
	void			Resize(UINT width, UINT height) override;

	IRDevice*		RDevice() override		{ return m_pRenderer; }
	IAssetMgr*		AssetMgr() override		{ return &m_AssetMgr; }
	ISceneMgr*		SceneMgr() override		{ return &m_SceneMgr; }
	IEntityMgr*		EntityMgr() override	{ return &m_EntityMgr; }
	IRenderHelper*	RenderHelper() override { return m_pRenderer->GetRenderHelper(); }
	void			UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback) override;
	CTimer*			GlobalTimer() override	{ return &m_GlobalTimer; }
	IFileUtility*	FileUtility() override	{ return &m_FileUtility; }
	ILoader*		Loader() override		{ return &m_Loader; }

public:
	long			GetCurrentFrame()		{ return m_CurrentFrame; }

private:
	IRDevice*		m_pRenderer;
	IRenderHelper*	m_pRenderHelper;

	EAssetMgr		m_AssetMgr;
	ESceneMgr		m_SceneMgr;
	EEntityMgr		m_EntityMgr;
	ELoader			m_Loader;

	long			m_CurrentFrame;
	CTimer			m_GlobalTimer;

	EFileUtility	m_FileUtility;
};

extern EEngine g_Engine;