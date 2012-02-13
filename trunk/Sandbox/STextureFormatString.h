#pragma once


const char* PixelFormatToString( eTEXTURE_FORMAT format )
{
	const char* pstr = NULL;
	switch( format )
	{
	case COLOR_FORMAT_R32G32B32A32_TYPELESS:
		pstr = "R32G32B32A32_TYPELESS"; break;
	case COLOR_FORMAT_R32G32B32A32_FLOAT:
		pstr = "R32G32B32A32_FLOAT"; break;
	case COLOR_FORMAT_R32G32B32A32_UINT:
		pstr = "R32G32B32A32_UINT"; break;
	case COLOR_FORMAT_R32G32B32A32_SINT:
		pstr = "R32G32B32A32_SINT"; break;
	case COLOR_FORMAT_R32G32B32_TYPELESS:
		pstr = "R32G32B32_TYPELESS"; break;
	case COLOR_FORMAT_R32G32B32_FLOAT:
		pstr = "R32G32B32_FLOAT"; break;
	case COLOR_FORMAT_R32G32B32_UINT:
		pstr = "R32G32B32_UINT"; break;
	case COLOR_FORMAT_R32G32B32_SINT:
		pstr = "R32G32B32_SINT"; break;
	case COLOR_FORMAT_R16G16B16A16_TYPELESS:
		pstr = "R16G16B16A16_TYPELESS"; break;
	case COLOR_FORMAT_R16G16B16A16_FLOAT:
		pstr = "R16G16B16A16_FLOAT"; break;
	case COLOR_FORMAT_R16G16B16A16_UNORM:
		pstr = "R16G16B16A16_UNORM"; break;
	case COLOR_FORMAT_R16G16B16A16_UINT:
		pstr = "R16G16B16A16_UINT"; break;
	case COLOR_FORMAT_R16G16B16A16_SNORM:
		pstr = "R16G16B16A16_SNORM"; break;
	case COLOR_FORMAT_R16G16B16A16_SINT:
		pstr = "R16G16B16A16_SINT"; break;
	case COLOR_FORMAT_R32G32_TYPELESS:
		pstr = "R32G32_TYPELESS"; break;
	case COLOR_FORMAT_R32G32_FLOAT:
		pstr = "R32G32_FLOAT"; break;
	case COLOR_FORMAT_R32G32_UINT:
		pstr = "R32G32_UINT"; break;
	case COLOR_FORMAT_R32G32_SINT:
		pstr = "R32G32_SINT"; break;
	case COLOR_FORMAT_R32G8X24_TYPELESS:
		pstr = "R32G8X24_TYPELESS"; break;
	case COLOR_FORMAT_D32_FLOAT_S8X24_UINT:
		pstr = "D32_FLOAT_S8X24_UINT"; break;
	case COLOR_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		pstr = "R32_FLOAT_X8X24_TYPELESS"; break;
	case COLOR_FORMAT_X32_TYPELESS_G8X24_UINT:
		pstr = "X32_TYPELESS_G8X24_UINT"; break;
	case COLOR_FORMAT_R10G10B10A2_TYPELESS:
		pstr = "R10G10B10A2_TYPELESS"; break;
	case COLOR_FORMAT_R10G10B10A2_UNORM:
		pstr = "R10G10B10A2_UNORM"; break;
	case COLOR_FORMAT_R10G10B10A2_UINT:
		pstr = "R10G10B10A2_UINT"; break;
	case COLOR_FORMAT_R11G11B10_FLOAT:
		pstr = "R11G11B10_FLOAT"; break;
	case COLOR_FORMAT_R8G8B8A8_TYPELESS:
		pstr = "R8G8B8A8_TYPELESS"; break;
	case COLOR_FORMAT_R8G8B8A8_UNORM:
		pstr = "R8G8B8A8_UNORM"; break;
	case COLOR_FORMAT_R8G8B8A8_UNORM_SRGB:
		pstr = "R8G8B8A8_UNORM_SRGB"; break;
	case COLOR_FORMAT_R8G8B8A8_UINT:
		pstr = "R8G8B8A8_UINT"; break;
	case COLOR_FORMAT_R8G8B8A8_SNORM:
		pstr = "R8G8B8A8_SNORM"; break;
	case COLOR_FORMAT_R8G8B8A8_SINT:
		pstr = "R8G8B8A8_SINT"; break;
	case COLOR_FORMAT_R16G16_TYPELESS:
		pstr = "R16G16_TYPELESS"; break;
	case COLOR_FORMAT_R16G16_FLOAT:
		pstr = "R16G16_FLOAT"; break;
	case COLOR_FORMAT_R16G16_UNORM:
		pstr = "R16G16_UNORM"; break;
	case COLOR_FORMAT_R16G16_UINT:
		pstr = "R16G16_UINT"; break;
	case COLOR_FORMAT_R16G16_SNORM:
		pstr = "R16G16_SNORM"; break;
	case COLOR_FORMAT_R16G16_SINT:
		pstr = "R16G16_SINT"; break;
	case COLOR_FORMAT_R32_TYPELESS:
		pstr = "R32_TYPELESS"; break;
	case COLOR_FORMAT_D32_FLOAT:
		pstr = "D32_FLOAT"; break;
	case COLOR_FORMAT_R32_FLOAT:
		pstr = "R32_FLOAT"; break;
	case COLOR_FORMAT_R32_UINT:
		pstr = "R32_UINT"; break;
	case COLOR_FORMAT_R32_SINT:
		pstr = "R32_SINT"; break;
	case COLOR_FORMAT_R24G8_TYPELESS:
		pstr = "R24G8_TYPELESS"; break;
	case COLOR_FORMAT_D24_UNORM_S8_UINT:
		pstr = "D24_UNORM_S8_UINT"; break;
	case COLOR_FORMAT_R24_UNORM_X8_TYPELESS:
		pstr = "R24_UNORM_X8_TYPELESS"; break;
	case COLOR_FORMAT_X24_TYPELESS_G8_UINT:
		pstr = "X24_TYPELESS_G8_UINT"; break;
	case COLOR_FORMAT_R8G8_TYPELESS:
		pstr = "R8G8_TYPELESS"; break;
	case COLOR_FORMAT_R8G8_UNORM:
		pstr = "R8G8_UNORM"; break;
	case COLOR_FORMAT_R8G8_UINT:
		pstr = "R8G8_UINT"; break;
	case COLOR_FORMAT_R8G8_SNORM:
		pstr = "R8G8_SNORM"; break;
	case COLOR_FORMAT_R8G8_SINT:
		pstr = "R8G8_SINT"; break;
	case COLOR_FORMAT_R16_TYPELESS:
		pstr = "R16_TYPELESS"; break;
	case COLOR_FORMAT_R16_FLOAT:
		pstr = "R16_FLOAT"; break;
	case COLOR_FORMAT_D16_UNORM:
		pstr = "D16_UNORM"; break;
	case COLOR_FORMAT_R16_UNORM:
		pstr = "R16_UNORM"; break;
	case COLOR_FORMAT_R16_UINT:
		pstr = "R16_UINT"; break;
	case COLOR_FORMAT_R16_SNORM:
		pstr = "R16_SNORM"; break;
	case COLOR_FORMAT_R16_SINT:
		pstr = "R16_SINT"; break;
	case COLOR_FORMAT_R8_TYPELESS:
		pstr = "R8_TYPELESS"; break;
	case COLOR_FORMAT_R8_UNORM:
		pstr = "R8_UNORM"; break;
	case COLOR_FORMAT_R8_UINT:
		pstr = "R8_UINT"; break;
	case COLOR_FORMAT_R8_SNORM:
		pstr = "R8_SNORM"; break;
	case COLOR_FORMAT_R8_SINT:
		pstr = "R8_SINT"; break;
	case COLOR_FORMAT_A8_UNORM:
		pstr = "A8_UNORM"; break;
	case COLOR_FORMAT_R1_UNORM:
		pstr = "R1_UNORM"; break;
	case COLOR_FORMAT_R9G9B9E5_SHAREDEXP:
		pstr = "R9G9B9E5_SHAREDEXP"; break;
	case COLOR_FORMAT_R8G8_B8G8_UNORM:
		pstr = "R8G8_B8G8_UNORM"; break;
	case COLOR_FORMAT_G8R8_G8B8_UNORM:
		pstr = "G8R8_G8B8_UNORM"; break;
	case COLOR_FORMAT_BC1_TYPELESS:
		pstr = "BC1_TYPELESS"; break;
	case COLOR_FORMAT_BC1_UNORM:
		pstr = "BC1_UNORM"; break;
	case COLOR_FORMAT_BC1_UNORM_SRGB:
		pstr = "BC1_UNORM_SRGB"; break;
	case COLOR_FORMAT_BC2_TYPELESS:
		pstr = "BC2_TYPELESS"; break;
	case COLOR_FORMAT_BC2_UNORM:
		pstr = "BC2_UNORM"; break;
	case COLOR_FORMAT_BC2_UNORM_SRGB:
		pstr = "BC2_UNORM_SRGB"; break;
	case COLOR_FORMAT_BC3_TYPELESS:
		pstr = "BC3_TYPELESS"; break;
	case COLOR_FORMAT_BC3_UNORM:
		pstr = "BC3_UNORM"; break;
	case COLOR_FORMAT_BC3_UNORM_SRGB:
		pstr = "BC3_UNORM_SRGB"; break;
	case COLOR_FORMAT_BC4_TYPELESS:
		pstr = "BC4_TYPELESS"; break;
	case COLOR_FORMAT_BC4_UNORM:
		pstr = "BC4_UNORM"; break;
	case COLOR_FORMAT_BC4_SNORM:
		pstr = "BC4_SNORM"; break;
	case COLOR_FORMAT_BC5_TYPELESS:
		pstr = "BC5_TYPELESS"; break;
	case COLOR_FORMAT_BC5_UNORM:
		pstr = "BC5_UNORM"; break;
	case COLOR_FORMAT_BC5_SNORM:
		pstr = "BC5_SNORM"; break;
	case COLOR_FORMAT_B5G6R5_UNORM:
		pstr = "B5G6R5_UNORM"; break;
	case COLOR_FORMAT_B5G5R5A1_UNORM:
		pstr = "B5G5R5A1_UNORM"; break;
	case COLOR_FORMAT_B8G8R8A8_UNORM:
		pstr = "B8G8R8A8_UNORM"; break;
	default:
		pstr = "Unknown format"; break;
	}

	return pstr;
}
