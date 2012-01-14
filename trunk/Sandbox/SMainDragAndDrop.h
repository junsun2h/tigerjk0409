#pragma once

#include "SGlobal.h"
#include "wx/dnd.h"


class SMainDragAndDrop : public wxFileDropTarget
{
public:
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
};