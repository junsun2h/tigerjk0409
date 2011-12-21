#pragma once

#include "IRDevice.h"
#include "RDefine.h"
#include "RDX11StateRepository.h"
#include "RDX11Context.h"
#include "RDX11Window.h"
#include "CThread.h"
#include <map>

struct RDX11Setting
{
	D3D_FEATURE_LEVEL	m_FeatureLevel;
	D3D_DRIVER_TYPE		m_DriverType;
};




class RDX11Device : public IRDevice
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

	virtual void	TS_CreateDPResource(DEVICE_DEPENDENT_RESOURCE type, void* pBuf ,int size, IResource* pResource) override;

private:
	void			TS_CreateVB(void* pBuf ,int size, IResource* pResource);
	void			TS_CreateVBOut(void* pBuf ,int size, IResource* pResource);
	void			TS_CreateIB(void* pBuf ,int size, IResource* pResource);
	void			TS_CreateTexture(void* pBuf ,int size, IResource* pResource);
	void			TS_CreateShader(void* pBuf ,int size, IResource* pResource);


	ID3D11Device*					m_pD3Device;
	RDX11Setting					m_pDeviceSetting;
	RDX11Context					m_pImmediateContext;
	RDX11StateRepository			m_StateRepository;
	RDX11Window						m_MainWindow;

	typedef std::map<long, ID3D11Buffer*>				RDX11BUFFER_RESOURCE_MAP;
	typedef std::map<long, ID3D11ShaderResourceView*>	RDX11TEXTURE_RESOURCE_MAP;
	typedef std::map<long, ID3D11DeviceChild*>			RDX11SHADER_RESOURCE_MAP;

	RDX11BUFFER_RESOURCE_MAP	m_RscVBMap;
	RDX11BUFFER_RESOURCE_MAP	m_RscIBMap;
	RDX11TEXTURE_RESOURCE_MAP	m_RscTexMap;
	RDX11SHADER_RESOURCE_MAP	m_RscShaderMap;
};