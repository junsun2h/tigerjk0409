#pragma once

#include "IRenderStateMgr.h"


struct SHADER_COMPILE_DESC
{
	LPCSTR		szFileName; 
	void*		pSrc; 
	SIZE_T		SrcDataSize;
	LPCSTR		szEntrypoint; 
	LPCSTR		szShaderModel; 
	UINT		flag;
	eCVERTEX_TYPE	eVertexyType;
	
	LPCSTR		debugName;

	SHADER_COMPILE_DESC()
	{
		memset( this, 0, sizeof(SHADER_COMPILE_DESC));
	}
};


class RDX11ShaderBase : public IShader
{
public:
	RDX11ShaderBase();
	virtual ~RDX11ShaderBase();

	virtual void		Begin() override;
	virtual void		End() override {}

	virtual void		SetShaderContants(IEntityProxyRender* pRenderProxy) override {}
	virtual void		SetShaderContants(CResourceMtrl* pMaterial) override {}
	virtual void		SetShaderContants(XMMATRIX& tm) override {}

public:
	void				CreateVS( SHADER_COMPILE_DESC& desc);
	void				CreatePS( SHADER_COMPILE_DESC& desc);
	void				CreateGS( SHADER_COMPILE_DESC& desc);

	void				SetRenderState(	const GRAPHIC_DEVICE_DESC desc);

	void				ApplyRenderState();
	void				Destroy();


private:
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;	
	ID3D11GeometryShader*		m_pGeometryShader;

	GRAPHIC_DEVICE_DESC			m_RenderState;
};