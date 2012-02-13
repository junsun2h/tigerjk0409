#pragma once


class SPropertyGrid;

class SEntityTreeCtrl : public wxTreeCtrl
{
public:
	SEntityTreeCtrl(){}
	SEntityTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id);

	const char*		ProxyTypeString(eENTITY_PROXY_TYPE type);
	void			OnSelChanged(wxTreeEvent& event);
	void			SetEntity(IEntity* pEntity);

private:
	IEntity*		m_pEntity;
	SPropertyGrid*	m_pGrid;

	DECLARE_DYNAMIC_CLASS(SEntityTreeCtrl)
	DECLARE_EVENT_TABLE()
};
