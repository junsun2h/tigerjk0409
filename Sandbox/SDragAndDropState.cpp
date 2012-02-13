#include "wx/wx.h"
#include "wx/dnd.h"

#include "SDragAndDropState.h"


SDragAndDragDesc g_DragAndDropState;
bool			 g_bHoldDragItem = false;

void SDragAndDropState::Do(SDragAndDragDesc& desc, wxWindow *win)
{
	g_DragAndDropState = desc;

	wxTextDataObject data;
	wxDropSource dragSource(win);
	dragSource.SetData(data);

	g_bHoldDragItem = true;
	dragSource.DoDragDrop();
	g_bHoldDragItem = false;
}

bool SDragAndDropState::Get(SDragAndDragDesc& desc)
{
	desc = g_DragAndDropState;
	return g_bHoldDragItem;
}