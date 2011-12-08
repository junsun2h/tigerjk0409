#pragma once

#include "RDefine.h"
#include "RDX11Texture.h"
#include "CGrowableArray.h"



class RDX11TextureCache
{
public:
	~RDX11TextureCache();


	HRESULT		CreateTextureFromFile( ID3D11Device* pDevice, ID3D11DeviceContext *pContext, LPCTSTR pSrcFile,
		ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false );
	HRESULT		CreateTextureFromFile( ID3D11Device* pDevice, ID3D11DeviceContext *pContext, LPCSTR pSrcFile,
		ID3D11ShaderResourceView** ppOutputRV, bool bSRGB=false );
	HRESULT		CreateTextureFromFileEx( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LPCTSTR pSrcFile,
		D3DX11_IMAGE_LOAD_INFO* pLoadInfo, ID3DX11ThreadPump* pPump,
		ID3D11ShaderResourceView** ppOutputRV, bool bSRGB );
	/*
protected:
	friend RDX11TextureCache& WINAPI DXUTGetGlobalResourceCache();
	friend HRESULT WINAPI   DXUTInitialize3DEnvironment();
	friend HRESULT WINAPI   DXUTReset3DEnvironment();
	friend void WINAPI      DXUTCleanup3DEnvironment( bool bReleaseSettings );

	RDX11TextureCache()
	{
	}
	*/
	CGrowableArray <RDX11Texture*> m_TextureCache;
};


RDX11TextureCache& WINAPI DXUTGetGlobalResourceCache();