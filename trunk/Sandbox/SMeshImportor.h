#pragma once

#include "SGlobal.h"
#include "CDefine.h"
#include <vector>
#include <map>

typedef std::map<int, std::vector<int>* > MTRL_FACE_MAP;


struct RAW_WEIGHT
{
	int		count;
	float	weight[3];
	uint8	bone[4];
};

enum TCOODSYS
{
	COODSYS_RAW,
	COODSYS_DIRECTX,
	COODSYS_OPENGL
};


struct TRAW_MESH
{
	TRAW_MESH();
	~TRAW_MESH();
	void TRAW_MESH::ChangeCoordsys(TCOODSYS coodSys_);

	CVector3	postion;
	CQuat		rotation;

	CVector3	BBoxMin;
	CVector3	BBoxMax;
	TCOODSYS	coordSys;

	std::vector<CVector3> posList;
	std::vector<CVector2> uvList;
	std::vector<CVector3> normalList;
	std::vector<RAW_WEIGHT> weightList;

	std::vector<int> facePos;
	std::vector<int> faceNormal;
	std::vector<int> faceUV;

	MTRL_FACE_MAP faceMtrlID;
	std::vector<wxString> mtrlList;
};


bool LoadRawMesh(const char* strFileName, TRAW_MESH& RawMesh);
void ImportRawMesh( TRAW_MESH* pRawMesh, wxString name );

