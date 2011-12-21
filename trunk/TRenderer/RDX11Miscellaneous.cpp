#include "RDX11Miscellaneous.h"
#include <d3d9types.h>


//--------------------------------------------------------------------------------------
#define ISBITMASK( r,g,b,a ) ( ddpf.dwRBitMask == r && ddpf.dwGBitMask == g && ddpf.dwBBitMask == b && ddpf.dwABitMask == a )


//-------------------------------------------------------------------------------------- 
HRESULT DXUTSnapD3D11Screenshot( LPCTSTR szFileName, 
								 D3DX11_IMAGE_FILE_FORMAT iff,
								IDXGISwapChain *pSwap,
								ID3D11DeviceContext *dc,
								ID3D11Device *pDevice )
{
	if (!pSwap)
		return E_FAIL;

	ID3D11Texture2D* pBackBuffer;
	HRESULT hr = pSwap->GetBuffer( 0, __uuidof( *pBackBuffer ), ( LPVOID* )&pBackBuffer );
	if (hr != S_OK)
		return hr;

	if (!dc) {
		SAFE_RELEASE(pBackBuffer);
		return E_FAIL;
	}

	if (!dc) {
		SAFE_RELEASE(pBackBuffer);
		return E_FAIL;
	}

	D3D11_TEXTURE2D_DESC dsc;
	pBackBuffer->GetDesc(&dsc);
	D3D11_RESOURCE_DIMENSION dim;
	pBackBuffer->GetType(&dim);
	// special case msaa textures
	ID3D11Texture2D *pCompatableTexture = pBackBuffer;
	if ( dsc.SampleDesc.Count > 1) {
		D3D11_TEXTURE2D_DESC dsc_new = dsc;
		dsc_new.SampleDesc.Count = 1;
		dsc_new.SampleDesc.Quality = 0;
		dsc_new.Usage = D3D11_USAGE_DEFAULT;
		dsc_new.BindFlags = 0;
		dsc_new.CPUAccessFlags = 0;
		ID3D11Texture2D *resolveTexture;
		hr = pDevice->CreateTexture2D(&dsc_new, NULL, &resolveTexture);
		if ( SUCCEEDED(hr) )
		{
			DXUT_SetDebugName(resolveTexture, "DXUT");
			dc->ResolveSubresource(resolveTexture, 0, pBackBuffer, 0, dsc.Format);
			pCompatableTexture = resolveTexture;
		}
		pCompatableTexture->GetDesc(&dsc);
	}

	hr = D3DX11SaveTextureToFileW(dc, pCompatableTexture, iff, szFileName); 

	SAFE_RELEASE(pBackBuffer);
	SAFE_RELEASE(pCompatableTexture);

	return hr;

}

//--------------------------------------------------------------------------------------
// Returns the string for the given DXGI_FORMAT.
//--------------------------------------------------------------------------------------
LPCWSTR WINAPI DXUTDXGIFormatToString( DXGI_FORMAT format, bool bWithPrefix )
{
	WCHAR* pstr = NULL;
	switch( format )
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G32B32A32_TYPELESS"; break;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		pstr = L"DXGI_FORMAT_R32G32B32A32_FLOAT"; break;
	case DXGI_FORMAT_R32G32B32A32_UINT:
		pstr = L"DXGI_FORMAT_R32G32B32A32_UINT"; break;
	case DXGI_FORMAT_R32G32B32A32_SINT:
		pstr = L"DXGI_FORMAT_R32G32B32A32_SINT"; break;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G32B32_TYPELESS"; break;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		pstr = L"DXGI_FORMAT_R32G32B32_FLOAT"; break;
	case DXGI_FORMAT_R32G32B32_UINT:
		pstr = L"DXGI_FORMAT_R32G32B32_UINT"; break;
	case DXGI_FORMAT_R32G32B32_SINT:
		pstr = L"DXGI_FORMAT_R32G32B32_SINT"; break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		pstr = L"DXGI_FORMAT_R16G16B16A16_TYPELESS"; break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		pstr = L"DXGI_FORMAT_R16G16B16A16_FLOAT"; break;
	case DXGI_FORMAT_R16G16B16A16_UNORM:
		pstr = L"DXGI_FORMAT_R16G16B16A16_UNORM"; break;
	case DXGI_FORMAT_R16G16B16A16_UINT:
		pstr = L"DXGI_FORMAT_R16G16B16A16_UINT"; break;
	case DXGI_FORMAT_R16G16B16A16_SNORM:
		pstr = L"DXGI_FORMAT_R16G16B16A16_SNORM"; break;
	case DXGI_FORMAT_R16G16B16A16_SINT:
		pstr = L"DXGI_FORMAT_R16G16B16A16_SINT"; break;
	case DXGI_FORMAT_R32G32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G32_TYPELESS"; break;
	case DXGI_FORMAT_R32G32_FLOAT:
		pstr = L"DXGI_FORMAT_R32G32_FLOAT"; break;
	case DXGI_FORMAT_R32G32_UINT:
		pstr = L"DXGI_FORMAT_R32G32_UINT"; break;
	case DXGI_FORMAT_R32G32_SINT:
		pstr = L"DXGI_FORMAT_R32G32_SINT"; break;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		pstr = L"DXGI_FORMAT_R32G8X24_TYPELESS"; break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		pstr = L"DXGI_FORMAT_D32_FLOAT_S8X24_UINT"; break;
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		pstr = L"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS"; break;
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		pstr = L"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT"; break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		pstr = L"DXGI_FORMAT_R10G10B10A2_TYPELESS"; break;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		pstr = L"DXGI_FORMAT_R10G10B10A2_UNORM"; break;
	case DXGI_FORMAT_R10G10B10A2_UINT:
		pstr = L"DXGI_FORMAT_R10G10B10A2_UINT"; break;
	case DXGI_FORMAT_R11G11B10_FLOAT:
		pstr = L"DXGI_FORMAT_R11G11B10_FLOAT"; break;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		pstr = L"DXGI_FORMAT_R8G8B8A8_TYPELESS"; break;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		pstr = L"DXGI_FORMAT_R8G8B8A8_UNORM"; break;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB"; break;
	case DXGI_FORMAT_R8G8B8A8_UINT:
		pstr = L"DXGI_FORMAT_R8G8B8A8_UINT"; break;
	case DXGI_FORMAT_R8G8B8A8_SNORM:
		pstr = L"DXGI_FORMAT_R8G8B8A8_SNORM"; break;
	case DXGI_FORMAT_R8G8B8A8_SINT:
		pstr = L"DXGI_FORMAT_R8G8B8A8_SINT"; break;
	case DXGI_FORMAT_R16G16_TYPELESS:
		pstr = L"DXGI_FORMAT_R16G16_TYPELESS"; break;
	case DXGI_FORMAT_R16G16_FLOAT:
		pstr = L"DXGI_FORMAT_R16G16_FLOAT"; break;
	case DXGI_FORMAT_R16G16_UNORM:
		pstr = L"DXGI_FORMAT_R16G16_UNORM"; break;
	case DXGI_FORMAT_R16G16_UINT:
		pstr = L"DXGI_FORMAT_R16G16_UINT"; break;
	case DXGI_FORMAT_R16G16_SNORM:
		pstr = L"DXGI_FORMAT_R16G16_SNORM"; break;
	case DXGI_FORMAT_R16G16_SINT:
		pstr = L"DXGI_FORMAT_R16G16_SINT"; break;
	case DXGI_FORMAT_R32_TYPELESS:
		pstr = L"DXGI_FORMAT_R32_TYPELESS"; break;
	case DXGI_FORMAT_D32_FLOAT:
		pstr = L"DXGI_FORMAT_D32_FLOAT"; break;
	case DXGI_FORMAT_R32_FLOAT:
		pstr = L"DXGI_FORMAT_R32_FLOAT"; break;
	case DXGI_FORMAT_R32_UINT:
		pstr = L"DXGI_FORMAT_R32_UINT"; break;
	case DXGI_FORMAT_R32_SINT:
		pstr = L"DXGI_FORMAT_R32_SINT"; break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		pstr = L"DXGI_FORMAT_R24G8_TYPELESS"; break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		pstr = L"DXGI_FORMAT_D24_UNORM_S8_UINT"; break;
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		pstr = L"DXGI_FORMAT_R24_UNORM_X8_TYPELESS"; break;
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		pstr = L"DXGI_FORMAT_X24_TYPELESS_G8_UINT"; break;
	case DXGI_FORMAT_R8G8_TYPELESS:
		pstr = L"DXGI_FORMAT_R8G8_TYPELESS"; break;
	case DXGI_FORMAT_R8G8_UNORM:
		pstr = L"DXGI_FORMAT_R8G8_UNORM"; break;
	case DXGI_FORMAT_R8G8_UINT:
		pstr = L"DXGI_FORMAT_R8G8_UINT"; break;
	case DXGI_FORMAT_R8G8_SNORM:
		pstr = L"DXGI_FORMAT_R8G8_SNORM"; break;
	case DXGI_FORMAT_R8G8_SINT:
		pstr = L"DXGI_FORMAT_R8G8_SINT"; break;
	case DXGI_FORMAT_R16_TYPELESS:
		pstr = L"DXGI_FORMAT_R16_TYPELESS"; break;
	case DXGI_FORMAT_R16_FLOAT:
		pstr = L"DXGI_FORMAT_R16_FLOAT"; break;
	case DXGI_FORMAT_D16_UNORM:
		pstr = L"DXGI_FORMAT_D16_UNORM"; break;
	case DXGI_FORMAT_R16_UNORM:
		pstr = L"DXGI_FORMAT_R16_UNORM"; break;
	case DXGI_FORMAT_R16_UINT:
		pstr = L"DXGI_FORMAT_R16_UINT"; break;
	case DXGI_FORMAT_R16_SNORM:
		pstr = L"DXGI_FORMAT_R16_SNORM"; break;
	case DXGI_FORMAT_R16_SINT:
		pstr = L"DXGI_FORMAT_R16_SINT"; break;
	case DXGI_FORMAT_R8_TYPELESS:
		pstr = L"DXGI_FORMAT_R8_TYPELESS"; break;
	case DXGI_FORMAT_R8_UNORM:
		pstr = L"DXGI_FORMAT_R8_UNORM"; break;
	case DXGI_FORMAT_R8_UINT:
		pstr = L"DXGI_FORMAT_R8_UINT"; break;
	case DXGI_FORMAT_R8_SNORM:
		pstr = L"DXGI_FORMAT_R8_SNORM"; break;
	case DXGI_FORMAT_R8_SINT:
		pstr = L"DXGI_FORMAT_R8_SINT"; break;
	case DXGI_FORMAT_A8_UNORM:
		pstr = L"DXGI_FORMAT_A8_UNORM"; break;
	case DXGI_FORMAT_R1_UNORM:
		pstr = L"DXGI_FORMAT_R1_UNORM"; break;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		pstr = L"DXGI_FORMAT_R9G9B9E5_SHAREDEXP"; break;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
		pstr = L"DXGI_FORMAT_R8G8_B8G8_UNORM"; break;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		pstr = L"DXGI_FORMAT_G8R8_G8B8_UNORM"; break;
	case DXGI_FORMAT_BC1_TYPELESS:
		pstr = L"DXGI_FORMAT_BC1_TYPELESS"; break;
	case DXGI_FORMAT_BC1_UNORM:
		pstr = L"DXGI_FORMAT_BC1_UNORM"; break;
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_BC1_UNORM_SRGB"; break;
	case DXGI_FORMAT_BC2_TYPELESS:
		pstr = L"DXGI_FORMAT_BC2_TYPELESS"; break;
	case DXGI_FORMAT_BC2_UNORM:
		pstr = L"DXGI_FORMAT_BC2_UNORM"; break;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_BC2_UNORM_SRGB"; break;
	case DXGI_FORMAT_BC3_TYPELESS:
		pstr = L"DXGI_FORMAT_BC3_TYPELESS"; break;
	case DXGI_FORMAT_BC3_UNORM:
		pstr = L"DXGI_FORMAT_BC3_UNORM"; break;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		pstr = L"DXGI_FORMAT_BC3_UNORM_SRGB"; break;
	case DXGI_FORMAT_BC4_TYPELESS:
		pstr = L"DXGI_FORMAT_BC4_TYPELESS"; break;
	case DXGI_FORMAT_BC4_UNORM:
		pstr = L"DXGI_FORMAT_BC4_UNORM"; break;
	case DXGI_FORMAT_BC4_SNORM:
		pstr = L"DXGI_FORMAT_BC4_SNORM"; break;
	case DXGI_FORMAT_BC5_TYPELESS:
		pstr = L"DXGI_FORMAT_BC5_TYPELESS"; break;
	case DXGI_FORMAT_BC5_UNORM:
		pstr = L"DXGI_FORMAT_BC5_UNORM"; break;
	case DXGI_FORMAT_BC5_SNORM:
		pstr = L"DXGI_FORMAT_BC5_SNORM"; break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		pstr = L"DXGI_FORMAT_B5G6R5_UNORM"; break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		pstr = L"DXGI_FORMAT_B5G5R5A1_UNORM"; break;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		pstr = L"DXGI_FORMAT_B8G8R8A8_UNORM"; break;
	default:
		pstr = L"Unknown format"; break;
	}
	if( bWithPrefix || wcsstr( pstr, L"DXGI_FORMAT_" ) == NULL )
		return pstr;
	else
		return pstr + lstrlen( L"DXGI_FORMAT_" );
}

//--------------------------------------------------------------------------------------
// Helper functions to create SRGB formats from typeless formats and vice versa
//--------------------------------------------------------------------------------------
DXGI_FORMAT MAKE_SRGB( DXGI_FORMAT format )
{
	switch( format )
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM_SRGB;
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM_SRGB;
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM_SRGB;

	};

	return format;
}

//--------------------------------------------------------------------------------------
// Outputs to the debug stream a formatted Unicode string with a variable-argument list.
//--------------------------------------------------------------------------------------
VOID WINAPI DXUTOutputDebugStringW( LPCWSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
	WCHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	vswprintf_s( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	OutputDebugString( strBuffer );
#else
	UNREFERENCED_PARAMETER( strMsg );
#endif
}


//--------------------------------------------------------------------------------------
// Outputs to the debug stream a formatted MBCS string with a variable-argument list.
//--------------------------------------------------------------------------------------
VOID WINAPI DXUTOutputDebugStringA( LPCSTR strMsg, ... )
{
#if defined(DEBUG) || defined(_DEBUG)
	CHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	sprintf_s( strBuffer, 512, strMsg, args );
	strBuffer[511] = '\0';
	va_end(args);

	OutputDebugStringA( strBuffer );
#else
	UNREFERENCED_PARAMETER( strMsg );
#endif
}



//--------------------------------------------------------------------------------------
// Multimon API handling for OSes with or without multimon API support
//--------------------------------------------------------------------------------------
#define DXUT_PRIMARY_MONITOR ((HMONITOR)0x12340042)
typedef HMONITOR ( WINAPI* LPMONITORFROMWINDOW )( HWND, DWORD );
typedef BOOL ( WINAPI* LPGETMONITORINFO )( HMONITOR, LPMONITORINFO );
typedef HMONITOR ( WINAPI* LPMONITORFROMRECT )( LPCRECT lprcScreenCoords, DWORD dwFlags );

BOOL WINAPI DXUTGetMonitorInfo( HMONITOR hMonitor, LPMONITORINFO lpMonitorInfo )
{
	static bool s_bInited = false;
	static LPGETMONITORINFO s_pFnGetMonitorInfo = NULL;
	if( !s_bInited )
	{
		s_bInited = true;
		HMODULE hUser32 = GetModuleHandle( L"USER32" );
		if( hUser32 )
		{
			OSVERSIONINFOA osvi =
			{
				0
			}; osvi.dwOSVersionInfoSize = sizeof( osvi ); GetVersionExA( ( OSVERSIONINFOA* )&osvi );
			bool bNT = ( VER_PLATFORM_WIN32_NT == osvi.dwPlatformId );
			s_pFnGetMonitorInfo = ( LPGETMONITORINFO )( bNT ? GetProcAddress( hUser32,
				"GetMonitorInfoW" ) :
			GetProcAddress( hUser32, "GetMonitorInfoA" ) );
		}
	}

	if( s_pFnGetMonitorInfo )
		return s_pFnGetMonitorInfo( hMonitor, lpMonitorInfo );

	RECT rcWork;
	if( ( hMonitor == DXUT_PRIMARY_MONITOR ) && lpMonitorInfo && ( lpMonitorInfo->cbSize >= sizeof( MONITORINFO ) ) &&
		SystemParametersInfoA( SPI_GETWORKAREA, 0, &rcWork, 0 ) )
	{
		lpMonitorInfo->rcMonitor.left = 0;
		lpMonitorInfo->rcMonitor.top = 0;
		lpMonitorInfo->rcMonitor.right = GetSystemMetrics( SM_CXSCREEN );
		lpMonitorInfo->rcMonitor.bottom = GetSystemMetrics( SM_CYSCREEN );
		lpMonitorInfo->rcWork = rcWork;
		lpMonitorInfo->dwFlags = MONITORINFOF_PRIMARY;
		return TRUE;
	}
	return FALSE;
}



HMONITOR WINAPI DXUTMonitorFromWindow( HWND hWnd, DWORD dwFlags )
{
	static bool s_bInited = false;
	static LPMONITORFROMWINDOW s_pFnGetMonitorFromWindow = NULL;
	if( !s_bInited )
	{
		s_bInited = true;
		HMODULE hUser32 = GetModuleHandle( L"USER32" );
		if( hUser32 ) s_pFnGetMonitorFromWindow = ( LPMONITORFROMWINDOW )GetProcAddress( hUser32,
			"MonitorFromWindow" );
	}

	if( s_pFnGetMonitorFromWindow )
		return s_pFnGetMonitorFromWindow( hWnd, dwFlags );
	else
		return DXUT_PRIMARY_MONITOR;
}


HMONITOR WINAPI DXUTMonitorFromRect( LPCRECT lprcScreenCoords, DWORD dwFlags )
{
	static bool s_bInited = false;
	static LPMONITORFROMRECT s_pFnGetMonitorFromRect = NULL;
	if( !s_bInited )
	{
		s_bInited = true;
		HMODULE hUser32 = GetModuleHandle( L"USER32" );
		if( hUser32 ) s_pFnGetMonitorFromRect = ( LPMONITORFROMRECT )GetProcAddress( hUser32, "MonitorFromRect" );
	}

	if( s_pFnGetMonitorFromRect )
		return s_pFnGetMonitorFromRect( lprcScreenCoords, dwFlags );
	else
		return DXUT_PRIMARY_MONITOR;
}

//--------------------------------------------------------------------------------------
// Returns the number of color channel bits in the specified DXGI_FORMAT
//--------------------------------------------------------------------------------------
UINT WINAPI DXUTGetDXGIColorChannelBits( DXGI_FORMAT fmt )
{
	switch( fmt )
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 32;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
		return 16;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
		return 10;

	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return 8;

	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		return 5;

	default:
		return 0;
	}
}


//--------------------------------------------------------------------------------------
DXGI_FORMAT GetDXGIFormat( const DDS_PIXELFORMAT& ddpf )
{
	if( ddpf.dwFlags & DDS_RGB )
	{
		switch (ddpf.dwRGBBitCount)
		{
		case 32:
			// DXGI_FORMAT_B8G8R8A8_UNORM_SRGB & DXGI_FORMAT_B8G8R8X8_UNORM_SRGB should be
			// written using the DX10 extended header instead since these formats require
			// DXGI 1.1
			//
			// This code will use the fallback to swizzle RGB to BGR in memory for standard
			// DDS files which works on 10 and 10.1 devices with WDDM 1.0 drivers
			//
			// NOTE: We don't use DXGI_FORMAT_B8G8R8X8_UNORM or DXGI_FORMAT_B8G8R8X8_UNORM
			// here because they were defined for DXGI 1.0 but were not required for D3D10/10.1

			if( ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0xff000000) )
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			if( ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) )
				return DXGI_FORMAT_R8G8B8A8_UNORM; // No D3DFMT_X8B8G8R8 in DXGI

			// Note that many common DDS reader/writers swap the
			// the RED/BLUE masks for 10:10:10:2 formats. We assumme
			// below that the 'correct' header mask is being used. The
			// more robust solution is to use the 'DX10' header extension and
			// specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly
			if( ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) )
				return DXGI_FORMAT_R10G10B10A2_UNORM;

			if( ISBITMASK(0x0000ffff,0xffff0000,0x00000000,0x00000000) )
				return DXGI_FORMAT_R16G16_UNORM;

			if( ISBITMASK(0xffffffff,0x00000000,0x00000000,0x00000000) )
				// Only 32-bit color channel format in D3D9 was R32F
				return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
			break;

		case 24:
			// No 24bpp DXGI formats
			break;

		case 16:
			// 5:5:5 & 5:6:5 formats are defined for DXGI, but are deprecated for D3D10+

			if( ISBITMASK(0x0000f800,0x000007e0,0x0000001f,0x00000000) )
				return DXGI_FORMAT_B5G6R5_UNORM;
			if( ISBITMASK(0x00007c00,0x000003e0,0x0000001f,0x00008000) )
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			if( ISBITMASK(0x00007c00,0x000003e0,0x0000001f,0x00000000) )
				return DXGI_FORMAT_B5G5R5A1_UNORM; // No D3DFMT_X1R5G5B5 in DXGI

			// No 4bpp or 3:3:2 DXGI formats
			break;
		}
	}
	else if( ddpf.dwFlags & DDS_LUMINANCE )
	{
		if( 8 == ddpf.dwRGBBitCount )
		{
			if( ISBITMASK(0x000000ff,0x00000000,0x00000000,0x00000000) )
				return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension

			// No 4bpp DXGI formats
		}

		if( 16 == ddpf.dwRGBBitCount )
		{
			if( ISBITMASK(0x0000ffff,0x00000000,0x00000000,0x00000000) )
				return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
			if( ISBITMASK(0x000000ff,0x00000000,0x00000000,0x0000ff00) )
				return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
		}
	}
	else if( ddpf.dwFlags & DDS_ALPHA )
	{
		if( 8 == ddpf.dwRGBBitCount )
		{
			return DXGI_FORMAT_A8_UNORM;
		}
	}
	else if( ddpf.dwFlags & DDS_FOURCC )
	{
		if( MAKEFOURCC( 'D', 'X', 'T', '1' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_BC1_UNORM;
		if( MAKEFOURCC( 'D', 'X', 'T', '3' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_BC2_UNORM;
		if( MAKEFOURCC( 'D', 'X', 'T', '5' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_BC3_UNORM;

		if( MAKEFOURCC( 'B', 'C', '4', 'U' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_BC4_UNORM;
		if( MAKEFOURCC( 'B', 'C', '4', 'S' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_BC4_SNORM;

		if( MAKEFOURCC( 'A', 'T', 'I', '2' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_BC5_UNORM;
		if( MAKEFOURCC( 'B', 'C', '5', 'S' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_BC5_SNORM;

		if( MAKEFOURCC( 'R', 'G', 'B', 'G' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_R8G8_B8G8_UNORM;
		if( MAKEFOURCC( 'G', 'R', 'G', 'B' ) == ddpf.dwFourCC )
			return DXGI_FORMAT_G8R8_G8B8_UNORM;

		// Check for D3DFORMAT enums being set here
		switch( ddpf.dwFourCC )
		{
		case D3DFMT_A16B16G16R16: // 36
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case D3DFMT_Q16W16V16U16: // 110
			return DXGI_FORMAT_R16G16B16A16_SNORM;

		case D3DFMT_R16F: // 111
			return DXGI_FORMAT_R16_FLOAT;

		case D3DFMT_G16R16F: // 112
			return DXGI_FORMAT_R16G16_FLOAT;

		case D3DFMT_A16B16G16R16F: // 113
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case D3DFMT_R32F: // 114
			return DXGI_FORMAT_R32_FLOAT;

		case D3DFMT_G32R32F: // 115
			return DXGI_FORMAT_R32G32_FLOAT;

		case D3DFMT_A32B32G32R32F: // 116
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}



static void GetSurfaceInfo( UINT width, UINT height, DXGI_FORMAT fmt, UINT* pNumBytes, UINT* pRowBytes, UINT* pNumRows )
{
	UINT numBytes = 0;
	UINT rowBytes = 0;
	UINT numRows = 0;

	bool bc = true;
	int bcnumBytesPerBlock = 16;
	switch (fmt)
	{
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		bcnumBytesPerBlock = 8;
		break;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		break;

	default:
		bc = false;
		break;
	}

	if( bc )
	{
		int numBlocksWide = 0;
		if( width > 0 )
			numBlocksWide = max( 1, width / 4 );
		int numBlocksHigh = 0;
		if( height > 0 )
			numBlocksHigh = max( 1, height / 4 );
		rowBytes = numBlocksWide * bcnumBytesPerBlock;
		numRows = numBlocksHigh;
	}
	else
	{
		UINT bpp = DXUTGetDXGIColorChannelBits( fmt );
		rowBytes = ( width * bpp + 7 ) / 8; // round up to nearest byte
		numRows = height;
	}
	numBytes = rowBytes * numRows;
	if( pNumBytes != NULL )
		*pNumBytes = numBytes;
	if( pRowBytes != NULL )
		*pRowBytes = rowBytes;
	if( pNumRows != NULL )
		*pNumRows = numRows;
}
