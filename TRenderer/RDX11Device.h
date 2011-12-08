#pragma once

#include "IRenderer.h"
#include "RDefine.h"
#include "RDX11StateRepository.h"
#include "RDX11Context.h"
#include "RDX11Window.h"
#include "CThread.h"


struct RDX11Setting
{
	D3D_FEATURE_LEVEL	m_FeatureLevel;
	D3D_DRIVER_TYPE		m_DriverType;
};




class RDX11Device : public IRenderer
{
	CCriticalSectionSpinLock m_CS;

public:
	RDX11Device();
	~RDX11Device();

	/////////////////////////////////////////////////////////////////////////
	// IRenderer
	//////////////////////////////////////////////////////////////////////////
	virtual bool	StartUp(const CENGINE_INIT_PARAM &param) override;
	virtual void	ShutDown() override;
	virtual void	Render(uint32 index) override;
	virtual bool	Resize(const CENGINE_INIT_PARAM &param) override;

private:
	ID3D11Device*					m_pD3Device;
	RDX11Setting					m_pDeviceSetting;
	RDX11Context					m_pImmediateContext;
	RDX11StateRepository			m_StateRepository;
	RDX11Window						m_MainWindow;
};