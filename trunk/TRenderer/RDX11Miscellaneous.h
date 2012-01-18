#pragma once

#include "RDefine.h"
#include "RDX11DDS.h"

//--------------------------------------------------------------------------------------
// Takes a screen shot of a 32bit D3D11 back buffer and saves the images to a BMP file
//--------------------------------------------------------------------------------------
HRESULT DXUTSnapD3D11Screenshot( LPCTSTR szFileName, 
								D3DX11_IMAGE_FILE_FORMAT iff,
								IDXGISwapChain *pSwap,
								ID3D11DeviceContext *dc,
								ID3D11Device *pDevice );

//--------------------------------------------------------------------------------------
// Debug printing support
// See dxerr.h for more debug printing support
//--------------------------------------------------------------------------------------
void WINAPI DXUTOutputDebugStringW( LPCWSTR strMsg, ... );
void WINAPI DXUTOutputDebugStringA( LPCSTR strMsg, ... );
#ifdef UNICODE
#define DXUTOutputDebugString DXUTOutputDebugStringW
#else
#define DXUTOutputDebugString DXUTOutputDebugStringA
#endif

// These macros are very similar to dxerr's but it special cases the HRESULT defined
// by DXUT to pop better message boxes. 
#if defined(DEBUG) || defined(_DEBUG)
#define DXUTTRACE                  DXUTOutputDebugString
#else
#define DXUTTRACE                  (__noop)
#endif


//--------------------------------------------------------------------------------------
// Profiling/instrumentation support
//--------------------------------------------------------------------------------------

// Use DXUT_SetDebugName() to attach names to D3D objects for use by 
// SDKDebugLayer, PIX's object table, etc.
#if defined(PROFILE) || defined(DEBUG)
inline void DXUT_SetDebugName( IDXGIObject* pObj, const CHAR* pstrName )
{
	if ( pObj )
		pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
}
inline void DXUT_SetDebugName( ID3D11Device* pObj, const CHAR* pstrName )
{
	if ( pObj )
		pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
}
inline void DXUT_SetDebugName( ID3D11DeviceChild* pObj, const CHAR* pstrName )
{
	if ( pObj )
		pObj->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(pstrName), pstrName );
}
#else
#define DXUT_SetDebugName( pObj, pstrName )
#endif



//--------------------------------------------------------------------------------------
// Multimon handling to support OSes with or without multimon API support.  
// Purposely avoiding the use of multimon.h so DXUT.lib doesn't require 
// COMPILE_MULTIMON_STUBS and cause complication with MFC or other users of multimon.h
//--------------------------------------------------------------------------------------
#ifndef MONITOR_DEFAULTTOPRIMARY
#define MONITORINFOF_PRIMARY        0x00000001
#define MONITOR_DEFAULTTONULL       0x00000000
#define MONITOR_DEFAULTTOPRIMARY    0x00000001
#define MONITOR_DEFAULTTONEAREST    0x00000002
typedef struct tagMONITORINFO
{
	DWORD cbSize;
	RECT rcMonitor;
	RECT rcWork;
	DWORD dwFlags;
}                           MONITORINFO, *LPMONITORINFO;
typedef struct tagMONITORINFOEXW : public tagMONITORINFO
{
	WCHAR szDevice[CCHDEVICENAME];
}                           MONITORINFOEXW, *LPMONITORINFOEXW;
typedef MONITORINFOEXW      MONITORINFOEX;
typedef LPMONITORINFOEXW    LPMONITORINFOEX;
#endif

HMONITOR WINAPI DXUTMonitorFromWindow( HWND hWnd, DWORD dwFlags );
HMONITOR WINAPI DXUTMonitorFromRect( LPCRECT lprcScreenCoords, DWORD dwFlags );
BOOL WINAPI DXUTGetMonitorInfo( HMONITOR hMonitor, LPMONITORINFO lpMonitorInfo );

UINT WINAPI DXUTGetDXGIColorChannelBits( DXGI_FORMAT fmt );
DXGI_FORMAT MAKE_SRGB( DXGI_FORMAT format );
DXGI_FORMAT GetDXGIFormat( const DDS_PIXELFORMAT& ddpf );
void GetSurfaceInfo( UINT width, UINT height, DXGI_FORMAT fmt, UINT* pNumBytes, UINT* pRowBytes, UINT* pNumRows );

