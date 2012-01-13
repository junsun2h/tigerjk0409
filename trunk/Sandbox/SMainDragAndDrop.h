#pragma once

#include "wx/wx.h"
#include "wx/dnd.h"
#include "SWxWidgetID.h"


class SMainDragAndDrop : public wxFileDropTarget
{
public:
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
};