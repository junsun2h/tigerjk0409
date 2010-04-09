#pragma once

struct sMaterial;

struct sMesh
{
	std::string strMeshName;

	uint32 uiNumVertices;
	uint32 uiNumNormals;
	uint32 uiNumTexcoords;
	uint32 uiNumColors;

	uint32 uiNumVertexIdx;		//The number of vertices
	uint32 uiNumNormalIdx;
	uint32 uiNumTexCoordIdx;	//The texture coordinate count
	uint32 uiNumColorIdx;

	uint32 uiPolyType;			//The type of polygons the vertices are order in.
	uint32 uiIdxCount;

	float32* pfVertices;				//Pointer to array of verts.
	float32* pfNormals;					//Pointer to array of verts.
	float32** pfTextureCoordinates;		//Pointer to an array of texture coords [coord_set][coord]
	float32* pfColors;				//Color pointer
	uint32* pIdxArray;

	sMaterial* pMaterials;
	uint32	   uiMaterialCount;

	bool	m_bBoundsSet;

	uint32 m_nVertexVBO;
	uint32 m_nNormalVBO;
	uint32 m_nTexCoordVBO;
	uint32 m_nColorVBO;

	sMesh() 
		: pMaterials(NULL)
		, pfVertices(NULL)
		, pfNormals(NULL)
		, pfTextureCoordinates(NULL)
		, pfColors(NULL)
		, pIdxArray(NULL)
		, m_bBoundsSet(false)
		, m_nNormalVBO(0)
		, m_nVertexVBO(0)
		, m_nTexCoordVBO(0)
	{

	}
};