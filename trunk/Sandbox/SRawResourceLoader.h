#pragma once


namespace SRAW_FILE_LOADER
{

	typedef std::map<int, std::vector<int>* > MTRL_FACE_MAP;


	struct RAW_WEIGHT
	{
		int		count;
		float	weight[3];
		uint8	bone[4];
	};

	struct RAW_ACTOR_NODE
	{
		CQuat			rot;
		CVector3		pos;
		std::string		name;
		std::string		parentName;
	};

	struct RAW_MOTION_KEY
	{
		CQuat			rot;
		CVector3		pos;
	};

	struct RAW_MOTION_NODE
	{
		std::string		name;
		std::string		parentName;
	};

	enum TCOODSYS
	{
		COODSYS_3DSMAX,
		COODSYS_DIRECTX,
		COODSYS_OPENGL
	};


	struct SRAW_MESH
	{
		SRAW_MESH();
		~SRAW_MESH();
		void SRAW_MESH::ChangeCoordsys(TCOODSYS coodSys_);

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

	struct SRAW_ACTOR
	{
		SRAW_ACTOR()
		{
			coordSys = COODSYS_3DSMAX;
		}

		void ChangeCoordsys(TCOODSYS coodSys_);

		TCOODSYS	coordSys;

		std::vector<std::string>	meshNames;
		std::vector<RAW_ACTOR_NODE>	joints;
	};

	struct SRAW_MOTION
	{
		SRAW_MOTION()
		{
			coordSys = COODSYS_3DSMAX;
		}

		void ChangeCoordsys(TCOODSYS coodSys_);

		TCOODSYS	coordSys;

		uint8		frameRate;
		UINT		totalFrame;
		uint8		frameInterval;

		std::vector<std::vector<RAW_MOTION_KEY>>	keys;
		std::vector<RAW_MOTION_NODE>				joints;
	};

	bool LoadRawMesh(const char* strFileName, SRAW_MESH& RawMesh);
	bool LoadRawActor(const char* strFileName, SRAW_ACTOR& RawMesh);
	bool LoadRawMotion(const char* strFileName, SRAW_MOTION& RawMesh);

	void SaveRawMeshToFile( SRAW_MESH* pRawMesh, wxString name );
	void SaveRawActorToFile( SRAW_ACTOR* pRawActor, wxString name );
	void SaveRawMotionToFile( SRAW_MOTION* pRawMotion, wxString name );

	void SaveActorToFile( const CResourceActor* pActor, wxString fullPath );
};


