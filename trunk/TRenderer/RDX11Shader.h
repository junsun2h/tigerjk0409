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

	D3D10_SHADER_MACRO* shader_Macros;
	eCVERTEX_TYPE	eVertexyType;
	
	LPCSTR		debugName;

	SHADER_COMPILE_DESC()
	{
		memset( this, 0, sizeof(SHADER_COMPILE_DESC));
	}
};

//---------------------------------------------------------------------------------------------------------------
class RDX11VertexShaderBase : public IShader
{
public:
	RDX11VertexShaderBase()	: m_pVertexShader(NULL)	{}
	virtual ~RDX11VertexShaderBase()	{ Destroy();}

	virtual void		Begin() override;
	void				CreateVS( SHADER_COMPILE_DESC& desc);

	eSHADER_TYPE		ShaderType()		{ return VERTEX_SHADER; }
	void				Destroy()			{ SAFE_RELEASE(m_pVertexShader); }

private:
	ID3D11VertexShader*			m_pVertexShader;
};


//---------------------------------------------------------------------------------------------------------------
class RDX11PixelShaderBase : public IShader
{
public:
	RDX11PixelShaderBase()	: m_pPixelShader(NULL)	{}
	virtual ~RDX11PixelShaderBase()	{ Destroy();}

	virtual void		Begin() override;
	void				CreatePS( SHADER_COMPILE_DESC& desc);
	void				SetRenderState(	const GRAPHIC_DEVICE_DESC desc);

	eSHADER_TYPE		ShaderType()		{ return PIXEL_SHADER; }
	void				Destroy()			{ SAFE_RELEASE(m_pPixelShader); }

private:
	ID3D11PixelShader*			m_pPixelShader;	
	GRAPHIC_DEVICE_DESC			m_RenderState;
};


//---------------------------------------------------------------------------------------------------------------
class RDX11GeometryShaderBase : public IShader
{
public:
	RDX11GeometryShaderBase()	: m_pGeometryShader(NULL)	{}
	virtual ~RDX11GeometryShaderBase()	{ Destroy();}

	virtual void		Begin() override;
	void				CreateGS( SHADER_COMPILE_DESC& desc);

	eSHADER_TYPE		ShaderType()		{ return GEOMETRY_SHADER; }
	void				Destroy()			{ SAFE_RELEASE(m_pGeometryShader); }

private:
	ID3D11GeometryShader*		m_pGeometryShader;
};
