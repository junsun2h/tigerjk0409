#pragma once


enum eDEFFERED_RENDER_TARGET
{
	RT_GPASS1,
	RT_GPASS2,
	RT_GPASS3,

	RT_LPASS,

	NUM_DEFFERED_RENDER_TARGET
};

struct IRenderTargetMgr
{
	virtual void	ClearAndSetMaineFrame() = 0;
	virtual void	SetDepthToResource() = 0;
};
