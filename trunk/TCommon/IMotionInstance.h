#pragma once


struct IEntity;
class CResourceMotion;


struct CMotionDesc
{
	float	fBlendInTime;
	float	fBlendOutTime;
	float	fTargetWeight;
	bool	bLoop;
	bool	bBlendOutBeforeEnd;
	int		Priority;

	float	timePerFrame;
	UINT	samplingCount;
	float	length;

	CResourceMotion*	pResource;

	CMotionDesc()
		: fBlendInTime(0.3f)
		, fBlendOutTime(0.3f)
		, fTargetWeight(1.0f)
		, bBlendOutBeforeEnd(true)
		, bLoop(false)
		, Priority(0)
		, pResource(NULL)
	{
	}
};


enum eMOTION_PLAY_STATE
{
	TMOTION_FADE_IN,
	TMOTION_PLAY,
	TMOTION_FADE_OUT_AFTER_END,
	TMOTION_FADE_OUT_BEFORE_END,
	TMOTION_STOP
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
		ePlayState = TMOTION_STOP;
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
	virtual void	Init(CMotionDesc& desc) = 0;
	virtual void	Destroy() = 0;

	virtual const CMotionDesc*	GetDesc() = 0;
	virtual const CMotionState*	GetState() = 0;
									
	virtual bool	Update(float timeDelta) = 0;
	virtual void	Stop(bool bInstant) = 0;
	virtual void	ApplyToMotionPose(MOTION_POSE* pMotionPose) = 0;
}; 