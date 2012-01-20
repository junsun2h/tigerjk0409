#pragma once

#include "CResource.h"



void ChangeDDSFile(const char* pszSourceFile,  const char* pszDestFile, eTEXTURE_FORMAT textureFormat);
void SaveTextureToPNGFile(const CResourceTexture* pTexture, eTEXTURE_FORMAT textureFormat);