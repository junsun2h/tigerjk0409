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
	virtual IRenderer* GetRenderer() override;

private:
	IRenderer*	m_pRenderer;
};

