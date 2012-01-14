#include "SMainToolBar.h"

SMainToolBar::SMainToolBar(wxWindow *parent, wxWindowID id)
	: wxToolBar(parent, id)
{	
	AddRadioTool( ID_TOOLBAR_SELECT, "Select", wxImage("res/select.png") );
	AddRadioTool( ID_TOOLBAR_POSITION, "Position", wxImage("res/move.png") );
	AddRadioTool( ID_TOOLBAR_ROTATION, "Rotate", wxImage("res/rotate.png") );
	AddRadioTool( ID_TOOLBAR_SCALE, "Scale", wxImage("res/scale.png") );

	this->Connect( ID_TOOLBAR_SELECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnSelectMode ) );
	this->Connect( ID_TOOLBAR_POSITION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnPositionMode ) );
	this->Connect( ID_TOOLBAR_ROTATION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnRotateMode ) );
	this->Connect( ID_TOOLBAR_SCALE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnScaleMode ) );

	AddSeparator();

	Realize();
}

void SMainToolBar::OnSelectMode( wxCommandEvent& event )
{

}

void SMainToolBar::OnPositionMode( wxCommandEvent& event )
{

}

void SMainToolBar::OnRotateMode( wxCommandEvent& event )
{

}

void SMainToolBar::OnScaleMode( wxCommandEvent& event )
{

}
