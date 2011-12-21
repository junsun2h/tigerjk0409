#pragma once

#include "IEngine.h"


class EEngine : public IEngine
{
public:
	EEngine();
	~EEngine();

	virtual bool StartUp(const CENGINE_INIT_PARAM &param);
	virtual bool ShutDown();

	//////////////////////////////////////////////////////////////////////////
	// IEngine
	//////////////////////////////////////////////////////////////////////////
	virtual IRDevice* GetRenderer() override;

private:
	IRDevice*	m_pRenderer;
};

