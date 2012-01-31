#include "SMainToolBar.h"
#include "S3DViewPanel.h"
#include "SGlobal.h"
#include "SSelectionMgr.h"


SMainToolBar::SMainToolBar(wxWindow *parent, wxWindowID id)
	: wxToolBar(parent, id)
{	
	AddRadioTool( ID_TOOLBAR_SELECT, "Select", wxImage("res/select.png") );
	AddRadioTool( ID_TOOLBAR_POSITION, "Position", wxImage("res/move.png") );
	AddRadioTool( ID_TOOLBAR_ROTATION, "Rotate", wxImage("res/rotate.png") );
	AddRadioTool( ID_TOOLBAR_SCALE, "Scale", wxImage("res/scale.png") );

	AddSeparator();
	AddSeparator();

	wxStaticText* strCameraSpeed = new wxStaticText( this, wxID_ANY, wxT("Camera Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	strCameraSpeed->Wrap( -1 );
	AddControl( strCameraSpeed );
	
	AddSeparator();

	wxComboBox *combo = new wxComboBox(this, ID_TOOLBAR_CAMERA_SPEED, wxEmptyString, wxDefaultPosition, wxSize(100,-1) );
	combo->Append(wxT("0.01x"));
	combo->Append(wxT("0.1x"));
	combo->Append(wxT("1x"));
	combo->Append(wxT("10x"));
	combo->Append(wxT("100x"));
	combo->Select(2);

	AddControl(combo, wxT("Camera Speed"));
	
	this->Connect( ID_TOOLBAR_SELECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnSelectMode ) );
	this->Connect( ID_TOOLBAR_POSITION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnPositionMode ) );
	this->Connect( ID_TOOLBAR_ROTATION, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnRotateMode ) );
	this->Connect( ID_TOOLBAR_SCALE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SMainToolBar::OnScaleMode ) );
	this->Connect( ID_TOOLBAR_CAMERA_SPEED, wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( SMainToolBar::OnSelectCameraSpeed ) );
	combo->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( SMainToolBar::OnSelectCameraSpeed ), NULL, this );

	Realize();
}

void SMainToolBar::OnSelectMode( wxCommandEvent& event )
{
	GLOBAL::SelectionMgr()->SetTransformMode( TRANSFORM_SELECT );
}

void SMainToolBar::OnPositionMode( wxCommandEvent& event )
{
	GLOBAL::SelectionMgr()->SetTransformMode( TRANSFORM_MOVE );
}

void SMainToolBar::OnRotateMode( wxCommandEvent& event )
{
	GLOBAL::SelectionMgr()->SetTransformMode( TRANSFORM_ROTATE );
}

void SMainToolBar::OnScaleMode( wxCommandEvent& event )
{
	GLOBAL::SelectionMgr()->SetTransformMode( TRANSFORM_SCALE );
}

void SMainToolBar::OnSelectCameraSpeed( wxCommandEvent& event )
{
	switch( event.GetSelection() )
	{
	case 0: GLOBAL::ViewPanel()->m_CameraSpeed = 0.01f; break; 
	case 1: GLOBAL::ViewPanel()->m_CameraSpeed = 0.1f; break;
	case 2: GLOBAL::ViewPanel()->m_CameraSpeed = 1.0f; break;
	case 3: GLOBAL::ViewPanel()->m_CameraSpeed = 10.0f; break;
	case 4: GLOBAL::ViewPanel()->m_CameraSpeed = 100.0f; break;
	}
}