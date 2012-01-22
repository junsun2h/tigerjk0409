#pragma once

#include "wx/wx.h"



class SNameDlg : public wxDialog 
{
private:

protected:
	wxTextCtrl* m_textCtrlName;
	wxButton* m_btnOK;
	wxButton* m_btnCancel;

	virtual void		OnBtnOK( wxCommandEvent& event )		{	EndModal(wxID_OK);	}
	virtual void		OnBtnCancel( wxCommandEvent& event )	{	EndModal(wxID_CANCEL);	}
	virtual wxString	GetName()								{	return m_textCtrlName->GetValue();	}

public:
	SNameDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Name Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 215,86 ), long style = wxDEFAULT_DIALOG_STYLE );
	{
		this->SetSizeHints( wxDefaultSize, wxDefaultSize );

		wxBoxSizer* bSizer2;
		bSizer2 = new wxBoxSizer( wxVERTICAL );

		wxBoxSizer* bSizer3;
		bSizer3 = new wxBoxSizer( wxVERTICAL );

		m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
		bSizer3->Add( m_textCtrl1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );

		wxGridSizer* gSizer3;
		gSizer3 = new wxGridSizer( 2, 2, 0, 0 );

		m_btnOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
		gSizer3->Add( m_btnOK, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		m_btnCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
		gSizer3->Add( m_btnCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

		bSizer2->Add( gSizer3, 1, wxEXPAND, 5 );

		this->SetSizer( bSizer2 );
		this->Layout();

		this->Centre( wxBOTH );

		// Connect Events
		m_btnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SNameDlg::OnOK ), NULL, this );
		m_btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SNameDlg::OnCancel ), NULL, this );
	}

	SNameDlg::~SNameDlg()
	{
		// Disconnect Events
		m_btnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SNameDlg::OnOK ), NULL, this );
		m_btnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SNameDlg::OnCancel ), NULL, this );
	}
};
