#include "wx/wx.h"
#include "wx/popupwin.h"

#include "STexturePopupWindow.h"


BEGIN_EVENT_TABLE(SCanvas, wxScrolledWindow)
	EVT_PAINT(SCanvas::OnPaint)
END_EVENT_TABLE()



SCanvas::SCanvas( wxWindow *parent )
	: wxScrolledWindow( parent , wxID_ANY, wxDefaultPosition,  wxSize(250, 200))
	, m_bImage(false)
{
	SetBackgroundColour( *wxLIGHT_GREY );
}

SCanvas::~SCanvas()
{
}

void SCanvas::OnPaint( wxPaintEvent &event )
{
	if( m_bImage == false )
		return;

	wxPaintDC dc( this );
	PrepareDC( dc );

	dc.DrawBitmap(m_BitMap, 0, 0, true);
}

void SCanvas::SetImage( wxImage* pImg )
{
	m_bImage = true;
	m_BitMap = wxBitmap( pImg->Scale(200,200,wxIMAGE_QUALITY_HIGH) );
}


//------------------------------------------------------------------------------------------------------------
IMPLEMENT_CLASS(STexturePopupWindow, wxPopupTransientWindow)

STexturePopupWindow::STexturePopupWindow( wxWindow *parent )
	:wxPopupTransientWindow( parent )
{
	m_panel = new wxScrolledWindow( this, wxID_ANY );
	m_panel->SetBackgroundColour( *wxLIGHT_GREY );

	wxStaticText *text = new wxStaticText( m_panel, wxID_ANY,
		wxT("wxPopupTransientWindow is a\n")
		wxT("wxPopupWindow which disappears\n")
		wxT("automatically when the user\n")
		wxT("clicks the mouse outside it or if it\n")
		wxT("(or its first child) loses focus in \n")
		wxT("any other way.") );

	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add( text, 0, wxALL, 5 );

	m_panel->SetSizer( topSizer );

	topSizer->Fit(m_panel);
	topSizer->Fit(this);
}