#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/popupwin.h"

class SCanvas: public wxScrolledWindow
{
public:
	SCanvas( wxWindow *parent );
	virtual ~SCanvas();

	void OnPaint( wxPaintEvent &event );
	void SetImage( wxImage* pImg );

private:
	bool			m_bImage;
    wxBitmap		m_BitMap;

    DECLARE_EVENT_TABLE()
};



class STexturePopupWindow: public wxPopupTransientWindow
{
public:
	STexturePopupWindow( wxWindow *parent );
	
private:
	wxScrolledWindow *m_panel;

private:
	DECLARE_CLASS(STexturePopupWindow)
};


