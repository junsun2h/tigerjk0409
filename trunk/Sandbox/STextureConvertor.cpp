#pragma warning(disable:4996)

#include "SGlobal.h"
#include "ATI_Compress.h"

#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include "ddraw.h"
#include "d3d9types.h"
#include "ATI_Compress.h"

#include "CResource.h"

#include "IAssetMgr.h"
#include "IRDevice.h"
#include "ILoader.h"

#include "STextureConvertor.h"

// ATI_Compress_Test_Helpers.cpp
//

#define FOURCC_ATI1N		        MAKEFOURCC('A', 'T', 'I', '1')
#define FOURCC_ATI2N		        MAKEFOURCC('A', 'T', 'I', '2')
#define FOURCC_ATI2N_XY		        MAKEFOURCC('A', '2', 'X', 'Y')
#define FOURCC_ATI2N_DXT5	        MAKEFOURCC('A', '2', 'D', '5')
#define FOURCC_DXT5_xGBR	        MAKEFOURCC('x', 'G', 'B', 'R')
#define FOURCC_DXT5_RxBG	        MAKEFOURCC('R', 'x', 'B', 'G')
#define FOURCC_DXT5_RBxG	        MAKEFOURCC('R', 'B', 'x', 'G')
#define FOURCC_DXT5_xRBG	        MAKEFOURCC('x', 'R', 'B', 'G')
#define FOURCC_DXT5_RGxB	        MAKEFOURCC('R', 'G', 'x', 'B')
#define FOURCC_DXT5_xGxR	        MAKEFOURCC('x', 'G', 'x', 'R')
#define FOURCC_APC1			        MAKEFOURCC('A', 'P', 'C', '1')
#define FOURCC_APC2			        MAKEFOURCC('A', 'P', 'C', '2')
#define FOURCC_APC3			        MAKEFOURCC('A', 'P', 'C', '3')
#define FOURCC_APC4			        MAKEFOURCC('A', 'P', 'C', '4')
#define FOURCC_APC5			        MAKEFOURCC('A', 'P', 'C', '5')
#define FOURCC_APC6			        MAKEFOURCC('A', 'P', 'C', '6')
#define FOURCC_ATC_RGB			    MAKEFOURCC('A', 'T', 'C', ' ')
#define FOURCC_ATC_RGBA_EXPLICIT    MAKEFOURCC('A', 'T', 'C', 'A')
#define FOURCC_ATC_RGBA_INTERP	    MAKEFOURCC('A', 'T', 'C', 'I')
#define FOURCC_ETC_RGB			    MAKEFOURCC('E', 'T', 'C', ' ')
#define FOURCC_BC1                  MAKEFOURCC('B', 'C', '1', ' ')
#define FOURCC_BC2                  MAKEFOURCC('B', 'C', '2', ' ')
#define FOURCC_BC3                  MAKEFOURCC('B', 'C', '3', ' ')
#define FOURCC_BC4                  MAKEFOURCC('B', 'C', '4', ' ')
#define FOURCC_BC4S                 MAKEFOURCC('B', 'C', '4', 'S')
#define FOURCC_BC4U                 MAKEFOURCC('B', 'C', '4', 'U')
#define FOURCC_BC5                  MAKEFOURCC('B', 'C', '5', ' ')
#define FOURCC_BC5S                 MAKEFOURCC('B', 'C', '5', 'S')
// Deprecated but still supported for decompression
#define FOURCC_DXT5_GXRB            MAKEFOURCC('G', 'X', 'R', 'B')
#define FOURCC_DXT5_GRXB            MAKEFOURCC('G', 'R', 'X', 'B')
#define FOURCC_DXT5_RXGB            MAKEFOURCC('R', 'X', 'G', 'B')
#define FOURCC_DXT5_BRGX            MAKEFOURCC('B', 'R', 'G', 'X')

typedef struct
{
	DWORD dwFourCC;
	ATI_TC_FORMAT nFormat;
} ATI_TC_FourCC;

ATI_TC_FourCC g_FourCCs[] =
{
	{FOURCC_DXT1,               ATI_TC_FORMAT_DXT1},
	{FOURCC_DXT3,               ATI_TC_FORMAT_DXT3},
	{FOURCC_DXT5,               ATI_TC_FORMAT_DXT5},
	{FOURCC_DXT5_xGBR,          ATI_TC_FORMAT_DXT5_xGBR},
	{FOURCC_DXT5_RxBG,          ATI_TC_FORMAT_DXT5_RxBG},
	{FOURCC_DXT5_RBxG,          ATI_TC_FORMAT_DXT5_RBxG},
	{FOURCC_DXT5_xRBG,          ATI_TC_FORMAT_DXT5_xRBG},
	{FOURCC_DXT5_RGxB,          ATI_TC_FORMAT_DXT5_RGxB},
	{FOURCC_DXT5_xGxR,          ATI_TC_FORMAT_DXT5_xGxR},
	{FOURCC_DXT5_GXRB,          ATI_TC_FORMAT_DXT5_xRBG},
	{FOURCC_DXT5_GRXB,          ATI_TC_FORMAT_DXT5_RxBG},
	{FOURCC_DXT5_RXGB,          ATI_TC_FORMAT_DXT5_xGBR},
	{FOURCC_DXT5_BRGX,          ATI_TC_FORMAT_DXT5_RGxB},
	{FOURCC_ATI1N,              ATI_TC_FORMAT_ATI1N},
	{FOURCC_ATI2N,              ATI_TC_FORMAT_ATI2N},
	{FOURCC_ATI2N_XY,           ATI_TC_FORMAT_ATI2N_XY},
	{FOURCC_ATI2N_DXT5,         ATI_TC_FORMAT_ATI2N_DXT5},
	{FOURCC_BC1,                ATI_TC_FORMAT_BC1},
	{FOURCC_BC2,                ATI_TC_FORMAT_BC2},
	{FOURCC_BC3,                ATI_TC_FORMAT_BC3},
	{FOURCC_BC4,                ATI_TC_FORMAT_BC4},
	{FOURCC_BC4S,               ATI_TC_FORMAT_BC4},
	{FOURCC_BC4U,               ATI_TC_FORMAT_BC4},
	{FOURCC_BC5,                ATI_TC_FORMAT_BC5},
	{FOURCC_BC5S,               ATI_TC_FORMAT_BC5},
	{FOURCC_ATC_RGB,            ATI_TC_FORMAT_ATC_RGB},
	{FOURCC_ATC_RGBA_EXPLICIT,  ATI_TC_FORMAT_ATC_RGBA_Explicit},
	{FOURCC_ATC_RGBA_INTERP,    ATI_TC_FORMAT_ATC_RGBA_Interpolated},
	{FOURCC_ETC_RGB,            ATI_TC_FORMAT_ETC_RGB},
};
DWORD g_dwFourCCCount = sizeof(g_FourCCs) / sizeof(g_FourCCs[0]);

ATI_TC_FORMAT GetFormat(DWORD dwFourCC)
{
	for(DWORD i = 0; i < g_dwFourCCCount; i++)
		if(g_FourCCs[i].dwFourCC == dwFourCC)
			return g_FourCCs[i].nFormat;

	return ATI_TC_FORMAT_Unknown;
}

DWORD GetFourCC(ATI_TC_FORMAT nFormat)
{
	for(DWORD i = 0; i < g_dwFourCCCount; i++)
		if(g_FourCCs[i].nFormat == nFormat)
			return g_FourCCs[i].dwFourCC;

	return 0;
}

bool IsDXT5SwizzledFormat(ATI_TC_FORMAT nFormat)
{
	if(nFormat == ATI_TC_FORMAT_DXT5_xGBR || nFormat == ATI_TC_FORMAT_DXT5_RxBG || nFormat == ATI_TC_FORMAT_DXT5_RBxG ||
		nFormat == ATI_TC_FORMAT_DXT5_xRBG || nFormat == ATI_TC_FORMAT_DXT5_RGxB || nFormat == ATI_TC_FORMAT_DXT5_xGxR ||
		nFormat == ATI_TC_FORMAT_ATI2N_DXT5)
		return true;
	else
		return false;
}

ATI_TC_FORMAT GET_ATI_FORMAT(eTEXTURE_FORMAT format)
{
	switch( format )
	{
		case COLOR_FORMAT_UNKNOWN : break;
		case COLOR_FORMAT_R32G32B32A32_TYPELESS        :
		case COLOR_FORMAT_R32G32B32A32_FLOAT           :
		case COLOR_FORMAT_R32G32B32A32_UINT            :
		case COLOR_FORMAT_R32G32B32A32_SINT            : return ATI_TC_FORMAT_ARGB_32F;
		case COLOR_FORMAT_R32G32B32_TYPELESS           : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R32G32B32_FLOAT              : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R32G32B32_UINT               : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R32G32B32_SINT               : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R16G16B16A16_TYPELESS        : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R16G16B16A16_FLOAT           : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R16G16B16A16_UNORM           : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R16G16B16A16_UINT            : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R16G16B16A16_SNORM           : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R16G16B16A16_SINT            : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R32G32_TYPELESS              : 
		case COLOR_FORMAT_R32G32_FLOAT                 : 
		case COLOR_FORMAT_R32G32_UINT                  : 
		case COLOR_FORMAT_R32G32_SINT                  : return ATI_TC_FORMAT_RG_32F;
		case COLOR_FORMAT_R32G8X24_TYPELESS            : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_D32_FLOAT_S8X24_UINT         : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R32_FLOAT_X8X24_TYPELESS     : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_X32_TYPELESS_G8X24_UINT      : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R10G10B10A2_TYPELESS         : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R10G10B10A2_UNORM            : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R10G10B10A2_UINT             : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R11G11B10_FLOAT              : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R8G8B8A8_TYPELESS            : 
		case COLOR_FORMAT_R8G8B8A8_UNORM               : 
		case COLOR_FORMAT_R8G8B8A8_UNORM_SRGB          : 
		case COLOR_FORMAT_R8G8B8A8_UINT                : 
		case COLOR_FORMAT_R8G8B8A8_SNORM               : 
		case COLOR_FORMAT_R8G8B8A8_SINT                : return ATI_TC_FORMAT_ARGB_8888;
		case COLOR_FORMAT_R16G16_TYPELESS              : 
		case COLOR_FORMAT_R16G16_FLOAT                 : 
		case COLOR_FORMAT_R16G16_UNORM                 : 
		case COLOR_FORMAT_R16G16_UINT                  : 
		case COLOR_FORMAT_R16G16_SNORM                 : 
		case COLOR_FORMAT_R16G16_SINT                  : return ATI_TC_FORMAT_RG_16F;
		case COLOR_FORMAT_R32_TYPELESS                 : 
		case COLOR_FORMAT_D32_FLOAT                    : 
		case COLOR_FORMAT_R32_FLOAT                    : 
		case COLOR_FORMAT_R32_UINT                     : 
		case COLOR_FORMAT_R32_SINT                     : return ATI_TC_FORMAT_R_32F;
		case COLOR_FORMAT_R24G8_TYPELESS               : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_D24_UNORM_S8_UINT            : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R24_UNORM_X8_TYPELESS        : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_X24_TYPELESS_G8_UINT         : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R8G8_TYPELESS                : 
		case COLOR_FORMAT_R8G8_UNORM                   : 
		case COLOR_FORMAT_R8G8_UINT                    : 
		case COLOR_FORMAT_R8G8_SNORM                   : 
		case COLOR_FORMAT_R8G8_SINT                    : return ATI_TC_FORMAT_RG_8;
		case COLOR_FORMAT_R16_TYPELESS                 : 
		case COLOR_FORMAT_R16_FLOAT                    : 
		case COLOR_FORMAT_D16_UNORM                    : 
		case COLOR_FORMAT_R16_UNORM                    : 
		case COLOR_FORMAT_R16_UINT                     : 
		case COLOR_FORMAT_R16_SNORM                    : 
		case COLOR_FORMAT_R16_SINT                     : return ATI_TC_FORMAT_R_16;
		case COLOR_FORMAT_R8_TYPELESS                  : 
		case COLOR_FORMAT_R8_UNORM                     : 
		case COLOR_FORMAT_R8_UINT                      : 
		case COLOR_FORMAT_R8_SNORM                     : 
		case COLOR_FORMAT_R8_SINT                      : 
		case COLOR_FORMAT_A8_UNORM                     : return ATI_TC_FORMAT_R_16;
		case COLOR_FORMAT_R1_UNORM                     : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R9G9B9E5_SHAREDEXP           : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_R8G8_B8G8_UNORM              : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_G8R8_G8B8_UNORM              : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_BC1_TYPELESS                 : 
		case COLOR_FORMAT_BC1_UNORM                    : 
		case COLOR_FORMAT_BC1_UNORM_SRGB               : return ATI_TC_FORMAT_BC1;
		case COLOR_FORMAT_BC2_TYPELESS                 :
		case COLOR_FORMAT_BC2_UNORM                    :
		case COLOR_FORMAT_BC2_UNORM_SRGB               : return ATI_TC_FORMAT_BC2;
		case COLOR_FORMAT_BC3_TYPELESS                 : 
		case COLOR_FORMAT_BC3_UNORM                    : 
		case COLOR_FORMAT_BC3_UNORM_SRGB               : return ATI_TC_FORMAT_BC3;
		case COLOR_FORMAT_BC4_TYPELESS                 : 
		case COLOR_FORMAT_BC4_UNORM                    : 
		case COLOR_FORMAT_BC4_SNORM                    : return ATI_TC_FORMAT_BC4;
		case COLOR_FORMAT_BC5_TYPELESS                 :
		case COLOR_FORMAT_BC5_UNORM                    :
		case COLOR_FORMAT_BC5_SNORM                    : return ATI_TC_FORMAT_BC5;
		case COLOR_FORMAT_B5G6R5_UNORM                 : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_B5G5R5A1_UNORM               : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_B8G8R8A8_UNORM               : 
		case COLOR_FORMAT_B8G8R8X8_UNORM               : return ATI_TC_FORMAT_ARGB_8888;
		case COLOR_FORMAT_R10G10B10_XR_BIAS_A2_UNORM   : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_B8G8R8A8_TYPELESS            : 
		case COLOR_FORMAT_B8G8R8A8_UNORM_SRGB          : 
		case COLOR_FORMAT_B8G8R8X8_TYPELESS            : 
		case COLOR_FORMAT_B8G8R8X8_UNORM_SRGB          : return ATI_TC_FORMAT_ARGB_8888;
		case COLOR_FORMAT_BC6H_TYPELESS                : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_BC6H_UF16                    : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_BC6H_SF16                    : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_BC7_TYPELESS                 : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_BC7_UNORM                    : return ATI_TC_FORMAT_Unknown;
		case COLOR_FORMAT_BC7_UNORM_SRGB               : return ATI_TC_FORMAT_Unknown;
	}

	return ATI_TC_FORMAT_Unknown;
}


#ifdef _WIN64
#	define POINTER_64 __ptr64

#pragma pack(4)

typedef struct _DDSURFACEDESC2_64
{
	DWORD               dwSize;                 // size of the DDSURFACEDESC structure
	DWORD               dwFlags;                // determines what fields are valid
	DWORD               dwHeight;               // height of surface to be created
	DWORD               dwWidth;                // width of input surface
	union
	{
		LONG            lPitch;                 // distance to start of next line (return value only)
		DWORD           dwLinearSize;           // Formless late-allocated optimized surface size
	} DUMMYUNIONNAMEN(1);
	union
	{
		DWORD           dwBackBufferCount;      // number of back buffers requested
		DWORD           dwDepth;                // the depth if this is a volume texture
	} DUMMYUNIONNAMEN(5);
	union
	{
		DWORD           dwMipMapCount;          // number of mip-map levels requestde
		// dwZBufferBitDepth removed, use ddpfPixelFormat one instead
		DWORD           dwRefreshRate;          // refresh rate (used when display mode is described)
		DWORD           dwSrcVBHandle;          // The source used in VB::Optimize
	} DUMMYUNIONNAMEN(2);
	DWORD               dwAlphaBitDepth;        // depth of alpha buffer requested
	DWORD               dwReserved;             // reserved
	void* __ptr32       lpSurface;              // pointer to the associated surface memory
	union
	{
		DDCOLORKEY      ddckCKDestOverlay;      // color key for destination overlay use
		DWORD           dwEmptyFaceColor;       // Physical color for empty cubemap faces
	} DUMMYUNIONNAMEN(3);
	DDCOLORKEY          ddckCKDestBlt;          // color key for destination blt use
	DDCOLORKEY          ddckCKSrcOverlay;       // color key for source overlay use
	DDCOLORKEY          ddckCKSrcBlt;           // color key for source blt use
	union
	{
		DDPIXELFORMAT   ddpfPixelFormat;        // pixel format description of the surface
		DWORD           dwFVF;                  // vertex format description of vertex buffers
	} DUMMYUNIONNAMEN(4);
	DDSCAPS2            ddsCaps;                // direct draw surface capabilities
	DWORD               dwTextureStage;         // stage in multitexture cascade
} DDSURFACEDESC2_64;

#define DDSD2 DDSURFACEDESC2_64
#else
#define DDSD2 DDSURFACEDESC2
#endif

static const DWORD DDS_HEADER = MAKEFOURCC('D', 'D', 'S', ' ');

bool LoadDDSFile(const char* pszFile, ATI_TC_Texture& texture)
{
	FILE* pSourceFile = NULL;
	
	fopen_s( &pSourceFile, pszFile, ("rb"));

	DWORD dwFileHeader;
	fread(&dwFileHeader, sizeof(DWORD), 1, pSourceFile);
	if(dwFileHeader != DDS_HEADER)
	{
		_tprintf(_T("Source file is not a valid DDS.\n"));
		fclose(pSourceFile);
		return false;
	}

	DDSD2 ddsd;
	fread(&ddsd, sizeof(DDSD2), 1, pSourceFile);

	memset(&texture, 0, sizeof(texture));
	texture.dwSize = sizeof(texture);
	texture.dwWidth = ddsd.dwWidth;
	texture.dwHeight = ddsd.dwHeight;
	texture.dwPitch = ddsd.lPitch;

	if(ddsd.ddpfPixelFormat.dwRGBBitCount==32)
		texture.format = ATI_TC_FORMAT_ARGB_8888;
	else if(ddsd.ddpfPixelFormat.dwRGBBitCount==24)
		texture.format = ATI_TC_FORMAT_RGB_888;
	else if(GetFormat(ddsd.ddpfPixelFormat.dwPrivateFormatBitCount) != ATI_TC_FORMAT_Unknown)
		texture.format = GetFormat(ddsd.ddpfPixelFormat.dwPrivateFormatBitCount);
	else if(GetFormat(ddsd.ddpfPixelFormat.dwFourCC) != ATI_TC_FORMAT_Unknown)
		texture.format = GetFormat(ddsd.ddpfPixelFormat.dwFourCC);
	else
	{
		_tprintf(_T("Unsupported source format.\n"));
		fclose(pSourceFile);
		return false;
	}

	// Init source texture
	texture.dwDataSize = ATI_TC_CalculateBufferSize(&texture);
	texture.pData = (ATI_TC_BYTE*) malloc(texture.dwDataSize);

	fread(texture.pData, texture.dwDataSize, 1, pSourceFile);
	fclose(pSourceFile);

	return true;
}

void SaveDDSFile(const char* pszFile, ATI_TC_Texture& texture)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, pszFile, ("wb"));
	if(!pFile)
		return;

	fwrite(&DDS_HEADER, sizeof(DWORD), 1, pFile);

	DDSD2 ddsd;
	memset(&ddsd, 0, sizeof(DDSD2));
	ddsd.dwSize = sizeof(DDSD2);
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT|DDSD_MIPMAPCOUNT|DDSD_LINEARSIZE;
	ddsd.dwWidth = texture.dwWidth;
	ddsd.dwHeight = texture.dwHeight;
	ddsd.dwMipMapCount = 1;

	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE|DDSCAPS_COMPLEX|DDSCAPS_MIPMAP;

	ddsd.ddpfPixelFormat.dwFourCC = GetFourCC(texture.format);
	if(ddsd.ddpfPixelFormat.dwFourCC)
	{
		ddsd.dwLinearSize = texture.dwDataSize;
		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
		if(IsDXT5SwizzledFormat(texture.format))
		{
			ddsd.ddpfPixelFormat.dwPrivateFormatBitCount = ddsd.ddpfPixelFormat.dwFourCC;
			ddsd.ddpfPixelFormat.dwFourCC = FOURCC_DXT5;
		}
	}
	else
	{
		switch(texture.format)
		{
		case ATI_TC_FORMAT_ARGB_8888:
			ddsd.ddpfPixelFormat.dwRBitMask = 0x00ff0000;
			ddsd.ddpfPixelFormat.dwGBitMask = 0x0000ff00;
			ddsd.ddpfPixelFormat.dwBBitMask = 0x000000ff;
			ddsd.lPitch = texture.dwPitch;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
			ddsd.ddpfPixelFormat.dwFlags=DDPF_ALPHAPIXELS|DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xff000000;
			break;

		case ATI_TC_FORMAT_RGB_888:
			ddsd.ddpfPixelFormat.dwRBitMask = 0x00ff0000;
			ddsd.ddpfPixelFormat.dwGBitMask = 0x0000ff00;
			ddsd.ddpfPixelFormat.dwBBitMask = 0x000000ff;
			ddsd.lPitch = texture.dwPitch;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 24;
			ddsd.ddpfPixelFormat.dwFlags=DDPF_RGB;
			break;

		case ATI_TC_FORMAT_RG_8:
			ddsd.ddpfPixelFormat.dwRBitMask = 0x0000ff00;
			ddsd.ddpfPixelFormat.dwGBitMask = 0x000000ff;
			ddsd.lPitch = texture.dwPitch;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
			ddsd.ddpfPixelFormat.dwFlags=DDPF_ALPHAPIXELS|DDPF_LUMINANCE;
			ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xff000000;
			break;

		case ATI_TC_FORMAT_R_8:
			ddsd.ddpfPixelFormat.dwRBitMask = 0x000000ff;
			ddsd.lPitch = texture.dwPitch;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 8;
			ddsd.ddpfPixelFormat.dwFlags= DDPF_LUMINANCE;
			break;

		case ATI_TC_FORMAT_ARGB_2101010:
			ddsd.ddpfPixelFormat.dwRBitMask = 0x000003ff;
			ddsd.ddpfPixelFormat.dwGBitMask = 0x000ffc00;
			ddsd.ddpfPixelFormat.dwBBitMask = 0x3ff00000;
			ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xc0000000;
			ddsd.lPitch = texture.dwPitch;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
			ddsd.ddpfPixelFormat.dwFlags=DDPF_ALPHAPIXELS|DDPF_RGB;
			break;

		case ATI_TC_FORMAT_ARGB_16:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_ALPHAPIXELS;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_A16B16G16R16;
			break;

		case ATI_TC_FORMAT_RG_16:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_G16R16;
			break;

		case ATI_TC_FORMAT_R_16:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_L16;
			break;

		case ATI_TC_FORMAT_ARGB_16F:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_ALPHAPIXELS;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_A16B16G16R16F;
			break;

		case ATI_TC_FORMAT_RG_16F:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_G16R16F;
			break;

		case ATI_TC_FORMAT_R_16F:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_R16F;
			break;

		case ATI_TC_FORMAT_ARGB_32F:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_ALPHAPIXELS;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_A32B32G32R32F;
			break;

		case ATI_TC_FORMAT_RG_32F:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_G32R32F;
			break;

		case ATI_TC_FORMAT_R_32F:
			ddsd.dwLinearSize = texture.dwDataSize;
			ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC;
			ddsd.ddpfPixelFormat.dwFourCC = D3DFMT_R32F;
			break;

		default:
			assert(0);
			break;
		}
	}

	fwrite(&ddsd, sizeof(DDSD2), 1, pFile);
	fwrite(texture.pData, texture.dwDataSize, 1, pFile);

	fclose(pFile);
}


void ChangeDDSFile(const char* pszSourceFile, const char* pszDestFile, eTEXTURE_FORMAT textureFormat)
{
	// Load the source texture
	ATI_TC_Texture srcTexture;
	if(!LoadDDSFile(pszSourceFile, srcTexture))
		return;

	// Init dest texture
	ATI_TC_Texture destTexture;
	destTexture.dwSize = sizeof(destTexture);
	destTexture.dwWidth = srcTexture.dwWidth;
	destTexture.dwHeight = srcTexture.dwHeight;
	destTexture.dwPitch = 0;
	destTexture.format = GET_ATI_FORMAT(textureFormat);
	destTexture.dwDataSize = ATI_TC_CalculateBufferSize(&destTexture);
	destTexture.pData = (ATI_TC_BYTE*) malloc(destTexture.dwDataSize);

	ATI_TC_CompressOptions options;
	memset(&options, 0, sizeof(options));
	options.dwSize = sizeof(options);
	options.nCompressionSpeed = ATI_TC_Speed_Normal;

	ATI_TC_ConvertTexture(&srcTexture, &destTexture, &options, NULL, NULL, NULL);

	SaveDDSFile(pszDestFile, destTexture);

	free(srcTexture.pData);
	free(destTexture.pData);
}

void SaveTextureToPNGFile(const CResourceTexture* pTexture, eTEXTURE_FORMAT textureFormat)
{
	// TODO: bug
	/*
	GLOBAL::Engine()->RDevice()->SaveTextureToFile(pTexture, IMG_FILE_DDS, "temp.dds" );
	ChangeDDSFile("temp.dds", "temp.dds", COLOR_FORMAT_R8G8B8A8_TYPELESS);

	CResourceBase* pChangedTexture = GLOBAL::Engine()->Loader()->LoadForward( "temp.dds", "temp" , RESOURCE_FILE_TEXTURE );
	GLOBAL::Engine()->RDevice()->SaveTextureToFile( (CResourceTexture*)pChangedTexture, IMG_FILE_PNG, "temp.png" );
	GLOBAL::Engine()->AssetMgr()->Remove( pChangedTexture);*/
}