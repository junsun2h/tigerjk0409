#pragma once

#include "IEngine.h"
#include "EActorMgr.h"
#include "EEntityMgr.h"
#include "EAssetMgr.h"



class EEngine : public IEngine
{
public:
	EEngine();
	~EEngine();

	bool			StartUp(const CENGINE_INIT_PARAM &param) override;
	bool			ShutDown() override;

	IRDevice*		RDevice() override		{ return m_pRenderer; }
	IEntityMgr*		EntityMgr()	override	{ return &m_EntityMgr; }
	IActorMgr*		AcotrMgr() override		{ return &m_ActorMgr; }
	IAssetMgr*		AssetMgr() override		{ return &m_AssetMgr; }

private:
	IRDevice*		m_pRenderer;
	EEntityMgr		m_EntityMgr;
	EActorMgr		m_ActorMgr;
	EAssetMgr		m_AssetMgr;
};

extern EEngine g_Engine;