#include "stdafx.h"


#pragma warning( disable : 4819 ) 
#pragma warning( disable : 4244 ) 

#include "TResourceMgr.h"
#include "TMaterial.h"
#include "TMesh.h"
#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domConstants.h>
#include <dom/domProfile_COMMON.h>





uint32 TResourceMgr::RenderLoadTexture(const std::string& strFilePath)
{
	/*
	CTextureImage* pTexImage = NULL;

	pTexImage = g_loadedTextureCache[strFilePath];

	if(pTexImage == NULL)
	{
		pTexImage = LoadImageFromDisk(strFilePath);

		g_loadedTextureCache[strFilePath] = pTexImage;
	}else if(pTexImage->m_uiHardwareId != 0)
	{
		pTexImage->m_uiReferenceCount++;

		return pTexImage->m_uiHardwareId;
	}

	if(!pTexImage)
		return 0;

	pTexImage->m_uiReferenceCount = 1;

	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	GLuint uiTextureId = 0;
	glGenTextures(1, &uiTextureId);
	glBindTexture(GL_TEXTURE_2D, uiTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);//GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);//GL_LINEAR);

	bool bEnableAnisotropy = false;
	if(bEnableAnisotropy)
	{
		float32 fMaxAnisotropy = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fMaxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fMaxAnisotropy);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int32 nPixelFormat = GL_RGBA;

	if(pTexImage->GetPixelFormat() == CTextureImage::PX_RGB)
		nPixelFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, nPixelFormat, pTexImage->GetWidth(), pTexImage->GetHeight(), 0, nPixelFormat, GL_UNSIGNED_BYTE, pTexImage->GetImageBuffer());
	gluBuild2DMipmaps(GL_TEXTURE_2D, nPixelFormat, pTexImage->GetWidth(), pTexImage->GetHeight(), nPixelFormat, GL_UNSIGNED_BYTE, pTexImage->GetImageBuffer());

	glDisable(GL_TEXTURE_2D);

	pTexImage->m_uiHardwareId = uiTextureId;

	g_textureIdToImage[uiTextureId] = pTexImage;
*/
	return 0;
}


void LoadLambert(domProfile_COMMON* pProfile, domProfile_COMMON::domTechnique::domLambert* pLambert, sMaterial* pMaterial)
{
	domCommon_color_or_texture_type* pRefProp = NULL;
	domCommon_color_or_texture_type::domColor* pColor = NULL;
	domFx_color_common rgbColor;

	//Specular component.
	pRefProp = pLambert->getReflective();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		rgbColor = pColor->getValue();
	}
	//Ambient
	pRefProp = pLambert->getAmbient();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)
		{
			rgbColor = pColor->getValue();
			pMaterial->rgbAmbient.fRed = rgbColor[0];
			pMaterial->rgbAmbient.fGreen = rgbColor[1];
			pMaterial->rgbAmbient.fBlue = rgbColor[2];
		}
		pMaterial->rgbDiffuse.fAlpha = 1.0f;
	}

	//Diffuse
	pRefProp = pLambert->getDiffuse();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)
		{
			rgbColor = pColor->getValue();
			pMaterial->rgbDiffuse.fRed = rgbColor[0];
			pMaterial->rgbDiffuse.fGreen = rgbColor[1];
			pMaterial->rgbDiffuse.fBlue = rgbColor[2];
		}
		pMaterial->rgbDiffuse.fAlpha = 1.0f;

		// Texture loading code.
		//
		//
		domCommon_color_or_texture_type::domTextureRef txTexture = pRefProp->getTexture();
		if(txTexture)
		{
			std::string strTextureSamplerSid = txTexture->getTexture();

			domCommon_newparam_type_Array& arNewParams = pProfile->getNewparam_array();
			for(uint32 uiCurrParam = 0; uiCurrParam < arNewParams.getCount(); uiCurrParam++)
			{
				if(arNewParams[uiCurrParam]->getSid() == strTextureSamplerSid)
				{
					domFx_sampler2D_commonRef r2dSample = arNewParams[uiCurrParam]->getSampler2D();
					std::string strSurfaceSid = r2dSample->getSource()->getValue();
					for(uint32 uiSurfaceSearchIdx = 0; uiSurfaceSearchIdx < arNewParams.getCount(); uiSurfaceSearchIdx++)
					{
						if(arNewParams[uiSurfaceSearchIdx]->getSid() == strSurfaceSid)
						{	
							domFx_surface_commonRef rSurface = arNewParams[uiSurfaceSearchIdx]->getSurface();
							domFx_surface_init_from_common_Array& arInitData = rSurface->getFx_surface_init_common()->getInit_from_array();

							for(uint32 uiInitDataIdx = 0; uiInitDataIdx < arInitData.getCount(); uiInitDataIdx++)
							{
								domImage* pImage = daeSafeCast<domImage>(arInitData[uiInitDataIdx]->getValue().getElement());
								domImage::domInit_fromRef rInitRefData = pImage->getInit_from();
								xsAnyURI& strURI = rInitRefData->getValue();

								std::string strDir;
								std::string strFileName;
								std::string strExt;

								strURI.pathComponents(strDir, strFileName, strExt);

								if(strExt == ".tga")
								{
									//TODO
									std::string strFilePath = strFileName + strExt;
									uint32 nTextureId = 1;//RenderLoadTexture(strFilePath);

									pMaterial->nTextureIds = new uint32[1];
									pMaterial->nTextureIds[0] = nTextureId;
									pMaterial->uiTextureCount = 1;
								}else
									assert(false);
							}
						}
					}
				}
			}
		}
	}

	//Emission
	pRefProp = pLambert->getEmission();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)
		{
			rgbColor = pColor->getValue();
			pMaterial->rgbEmissive.fRed = rgbColor[0];
			pMaterial->rgbEmissive.fGreen = rgbColor[1];
			pMaterial->rgbEmissive.fBlue = rgbColor[2];
		}
		pMaterial->rgbEmissive.fAlpha = 1.0f;
	}	
}


void LoadBlinn(domProfile_COMMON* pProfile, domProfile_COMMON::domTechnique::domBlinn* pBlinn, sMaterial* pMaterial)
{
	domCommon_color_or_texture_type* pRefProp = NULL;
	domCommon_color_or_texture_type::domColor* pColor = NULL;
	domFx_color_common rgbColor;

	bool bHadDiffuseColor = false;
	bool bHadAmbientColor = false;

	//Specular component.
	pRefProp = pBlinn->getReflective();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)	
		{
			rgbColor = pColor->getValue();
		}
	}
	//Ambient
	pRefProp = pBlinn->getAmbient();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)
		{
			rgbColor = pColor->getValue();
			pMaterial->rgbAmbient.fRed = rgbColor[0];
			pMaterial->rgbAmbient.fGreen = rgbColor[1];
			pMaterial->rgbAmbient.fBlue = rgbColor[2];
			pMaterial->rgbAmbient.fAlpha = rgbColor[3];
			bHadAmbientColor = true;
		}else
		{
			pMaterial->rgbAmbient.fAlpha = 1.0f;
		}
	}

	//Diffuse
	pRefProp = pBlinn->getDiffuse();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)
		{
			rgbColor = pColor->getValue();
			pMaterial->rgbDiffuse.fRed = rgbColor[0];
			pMaterial->rgbDiffuse.fGreen = rgbColor[1];
			pMaterial->rgbDiffuse.fBlue = rgbColor[2];
			pMaterial->rgbDiffuse.fAlpha = rgbColor[3];
			bHadDiffuseColor = true;

		}else
		{
			pMaterial->rgbDiffuse.fAlpha = 1.0f;
		}

		// Texture loading code.
		//
		//
		domCommon_color_or_texture_type::domTextureRef txTexture = pRefProp->getTexture();
		if(txTexture)
		{
			bHadAmbientColor = false;

			if(bHadDiffuseColor == false)
			{
				pMaterial->rgbDiffuse.fRed = 1.0f;
				pMaterial->rgbDiffuse.fGreen = 1.0f;
				pMaterial->rgbDiffuse.fBlue = 1.0f;
			}

			if(bHadAmbientColor == false)
			{
				pMaterial->rgbAmbient.fRed = 0.5f;
				pMaterial->rgbAmbient.fGreen = 0.5f;
				pMaterial->rgbAmbient.fBlue = 0.5f;
			}
			std::string strTextureSamplerSid = txTexture->getTexture();

			domCommon_newparam_type_Array& arNewParams = pProfile->getNewparam_array();
			for(uint32 uiCurrParam = 0; uiCurrParam < arNewParams.getCount(); uiCurrParam++)
			{
				if(arNewParams[uiCurrParam]->getSid() == strTextureSamplerSid)
				{
					domFx_sampler2D_commonRef r2dSample = arNewParams[uiCurrParam]->getSampler2D();
					std::string strSurfaceSid = r2dSample->getSource()->getValue();
					for(uint32 uiSurfaceSearchIdx = 0; uiSurfaceSearchIdx < arNewParams.getCount(); uiSurfaceSearchIdx++)
					{
						if(arNewParams[uiSurfaceSearchIdx]->getSid() == strSurfaceSid)
						{	
							domFx_surface_commonRef rSurface = arNewParams[uiSurfaceSearchIdx]->getSurface();
							domFx_surface_init_from_common_Array& arInitData = rSurface->getFx_surface_init_common()->getInit_from_array();

							for(uint32 uiInitDataIdx = 0; uiInitDataIdx < arInitData.getCount(); uiInitDataIdx++)
							{
								domImage* pImage = daeSafeCast<domImage>(arInitData[uiInitDataIdx]->getValue().getElement());
								domImage::domInit_fromRef rInitRefData = pImage->getInit_from();
								xsAnyURI& strURI = rInitRefData->getValue();

								std::string strDir;
								std::string strFileName;
								std::string strExt;

								strURI.pathComponents(strDir, strFileName, strExt);

								if(strExt == ".tga")
								{
									//TODO
									std::string strFilePath = strFileName + strExt;
									uint32 nTextureId = 1;//RenderLoadTexture(strFilePath);

									pMaterial->nTextureIds = new uint32[1];
									pMaterial->nTextureIds[0] = nTextureId;
									pMaterial->uiTextureCount = 1;
								}else
									assert(false);
							}
						}
					}
				}
			}
		}
	}

	//Emission
	pRefProp = pBlinn->getEmission();
	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)
		{
			rgbColor = pColor->getValue();
			pMaterial->rgbEmissive.fRed = rgbColor[0];
			pMaterial->rgbEmissive.fGreen = rgbColor[1];
			pMaterial->rgbEmissive.fBlue = rgbColor[2];
			pMaterial->rgbEmissive.fAlpha = rgbColor[3];

		}else
		{
			pMaterial->rgbEmissive.fAlpha = 1.0f;
		}
	}

	pRefProp = pBlinn->getSpecular();

	if(pRefProp)
	{
		pColor = pRefProp->getColor();
		if(pColor)
		{
			rgbColor = pColor->getValue();
			pMaterial->rgbSpecular.fRed = rgbColor[0];
			pMaterial->rgbSpecular.fGreen = rgbColor[1];
			pMaterial->rgbSpecular.fBlue = rgbColor[2];
			pMaterial->rgbSpecular.fAlpha = rgbColor[3];

		}else
		{
			pMaterial->rgbSpecular.fAlpha = 1.0f;
		}
	}
}



bool TResourceMgr::LoadDae()
{
	DAE* pColladaDom = new DAE();

	int nError = pColladaDom->load("test.dae");
	bool bIs3dStudio = false;

	domAsset::domContributor::domAuthoring_tool* pAuthorTool = NULL;
	pColladaDom->getDatabase()->getElement((daeElement**)&pAuthorTool, 0, NULL, COLLADA_TYPE_AUTHORING_TOOL, NULL);
	if (pAuthorTool)
	{
		std::string strAuthorTool = pAuthorTool->getValue();
		if(strAuthorTool.find("3ds") != -1)
		{
			bIs3dStudio = true;
		}
	}

	sMesh* pNewMesh = new sMesh;
	sMaterial* pNewMaterial = new sMaterial;

	memset(pNewMesh, 0, sizeof(sMesh));
	memset(pNewMaterial, 0, sizeof(sMaterial));

	pNewMesh->uiMaterialCount = 0;
	pNewMesh->pMaterials = pNewMaterial;


	//Load the materials.
	uint32 nMaterialCount = pColladaDom->getDatabase()->getElementCount(NULL, COLLADA_TYPE_MATERIAL, NULL);
	for(uint32 nCurrentMaterial = 0; nCurrentMaterial < nMaterialCount; nCurrentMaterial++)
	{
		domMaterial* pThisMaterial = NULL;
		uint32 nError = pColladaDom->getDatabase()->getElement((daeElement**)&pThisMaterial, nCurrentMaterial, NULL, COLLADA_TYPE_MATERIAL, NULL);

		if(pThisMaterial)
		{
			std::string strMaterialName = pThisMaterial->getName();
			domInstance_effect* pInstanceEffect = pThisMaterial->getInstance_effect();
			domEffect* pEffect = daeSafeCast<domEffect>(pInstanceEffect->getUrl().getElement());

			//Only supports the common profile.
			//
			domProfile_COMMON* pProfileData = daeSafeCast<domProfile_COMMON>(pEffect->getChild("profile_COMMON"));

			if(pProfileData)
			{
				domProfile_COMMON::domTechnique* pTechnique = pProfileData->getTechnique();
				domProfile_COMMON::domTechnique::domLambert* pLambertProps = pTechnique->getLambert();
				domProfile_COMMON::domTechnique::domBlinn*   pBlinn = pTechnique->getBlinn();

				if(pLambertProps)
					LoadLambert(pProfileData, pLambertProps, pNewMaterial);

				if(pBlinn)
					LoadBlinn(pProfileData, pBlinn, pNewMaterial);
			}
		}
	}


	//Load the geometries
	//The way the data is represented in the engine,
	//1 Collada geometry instance is equivalent to 1 sMesh
	//
	uint32 uiGeometryCount = pColladaDom->getDatabase()->getElementCount(NULL, COLLADA_TYPE_GEOMETRY, NULL);
	for (unsigned int i = 0; i <= uiGeometryCount; i++)
	{ 
		domGeometry* pThisGeometry = NULL;
		uint32 nError = pColladaDom->getDatabase()->getElement((daeElement**)&pThisGeometry, i, NULL, COLLADA_TYPE_GEOMETRY, NULL);

		if(pThisGeometry)
		{
			std::string strGeometryName = pThisGeometry->getName();
			domMesh* pMesh = pThisGeometry->getMesh();

			pNewMesh->strMeshName = strGeometryName;

			//Mesh only supports one list of triangles per set
			//
			if(pMesh)
			{
				domTriangles_Array arTrianglesArray = pMesh->getTriangles_array();

				//
				uint32 uiTriangleCount = arTrianglesArray.getCount();

				if(uiTriangleCount != 0)
					uiTriangleCount = 1;

				assert(uiTriangleCount == 1);
				//Get the index array! ONLY SUPPORTS 1!
				//

				uint32* pIdxArray = 0;
				uint32  uiIndexCount = 0;
				uint32  uiInputCount = 0;

				uint32 nPositionOffset = 0;
				uint32 nNormalOffset = 0;
				uint32 nTexOffset = 0;

				std::string strMaterialName;

				for(uint32 uiCurrentTriangle = 0; uiCurrentTriangle < uiTriangleCount; uiCurrentTriangle++)
				{
					domTriangles* pTriangleIndex = arTrianglesArray[uiCurrentTriangle];

					strMaterialName = pTriangleIndex->getMaterial();

					domInputLocalOffset_Array arInputOffsets = pTriangleIndex->getInput_array();

					uiInputCount = arInputOffsets.getCount();

					for(uint32 uiCurrentIO = 0; uiCurrentIO < arInputOffsets.getCount(); uiCurrentIO++)
					{
						domInputLocalOffset* pCurrentOffset = arInputOffsets[uiCurrentIO];

						if(strcmp(pCurrentOffset->getSemantic(), "VERTEX") == 0)
						{
							nPositionOffset = pCurrentOffset->getOffset();
						}else if(strcmp(pCurrentOffset->getSemantic(), "NORMAL") == 0)
						{
							nNormalOffset = pCurrentOffset->getOffset();
						}else if(strcmp(pCurrentOffset->getSemantic(), "TEXCOORD") == 0)
						{
							nTexOffset = pCurrentOffset->getOffset();
						}
					}

					domPRef pPolyInfo = pTriangleIndex->getP();

					domListOfUInts idxArray = pPolyInfo->getValue();


					// Index array in Collada is stored depending on the number of 
					// input on the polygon, therefore the indices are not shared across vertices.
					//
					//
					uiIndexCount	= idxArray.getCount();
					pIdxArray		= new uint32[uiIndexCount];

					memset(pIdxArray, 0, sizeof(uint32) * (uiIndexCount));

					for(uint32 uiIdxCount = 0; uiIdxCount < idxArray.getCount(); uiIdxCount++)
					{
						pIdxArray[uiIdxCount] = idxArray[uiIdxCount];
					}
				}


				// Load Sources.
				//
				uint32 uiVertexCount = 0;
				float32* pfVertices = 0;

				uint32 uiNormalCount = 0;
				float32* pfNormals = 0;

				uint32 uiTexCoordCount = 0;
				float32* pfTexCoords = 0;

				for(uint32 uiCurrentSource = 0; uiCurrentSource < pMesh->getSource_array().getCount(); uiCurrentSource++)
				{
					domSource* pSource = pMesh->getSource_array()[uiCurrentSource];

					bool bIsPosition = false;
					bool bIsNormal = false;
					bool bIsTexCoord = false;

					std::string strName = (pSource->getName()) ? pSource->getName() : "";
					std::string strID = pSource->getID();

					if(strName == "position" || strID.find("positions") != -1)
						bIsPosition = true;
					if(strName == "normal" || strID.find("normals") != -1)
						bIsNormal = true;
					if(strName == "map1" || strID.find("channel1") != -1)
						bIsTexCoord = true;

					if(bIsPosition)
					{
						domListOfFloats vxArray = pSource->getFloat_array()->getValue();

						uiVertexCount = vxArray.getCount();
						pfVertices = new float32[uiVertexCount];

						memset(pfVertices, 0, sizeof(float32) * uiVertexCount);

						for(uint32 uiVtxCount = 0; uiVtxCount < vxArray.getCount(); uiVtxCount+=3)
						{
							if(bIs3dStudio)
							{
								pfVertices[uiVtxCount] = vxArray[uiVtxCount];
								pfVertices[uiVtxCount+1] = vxArray[uiVtxCount + 2];
								pfVertices[uiVtxCount+2] = vxArray[uiVtxCount + 1];
							}else
							{
								pfVertices[uiVtxCount] = vxArray[uiVtxCount];
								pfVertices[uiVtxCount+1] = vxArray[uiVtxCount + 1];
								pfVertices[uiVtxCount+2] = vxArray[uiVtxCount + 2];
							}
						}

					}else if(bIsNormal)
					{
						domListOfFloats vxArray = pSource->getFloat_array()->getValue();

						uiNormalCount = vxArray.getCount();
						pfNormals = new float32[uiNormalCount];

						memset(pfNormals, 0, sizeof(float32) * uiNormalCount);

						for(uint32 uiNrmlCount = 0; uiNrmlCount < vxArray.getCount(); uiNrmlCount++)
						{
							pfNormals[uiNrmlCount] = vxArray[uiNrmlCount];
						}
					}else if(bIsTexCoord)
					{
						domListOfFloats vxArray = pSource->getFloat_array()->getValue();

						uiTexCoordCount = vxArray.getCount();
						pfTexCoords = new float32[uiTexCoordCount];

						memset(pfTexCoords, 0, sizeof(float32) * uiTexCoordCount);

						for(uint32 uiTexCount = 0; uiTexCount < vxArray.getCount(); uiTexCount++)
						{
							pfTexCoords[uiTexCount] = vxArray[uiTexCount];
						}
					}

				}

				//Once the source have been loaded, 
				//Unroll them based on the idx array.
				//
				uint32 uiNumberOfComponents = 3;
				uint32 uiNumberOfTxComponents = (bIs3dStudio) ? 3 : 2;
				uint32 nActualNormalCount = (uiIndexCount / uiInputCount);
				uint32 nActualVertexCount = (uiIndexCount / uiInputCount);
				uint32 nActualTexCoordCount = (uiIndexCount / uiNumberOfTxComponents);

				pNewMesh->uiNumVertices = nActualVertexCount;
				pNewMesh->uiNumNormals = nActualNormalCount;
				pNewMesh->uiNumTexcoords = nActualTexCoordCount;

				pNewMesh->uiNumVertexIdx = nActualVertexCount * uiNumberOfComponents;
				pNewMesh->uiNumNormalIdx = nActualNormalCount * uiNumberOfComponents;
				pNewMesh->uiNumTexCoordIdx = nActualTexCoordCount * uiNumberOfTxComponents;

				pNewMesh->pfVertices = new float32[pNewMesh->uiNumVertexIdx];
				pNewMesh->pfNormals = new float32[pNewMesh->uiNumNormalIdx];
				pNewMesh->pfTextureCoordinates = new float32*[1];
				pNewMesh->pfTextureCoordinates[0] = new float32[pNewMesh->uiNumTexCoordIdx];

				memset(pNewMesh->pfVertices, 0, sizeof(float32) * pNewMesh->uiNumVertexIdx);
				memset(pNewMesh->pfNormals, 0, sizeof(float32) * pNewMesh->uiNumNormalIdx);
				memset(pNewMesh->pfTextureCoordinates[0], 0, sizeof(float32) * pNewMesh->uiNumTexCoordIdx);

				uint32 nCurrentVertex = 0;
				uint32 nCurrentNormal = 0;
				uint32 nCurrentTexCoord = 0;

				for(uint32 nTriCurrentIdx = 0; nTriCurrentIdx < uiIndexCount; nTriCurrentIdx+=uiInputCount)
				{
					uint32 nVertexIdx = pIdxArray[nTriCurrentIdx + nPositionOffset] * uiNumberOfComponents;
					pNewMesh->pfVertices[nCurrentVertex] = pfVertices[nVertexIdx];						
					pNewMesh->pfVertices[nCurrentVertex + 1] = pfVertices[nVertexIdx + 1];						
					pNewMesh->pfVertices[nCurrentVertex + 2] = pfVertices[nVertexIdx + 2];
					nCurrentVertex += uiNumberOfComponents;

					uint32 nNormalIdx = pIdxArray[nTriCurrentIdx + nNormalOffset] * uiNumberOfComponents;
					pNewMesh->pfNormals[nCurrentNormal] = pfNormals[nNormalIdx];						
					pNewMesh->pfNormals[nCurrentNormal + 1] = pfNormals[nNormalIdx + 1];						
					pNewMesh->pfNormals[nCurrentNormal + 2] = pfNormals[nNormalIdx + 2];
					nCurrentNormal += uiNumberOfComponents;

					uint32 nTexCoordIdx = pIdxArray[nTriCurrentIdx + nTexOffset] * uiNumberOfTxComponents;
					pNewMesh->pfTextureCoordinates[0][nCurrentTexCoord] = pfTexCoords[nTexCoordIdx];						
					pNewMesh->pfTextureCoordinates[0][nCurrentTexCoord + 1] = pfTexCoords[nTexCoordIdx + 1];						
					nCurrentTexCoord += 2;
				}
			}
		}
	}
	return true;
}