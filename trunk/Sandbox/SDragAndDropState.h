#pragma once

#include "wx/wx.h"


enum eDRAG_OBJECT_TYPE
{
	DND_ACTOR,
	DND_MESH,
	DND_ENTITY,
	DND_MOTION,
};


struct SDragAndDragDesc
{
	eDRAG_OBJECT_TYPE	type;
	wxString			strParam;
	int					iParam;
	float				fParam;
};


struct SDragAndDropState
{
	static void	Do(SDragAndDragDesc& desc, wxWindow *win);
	static bool	Get(SDragAndDragDesc&);
};