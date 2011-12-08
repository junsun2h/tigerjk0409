#pragma once


#include "IEngine.h"


struct CENGINE_INIT_PARAM;


enum RENDER_OBJ_TYPE
{
	RENDER_OBJ_LAMBERT,
	RENDER_OBJ_NORMALMAP,
	RENDER_OBJ_ALPHA_BLEND,
	RENDER_OBJ_LINE,
	RENDER_OBJ_SKIN_MESH,
	RO_LIGHT_DIRECTIONAL,
	RO_LIGHT_OUTSIDE_OF_CAMERA,
	RO_LIGHT_INSIDE_OF_CAMERA,
	RO_LIGHT_SPOT_SHADOW,
	RENDER_OBJ_2D,

	NUM_RENDER_OBJ_TYPE,
};


struct IRenderer  
{
	virtual ~IRenderer(){}

	virtual bool			StartUp(const CENGINE_INIT_PARAM &param) = 0;
	virtual void			ShutDown() = 0;

	virtual void			Render(uint32 index) = 0;
	virtual bool			Resize(const CENGINE_INIT_PARAM &param) = 0;
};
