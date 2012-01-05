#pragma once

#include "IEngine.h"
#include "EEntityMgr.h"
#include "EAssetMgr.h"
#include "ESceneMgr.h"
#include "CTimer.h"
#include "IRenderCallback.h"





struct IEntityProxyCamera;


class EEngine : public IEngine
{
public:
	EEngine();
	~EEngine();

	bool			StartUp(const CENGINE_INIT_PARAM &param) override;
	bool			ShutDown() override;

	IRDevice*		RDevice() override		{ return m_pRenderer; }
	IAssetMgr*		AssetMgr() override		{ return &m_AssetMgr; }
	ISceneMgr*		SceneMgr() override		{ return &m_SceneMgr; }
	IEntityMgr*		EntityMgr() override	{ return &m_EntityMgr; }

	void			UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback) override;

public:
	long			GetCurrentFrame()		{ return m_CurrentFrame; }

private:
	IRDevice*		m_pRenderer;

	EAssetMgr		m_AssetMgr;
	ESceneMgr		m_SceneMgr;
	EEntityMgr		m_EntityMgr;
	
	long			m_CurrentFrame;

	CTimer			m_GlobalTimer;
};

extern EEngine g_Engine;