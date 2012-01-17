#pragma once

#include "wx/wx.h"


class SCreateEntityDlg : public wxDialog 
{
	virtual void OnBtnOK( wxCommandEvent& event )		{	EndModal(wxID_OK);	}
	virtual void OnBtnCancel( wxCommandEvent& event )	{	EndModal(wxID_CANCEL);	}

public:
	SCreateEntityDlg::SCreateEntityDlg( wxWindow* parent,  wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 210,200 ), long style = wxDEFAULT_DIALOG_STYLE )
		: wxDialog( parent, id, title, pos, size, style )
	{
		this->SetSizeHints( wxDefaultSize, wxDefaultSize );

		wxFlexGridSizer* pRootSizer;
		pRootSizer = new wxFlexGridSizer( 5, 2, 0, 0 );
		pRootSizer->SetFlexibleDirection( wxBOTH );
		pRootSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

		wxStaticText* m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("name"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText2->Wrap( -1 );
		pRootSizer->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		m_textCtrlName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		pRootSizer->Add( m_textCtrlName, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		wxStaticText* m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Postion X"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText3->Wrap( -1 );
		pRootSizer->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		m_textCtrlPosX = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		pRootSizer->Add( m_textCtrlPosX, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		wxStaticText* m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Postion Y"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText4->Wrap( -1 );
		pRootSizer->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		m_textCtrlPosY = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		pRootSizer->Add( m_textCtrlPosY, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		wxStaticText* m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Postion Z"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText5->Wrap( -1 );
		pRootSizer->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		m_textCtrlPosZ = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
		pRootSizer->Add( m_textCtrlPosZ, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		m_btnOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
		pRootSizer->Add( m_btnOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		m_btnCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
		pRootSizer->Add( m_btnCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		this->SetSizer( pRootSizer );
		this->Layout();

		this->Centre( wxBOTH );

		// Connect Events
		m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SCreateEntityDlg::OnBtnOK ), NULL, this );
		m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SCreateEntityDlg::OnBtnCancel ), NULL, this );
	}

	SCreateEntityDlg::~SCreateEntityDlg()
	{
		// Disconnect Events
		m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SCreateEntityDlg::OnBtnOK ), NULL, this );
		m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SCreateEntityDlg::OnBtnCancel ), NULL, this );
	}


	virtual void GetNameAndPosition(wxString& name, CVector3& pos)
	{
		name = m_textCtrlName->GetValue();
		pos.x = atof(m_textCtrlPosX->GetValue() );
		pos.y = atof(m_textCtrlPosY->GetValue() );
		pos.z = atof(m_textCtrlPosZ->GetValue() );
	}

protected:
	wxTextCtrl* m_textCtrlName;
	wxTextCtrl* m_textCtrlPosX;
	wxTextCtrl* m_textCtrlPosY;
	wxTextCtrl* m_textCtrlPosZ;
	wxButton* m_btnOK;
	wxButton* m_btnCancel;
};