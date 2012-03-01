#pragma once


struct IEntity;
class CResourceMotion;
class CResourceActor;


struct CMotionDesc
{
	float	fBlendInTime;
	float	fBlendOutTime;
	float	fTargetWeight;
	bool	bLoop;
	bool	bBlendOutBeforeEnd;
	int		Priority;

	float	timePerFrame;
	float	length;

	const CResourceMotion*	pResource;
	const CResourceActor*	pResourceActor;

	CMotionDesc()
		: fBlendInTime(0.3f)
		, fBlendOutTime(0.3f)
		, fTargetWeight(1.0f)
		, bBlendOutBeforeEnd(true)
		, bLoop(false)
		, Priority(0)
		, pResource(NULL)
		, pResourceActor(NULL)
	{
	}
};


enum eMOTION_PLAY_STATE
{
	MOTION_PLAY_FADE_IN,
	MOTION_PLAY,
	MOTION_PLAY_FADE_OUT_AFTER_END,
	MOTION_PLAY_FADE_OUT_BEFORE_END,
	MOTION_STOPPED,
	MOTION_READY,
	MOTION_PLAY_INVAILD
};


struct CMotionState
{
	int					currentKey;
	int					loopCount;

	float				passedTime;
	float				blendTime;

	float				blendRatio;	// between A Key and B Key
	eMOTION_PLAY_STATE	ePlayState;
	float				weight;
	
	long				id;	// id is assigned by generating order, this is also used for sorting motion Instances

	CMotionState()
	{
		Reset();
	}

	void Reset()
	{
		currentKey = 0;
		passedTime = 0;
		blendTime = 0;
		blendRatio = 0;
		ePlayState = MOTION_READY;
		weight = 0;
		loopCount = 0;
	}
};

struct CMotionTransform
{
	CQuat		rot;
	CVector3	pos;
};

typedef std::vector<CMotionTransform>	MOTION_POSE;

struct IMotionInstance
{
	virtual const CMotionDesc*	GetDesc() = 0;
	virtual const CMotionState*	GetState() = 0;							
	virtual eMOTION_PLAY_STATE	UpdateFrame(float timeDelta) = 0;
	virtual void				UpdateMatrix() = 0;

	virtual void	ApplyToMotionPose(MOTION_POSE* pMotionPose) = 0;
}; 