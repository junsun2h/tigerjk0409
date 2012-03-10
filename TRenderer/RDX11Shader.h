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
class RDX11VertexShader : public IShader
{
public:
	RDX11VertexShader()	: m_pVertexShader(NULL)	{}
	virtual ~RDX11VertexShader()	{ Destroy();}

	virtual void		Begin() override;
	void				CreateVS( SHADER_COMPILE_DESC& desc);

	eSHADER_TYPE		ShaderType()		{ return VERTEX_SHADER; }
	void				Destroy()			{ SAFE_RELEASE(m_pVertexShader); }

private:
	ID3D11VertexShader*			m_pVertexShader;
};


//---------------------------------------------------------------------------------------------------------------
class RDX11PixelShader : public IShader
{
public:
	RDX11PixelShader()	: m_pPixelShader(NULL)	{}
	virtual ~RDX11PixelShader()	{ Destroy();}

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
class RDX11GeometryShader : public IShader
{
public:
	RDX11GeometryShader()	: m_pGeometryShader(NULL)	{}
	virtual ~RDX11GeometryShader()	{ Destroy();}

	virtual void		Begin() override;
	void				CreateGS( SHADER_COMPILE_DESC& desc);

	eSHADER_TYPE		ShaderType()		{ return GEOMETRY_SHADER; }
	void				Destroy()			{ SAFE_RELEASE(m_pGeometryShader); }

private:
	ID3D11GeometryShader*		m_pGeometryShader;
};
