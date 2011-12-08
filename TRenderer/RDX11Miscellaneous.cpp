#include "RDX11Miscellaneous.h"


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