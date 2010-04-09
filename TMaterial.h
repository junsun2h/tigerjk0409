#pragma once


struct sMaterial
{
	std::string strMaterialName;

	sRGBColor rgbAmbient;
	sRGBColor rgbEmissive;
	sRGBColor rgbDiffuse;
	sRGBColor rgbSpecular;

	uint32* nTextureIds;				//List of texture ids
	uint32 uiTextureCount;		//The number of textures.
};