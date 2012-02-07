#include "wx/wx.h"
#include "SGlobal.h"
#include "SAnimationToolBar.h"



SAnimationToolBar::SAnimationToolBar(wxWindow *parent, wxWindowID id)
	: wxToolBar(parent, id,  wxDefaultPosition, wxSize(250, 35))
{	
	AddRadioTool( ID_ANIMATION_STOP, "stop", wxImage("res/stop.png") );
	AddRadioTool( ID_ANIMATION_PALY, "play", wxImage("res/play.png") );
	AddRadioTool( ID_ANIMATION_FREEZE, "freeze", wxImage("res/freeze.png") );

	AddControl(new wxSlider(this, ID_ANIMATION_PLAYTIME, 0, 0, 100, wxDefaultPosition, wxSize(90, -1), wxSL_HORIZONTAL ) );
	AddControl( new wxCheckBox( this, ID_ANIMATION_LOOP, wxT("Loop"), wxDefaultPosition, wxDefaultSize, 0 ) );

	this->Connect( ID_ANIMATION_PALY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SAnimationToolBar::OnPlay ) );
	this->Connect( ID_ANIMATION_FREEZE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SAnimationToolBar::OnFreeze ) );
	this->Connect( ID_ANIMATION_STOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SAnimationToolBar::OnStop ) );
	
	this->Connect( ID_ANIMATION_PLAYTIME, wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SAnimationToolBar::OnPlayTimeChanged ) );
	this->Connect( ID_ANIMATION_LOOP, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SAnimationToolBar::OnLoop ) );

	Realize();
}

void SAnimationToolBar::OnPlay( wxCommandEvent& event )
{
}

void SAnimationToolBar::OnFreeze( wxCommandEvent& event )
{
}

void SAnimationToolBar::OnStop( wxCommandEvent& event )
{
}

void SAnimationToolBar::OnPlayTimeChanged( wxScrollEvent& event )
{
}

void SAnimationToolBar::OnLoop( wxCommandEvent& event )
{
}
